/*
 *   Copyright (C) 2014 Peter Grasch <peter.grasch@bedahr.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License version 2,
 *   or (at your option) any later version, as published by the Free
 *   Software Foundation
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "coreaudiobackend.h"
#include <simonsound/soundbackendclient.h>
#include <simonlogging/logger.h>
#include <CoreAudio/AudioHardware.h>
#include <AudioToolbox/AudioToolbox.h>
#include <Foundation/NSString.h>
#include <KDebug>

static QString getDeviceName(AudioDeviceID device);
static QStringList getDevices(bool inputDevices);
static void deriveInputBufferSize(AudioQueueRef audioQueue, AudioStreamBasicDescription& ASBDescription,
                      Float64 seconds, UInt32* outBufferSize);
static void handleInputBuffer(void *aqData, AudioQueueRef inAQ, AudioQueueBufferRef inBuffer,
                              const AudioTimeStamp *inStartTime, UInt32 inNumPackets,
                              const AudioStreamPacketDescription *inPacketDesc);
void deriveOutputBufferSize(AudioStreamBasicDescription &ASBDesc, UInt32 maxPacketSize, Float64 seconds,
                            UInt32 *outBufferSize);
static void handleOutputBuffer(void *state, AudioQueueRef inAQ, AudioQueueBufferRef inBuffer);

static const int kNumberBuffers = 3;

struct AQRecorderState {
    AudioStreamBasicDescription  mDataFormat;
    AudioQueueRef                mQueue;
    AudioQueueBufferRef          mBuffers[kNumberBuffers];
    UInt32                       bufferByteSize;
    SInt64                       mCurrentPacket;
    bool                         mIsRunning;
    CoreAudioBackend*            mCoreAudioBackend;
    SoundBackendClient*          mClient;
    bool                         mIsPlayback;
};

CoreAudioBackend::CoreAudioBackend() : m_bufferSize(1024), aqData(0)
{
}

CoreAudioBackend::~CoreAudioBackend()
{
  delete aqData;
}

QStringList CoreAudioBackend::getAvailableInputDevices()
{
  return getDevices(true);
}

QStringList CoreAudioBackend::getAvailableOutputDevices()
{
  return getDevices(false);
}

bool CoreAudioBackend::check(SimonSound::SoundDeviceType type, const QString& device, int channels, int samplerate)
{
  Q_UNUSED(type);
  Q_UNUSED(device);
  Q_UNUSED(channels);
  Q_UNUSED(samplerate);
  return true; // Automatic resampling
}

QString getDefaultDevice(bool inputDevice)
{
  AudioDeviceID defaultId = 0;
  UInt32 propertySize = sizeof(AudioDeviceID);
  AudioObjectPropertyAddress address = { (inputDevice) ? kAudioHardwarePropertyDefaultInputDevice :
                                                         kAudioHardwarePropertyDefaultOutputDevice,
                                            kAudioObjectPropertyScopeGlobal,
                                            kAudioObjectPropertyElementMaster };
  if (AudioObjectGetPropertyData(kAudioObjectSystemObject,
                                 &address, 0, NULL, &propertySize, &defaultId) != noErr) {
    kWarning() << "Failed to get default device";
    return QString();
  }
  return getDeviceName(defaultId);
}

QString CoreAudioBackend::getDefaultInputDevice()
{
  return getDefaultDevice(true);
}

QString CoreAudioBackend::getDefaultOutputDevice()
{
  return getDefaultDevice(false);
}


int CoreAudioBackend::bufferSize()
{
  return m_bufferSize;
}

//recording
bool CoreAudioBackend::prepareRecording(const QString& device, int& channels, int& samplerate)
{
  if (!prepare(device, channels, samplerate, false))
    return false;

  deriveInputBufferSize(aqData->mQueue, aqData->mDataFormat, 0.25, &aqData->bufferByteSize);
  m_bufferSize = aqData->bufferByteSize;

  for (int i = 0; i < kNumberBuffers; ++i) {
    AudioQueueAllocateBuffer(aqData->mQueue, aqData->bufferByteSize, &aqData->mBuffers[i]);
    AudioQueueEnqueueBuffer(aqData->mQueue, aqData->mBuffers[i], 0, NULL);
  }

  emit stateChanged(SimonSound::PreparedState);
  return true;
}

bool CoreAudioBackend::startRecording(SoundBackendClient *client)
{
  if (aqData && aqData->mIsPlayback) {
    emit errorOccured(SimonSound::BackendBusy);
    return false;
  }

  aqData->mClient = m_client = client;

  aqData->mCurrentPacket = 0;
  aqData->mIsRunning = true;

  bool ret = (AudioQueueStart(aqData->mQueue, NULL) == noErr);
  if (ret)
    emit stateChanged(SimonSound::ActiveState);
  else
    emit errorOccured(SimonSound::OpenError);

  return ret;
}

static void handleInputBuffer(void *aqData, AudioQueueRef inAQ, AudioQueueBufferRef inBuffer,
                              const AudioTimeStamp *inStartTime, UInt32 inNumPackets,
                              const AudioStreamPacketDescription *inPacketDesc)
{
  Q_UNUSED(inAQ);
  Q_UNUSED(inStartTime);
  Q_UNUSED(inNumPackets);
  Q_UNUSED(inPacketDesc);
  AQRecorderState* state = static_cast<AQRecorderState*>(aqData);
  SoundBackendClient *m_client = state->mClient;

  // write data to client
  qint64 written = m_client->writeData(static_cast<char*>(inBuffer->mAudioData), inBuffer->mAudioDataByteSize);
  state->mCurrentPacket += (written / state->mDataFormat.mBytesPerPacket);

  if (state->mIsRunning == 0)
    return;

  //re-enqueue buffer
  AudioQueueEnqueueBuffer(state->mQueue, inBuffer, 0, NULL);
}

bool CoreAudioBackend::stopRecording()
{
  if (aqData && aqData->mIsPlayback) {
    emit errorOccured(SimonSound::BackendBusy);
    return false;
  }

  AudioQueueStop(
    aqData->mQueue,
    true
  );

  aqData->mIsRunning = false;
  AudioQueueDispose(aqData->mQueue, true);

  emit stateChanged(SimonSound::IdleState);
  return true;
}

//playback
bool CoreAudioBackend::preparePlayback(const QString& device, int& channels, int& samplerate)
{
  kDebug() << "Preparing playback " << device << channels << samplerate;
  if (!prepare(device, channels, samplerate, true))
    return false;
  kDebug() << "Preparing playback part 2 " << device << channels << samplerate;

  deriveOutputBufferSize(aqData->mDataFormat, 1, 0.5, &aqData->bufferByteSize);
  m_bufferSize = aqData->bufferByteSize;

  for (int i = 0; i < kNumberBuffers; ++i) {
    AudioQueueAllocateBuffer(aqData->mQueue, aqData->bufferByteSize, &aqData->mBuffers[i]);
  }

  bool ret = (AudioQueueSetParameter(aqData->mQueue, kAudioQueueParam_Volume, 1.0) == noErr);
  if (ret)
    emit stateChanged(SimonSound::PreparedState);
  else
    emit errorOccured(SimonSound::OpenError);
  return ret;
}

bool CoreAudioBackend::startPlayback(SoundBackendClient *client)
{
  if (aqData && !aqData->mIsPlayback) {
    emit errorOccured(SimonSound::BackendBusy);
    return false;
  }
  aqData->mClient = m_client = client;
  aqData->mIsRunning = true;
  for (int i = 0; i < kNumberBuffers; ++i) {
    handleOutputBuffer(aqData, aqData->mQueue, aqData->mBuffers[i]);
  }

  bool ret = (AudioQueueStart(aqData->mQueue, NULL) == noErr);
  if (ret)
    emit stateChanged(SimonSound::ActiveState);
  else
    emit errorOccured(SimonSound::OpenError);
  return ret;
}

static void handleOutputBuffer(void *state, AudioQueueRef inAQ, AudioQueueBufferRef inBuffer)
{
  Q_UNUSED(inAQ);
  AQRecorderState* pAqData = static_cast<AQRecorderState*>(state);

  if (pAqData->mIsRunning == 0)
    return;

  qint64 read = pAqData->mClient->readData((char*) inBuffer->mAudioData, pAqData->bufferByteSize);

  if (read == -1) {
    AudioQueueStop(pAqData->mQueue, false);
    pAqData->mIsRunning = false;
  } else {
    inBuffer->mAudioDataByteSize = read;
    AudioQueueEnqueueBuffer(pAqData->mQueue, inBuffer, 0, NULL);
  }
}

bool CoreAudioBackend::stopPlayback()
{
  if (aqData && !aqData->mIsPlayback) {
    emit errorOccured(SimonSound::BackendBusy);
    return false;
  }
  AudioQueueDispose(aqData->mQueue, true);

  return true;
}

bool CoreAudioBackend::prepare(const QString& device, int& channels, int& samplerate, bool isPlayback)
{
  if (aqData && (isPlayback != aqData->mIsPlayback)) {
    emit errorOccured(SimonSound::BackendBusy);
    return false;
  }
  delete aqData;
  aqData = new AQRecorderState;
  aqData->mIsPlayback                   = isPlayback;
  aqData->mDataFormat.mFormatID         = kAudioFormatLinearPCM;
  aqData->mDataFormat.mSampleRate       = samplerate;
  aqData->mDataFormat.mChannelsPerFrame = channels;
  aqData->mDataFormat.mBitsPerChannel   = 16;
  aqData->mDataFormat.mBytesPerPacket   =
    aqData->mDataFormat.mBytesPerFrame =
      aqData->mDataFormat.mChannelsPerFrame * sizeof (SInt16);
  aqData->mDataFormat.mFramesPerPacket  = 1;

  aqData->mDataFormat.mFormatFlags = kLinearPCMFormatFlagIsSignedInteger | kLinearPCMFormatFlagIsPacked;

  if (isPlayback)
    AudioQueueNewOutput(&aqData->mDataFormat, handleOutputBuffer, aqData, CFRunLoopGetCurrent(), kCFRunLoopCommonModes,
                       0, &aqData->mQueue);
  else
    AudioQueueNewInput(&aqData->mDataFormat, handleInputBuffer, aqData, NULL, kCFRunLoopCommonModes,
                       0, &aqData->mQueue);

  //set device
  QString uid = device.mid(device.lastIndexOf("(") + 1);
  uid = uid.left(uid.indexOf(")"));
  CFStringRef uidString = CFStringCreateWithCString(NULL, uid.toLocal8Bit().constData(), CFStringGetSystemEncoding());
  if (AudioQueueSetProperty(aqData->mQueue, kAudioQueueProperty_CurrentDevice, &uidString, sizeof(uidString)) != noErr) {
    kWarning() << "Failed to select audio device " << device;
    return false;
  }
  CFRelease(uidString);

  UInt32 dataFormatSize = sizeof(aqData->mDataFormat);

if (!isPlayback) {
  int err = AudioQueueGetProperty(aqData->mQueue, (isPlayback) ?
                                         kAudioConverterCurrentOutputStreamDescription :
                                         kAudioConverterCurrentInputStreamDescription,
                        &aqData->mDataFormat, &dataFormatSize);
  if (err != noErr) {
    kWarning() << "Failed to get applied data format" << err;
    return false;
  }
  samplerate = aqData->mDataFormat.mSampleRate;
  channels = aqData->mDataFormat.mChannelsPerFrame;
}
  return true;
}

static QString getDeviceName(AudioDeviceID device)
{
  CFStringRef deviceName;
  CFStringRef manufacturerName;
  CFStringRef uidString;
  QString qDeviceName;
  QString qManufacturerName;
  QString qAddress;
  AudioObjectPropertyAddress propertyAddress;
  UInt32 propertySize;

  //device name
  propertySize = sizeof(deviceName);
  propertyAddress.mSelector = kAudioObjectPropertyName;
  if (AudioObjectGetPropertyData(device, &propertyAddress, 0, NULL, &propertySize, &deviceName) != noErr) {
    kWarning() << "Failed to get device name for device";
    return QString();
  }
  qDeviceName = QString::fromLocal8Bit(CFStringGetCStringPtr(deviceName, CFStringGetSystemEncoding()));
  CFRelease(deviceName);

  //manufacturer name
  propertySize = sizeof(manufacturerName);
  propertyAddress.mSelector = kAudioObjectPropertyManufacturer;
  if (AudioObjectGetPropertyData(device, &propertyAddress, 0, NULL, &propertySize, &manufacturerName) != noErr) {
    kWarning() << "Failed to get manufacturer name for device " << qDeviceName;
    return QString();
  }
  qManufacturerName = QString::fromLocal8Bit(CFStringGetCStringPtr(manufacturerName, CFStringGetSystemEncoding()));
  CFRelease(manufacturerName);

  //address
  propertySize = sizeof(uidString);
  propertyAddress.mSelector = kAudioDevicePropertyDeviceUID;
  if (AudioObjectGetPropertyData(device, &propertyAddress, 0, NULL, &propertySize, &uidString) != noErr) {
    kWarning() << "Failed to get address for device " << qDeviceName;
    return QString();
  }
  qAddress  = QString::fromLocal8Bit(CFStringGetCStringPtr(uidString, CFStringGetSystemEncoding()));
  CFRelease(uidString);
  return i18nc("%1 is device name, %2 is manufacturer name and %3 is the id (which needs to be at "
                     "the end and wrapped in paranthesis", "%1 by %2 (%3)", qDeviceName,
                     qManufacturerName, qAddress);
}

/*
 * Roughly based on:
 * http://stackoverflow.com/questions/1983984/how-to-get-audio-device-uid-to-pass-into-nssounds-setplaybackdeviceidentifier
 */
static QStringList getDevices(bool inputDevices)
{
  QStringList devices;
  AudioObjectPropertyAddress propertyAddress;
  UInt32 propertySize;
  AudioObjectID *deviceIDs;
  NSInteger numDevices;
  QString thisDeviceName;

  propertyAddress.mSelector = kAudioHardwarePropertyDevices;
  propertyAddress.mScope = (inputDevices) ? kAudioDevicePropertyScopeInput : kAudioDevicePropertyScopeOutput;
  propertyAddress.mElement = kAudioObjectPropertyElementMaster;
  if (AudioObjectGetPropertyDataSize(kAudioObjectSystemObject, &propertyAddress, 0, NULL, &propertySize) != noErr) {
    kWarning() << "Failed to get property data size";
    return devices;
  }

  numDevices = propertySize / sizeof(AudioDeviceID);
  deviceIDs = (AudioDeviceID *)calloc(numDevices, sizeof(AudioDeviceID));

  if (AudioObjectGetPropertyData(kAudioObjectSystemObject, &propertyAddress, 0, NULL, &propertySize, deviceIDs) != noErr) {
    kWarning() << "Failed to get device list";
    return devices;
  }
  for (NSInteger idx=0; idx<numDevices; idx++) {
    //channels
    propertyAddress.mSelector = kAudioDevicePropertyStreamConfiguration;
    if (AudioObjectGetPropertyDataSize(deviceIDs[idx], &propertyAddress, 0, NULL, &propertySize) != noErr) {
      kWarning() << "Failed to get channel information size for device " << idx;
      continue;
    }
    AudioBufferList *bufferList = static_cast<AudioBufferList *>(malloc(propertySize));
    if (AudioObjectGetPropertyData(deviceIDs[idx], &propertyAddress, 0, NULL, &propertySize, bufferList) != noErr) {
      kWarning() << "Failed to get channel information for device " << idx;
      delete bufferList;
      continue;
    }
    if (bufferList->mNumberBuffers == 0) {
      delete bufferList;
      continue;
    }
    delete bufferList;
    thisDeviceName = getDeviceName(deviceIDs[idx]);
    if (!thisDeviceName.isNull())
      devices << thisDeviceName;
  }
  free(deviceIDs);
  return devices;
}

static void deriveInputBufferSize(AudioQueueRef audioQueue, AudioStreamBasicDescription& ASBDescription,
                      Float64 seconds, UInt32* outBufferSize)
{
  static const int maxBufferSize = 0x1500;

  int maxPacketSize = ASBDescription.mBytesPerPacket;
  if (maxPacketSize == 0) {
    UInt32 maxVBRPacketSize = sizeof(maxPacketSize);
    AudioQueueGetProperty(audioQueue, kAudioQueueProperty_MaximumOutputPacketSize, &maxPacketSize, &maxVBRPacketSize);
  }
  Float64 numBytesForTime = ASBDescription.mSampleRate * maxPacketSize * seconds;
  *outBufferSize = UInt32 (numBytesForTime < maxBufferSize ? numBytesForTime : maxBufferSize);
}

void deriveOutputBufferSize(AudioStreamBasicDescription &ASBDesc, UInt32 maxPacketSize, Float64 seconds,
                       UInt32 *outBufferSize)
{
    static const int maxBufferSize = 0x1500;
    static const int minBufferSize = 0x500;

    if (ASBDesc.mFramesPerPacket != 0) {
        Float64 numPacketsForTime = ASBDesc.mSampleRate / ASBDesc.mFramesPerPacket * seconds;
        *outBufferSize = numPacketsForTime * maxPacketSize;
    } else {
        *outBufferSize = maxBufferSize > maxPacketSize ? maxBufferSize : maxPacketSize;
    }

    if (*outBufferSize > maxBufferSize && *outBufferSize > maxPacketSize)
        *outBufferSize = maxBufferSize;
    else {
        if (*outBufferSize < minBufferSize)
            *outBufferSize = minBufferSize;
    }
}
