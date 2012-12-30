/*
*   Copyright (C) 2011 Patrick von Reth <patrick.vonreth@gmail.com>
*   Copyright (C) 2011 Peter Grasch <peter.grasch@bedahr.org>
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

#include "directsoundbackend.h"
#include <unistd.h>
#include <simonsound/soundbackendclient.h>
#include <QThread>
#include <KDebug>
#include <KLocalizedString>

//krazy:excludeall=captruefalse

#ifdef __MINGW64_VERSION_MAJOR
//libdxerr.a is missing on mingw so no error support :(
#define DXERR_TO_STRING(x) ""
#else
#include <dxerr.h>
#define DXERR_TO_STRING(x) QString::fromWCharArray(DXGetErrorDescription(x))<<QString::fromWCharArray(DXGetErrorString(x))
#endif

#define SAFE_RELEASE(x) if (x) { x->Release(); x = 0; }
#define NOTIFY_NUM 16
#define SIMON_WAIT_TIMEOUT 5000

HANDLE DirectSoundBackend::s_deviceCallbackEvent = CreateEvent(0, FALSE, FALSE, NULL);

class DirectSoundLoop : public QThread {
protected:
    DirectSoundBackend *m_parent;
    bool shouldRun;
public:
    DirectSoundLoop(DirectSoundBackend *parent) : m_parent(parent),
        shouldRun(true)
    {}

    void stop() {
        shouldRun = false;
    }
};

//Capture loop
class DirectSoundCaptureLoop : public DirectSoundLoop
{
public:
    DirectSoundCaptureLoop(DirectSoundBackend *parent) : DirectSoundLoop(parent)
    {}

    void run()
    {
        shouldRun = true;

        HRESULT hr = 0;
        char * buffer = new char[m_parent->m_bufferSize];
        memset(buffer, 0, m_parent->m_bufferSize);

        void* capture1 = NULL;
        void* capture2 = NULL;
        DWORD captureLength1, captureLength2;
        UINT dataWritten = 0;
        DWORD dwMyReadCursor = 0;
        DWORD dwReadPos;
        LONG lockSize;

        if(FAILED(hr = m_parent->m_primaryBufferC->Start( DSCBSTART_LOOPING ) )){
            kWarning()<<"Failed to start recording"<<DXERR_TO_STRING(hr);
        }


        while(shouldRun && hr == 0){
            HRESULT lr = WaitForSingleObject(m_parent->m_bufferEvents,SIMON_WAIT_TIMEOUT);
            if(lr == WAIT_FAILED){
                kWarning()<<"Event loop failed";
                break;
            }else if(lr != WAIT_OBJECT_0){
                continue;
            }
            if(FAILED(hr = m_parent->m_primaryBufferC->GetCurrentPosition(NULL,&dwReadPos))){
                kWarning()<<"Failed to get cursor"<<DXERR_TO_STRING(hr);
                break;
            }

            lockSize = dwReadPos - dwMyReadCursor;
            if( lockSize  < 0 )
                lockSize += m_parent->m_bufferSize;

            if (lockSize == 0){
                kWarning()<<"lock size should not be 0";
                continue;
            }

            if (FAILED(hr = m_parent->m_primaryBufferC->Lock(dwMyReadCursor, lockSize,&capture1, &captureLength1, &capture2, &captureLength2, 0))){
                kWarning()<<"Capture lock failure"<<lockSize<<DXERR_TO_STRING(hr);
                break;
            }



            memcpy(buffer,capture1,captureLength1);
            if (capture2 != NULL){
                memcpy(buffer+captureLength1,capture2,captureLength2);
            }

            dataWritten  = m_parent->m_client->writeData(buffer, lockSize);


            m_parent->m_primaryBufferC->Unlock(capture1,captureLength1,capture2,captureLength2);
            dwMyReadCursor += dataWritten;
            dwMyReadCursor %= m_parent->m_bufferSize;

        }
        if(hr != 0){
            kWarning()<<"Recorde error";
            m_parent->errorRecoveryFailed();
        }

        kWarning()<<"Record loop ended";
        delete[] buffer;
        m_parent->closeSoundSystem();
        shouldRun = false;
    }
};


//Playback loop
class DirectSoundPlaybackLoop : public DirectSoundLoop
{
public:
    DirectSoundPlaybackLoop(DirectSoundBackend *parent) : DirectSoundLoop(parent)
    {}

    bool checkCursor(ulong &cursor,const ulong &written)
    {
        if(written == -1)
            return false;
        cursor += written;
        cursor = cursor % m_parent->m_bufferSize;
        return true;
    }

    void run()
    {
        shouldRun = true;
        HRESULT hr = 0;

        void* audioBuffer1 = 0;
        void* audioBuffer2 = 0;
        ulong sizeAudioBuffer1 = 0;
        ulong sizeAudioBuffer2 = 0;
        ulong playCursor = 0;
        ulong myCursor = 0;
        long lockSize = 0;


        if (FAILED(hr = m_parent->m_primaryBuffer->Lock(0,0, &audioBuffer1, &sizeAudioBuffer1, NULL, 0, DSBLOCK_ENTIREBUFFER))) {
            kWarning() << "Lock DirectSoundBuffer Failed!"<<DXERR_TO_STRING(hr);
            m_parent->errorRecoveryFailed();
            return;
        }
        checkCursor(myCursor,m_parent->m_client->readData((char*) audioBuffer1,sizeAudioBuffer1));
        m_parent->m_primaryBuffer->Unlock(audioBuffer1, sizeAudioBuffer1,NULL, 0);

        //Begin playback
        if(FAILED(hr = m_parent->m_primaryBuffer->Play(0, 0, DSBPLAY_LOOPING))){
            kWarning()<<"Starting playback failed"<<DXERR_TO_STRING(hr);
            shouldRun = false;
        }

        //Playback Loop
        ///////////////
        while (shouldRun && hr == 0) {
            HRESULT lr = WaitForSingleObject(m_parent->m_bufferEvents, SIMON_WAIT_TIMEOUT);
            if(lr == WAIT_FAILED){
                kWarning()<<"Event loop failed";
                break;
            }else if(lr != WAIT_OBJECT_0){
                continue;
            }

            if(FAILED(hr = m_parent->m_primaryBuffer->GetCurrentPosition(&playCursor,NULL))){
                kWarning()<<"Failed to get cursor"<<DXERR_TO_STRING(hr);
                break;
            }
            //I can write until the play cursor

            lockSize = playCursor - myCursor;
            if( lockSize  < 0 )
                lockSize += m_parent->m_bufferSize;

            if (lockSize == 0){
                kWarning()<<"lock size should not be 0";
                continue;
            }
            //Lock DirectSoundBuffer Second Part
            if ( FAILED(hr = m_parent->m_primaryBuffer->Lock(myCursor, lockSize, &audioBuffer1, &sizeAudioBuffer1, &audioBuffer2, &sizeAudioBuffer2, 0)) ) {
                kWarning() << "Lock DirectSoundBuffer Failed!"<<lockSize<<DXERR_TO_STRING(hr);
                break;
            }

            if( !checkCursor(myCursor,m_parent->m_client->readData((char*)audioBuffer1, sizeAudioBuffer1))){
                //end of file
                memset(audioBuffer1,0,sizeAudioBuffer1);
                memset(audioBuffer2,0,sizeAudioBuffer2);
                m_parent->m_primaryBuffer->Unlock(audioBuffer1, sizeAudioBuffer1, audioBuffer2, sizeAudioBuffer2);
                kWarning()<<"Reached eof";
                break;
            }
            if(lockSize>sizeAudioBuffer1)
            {
                if( !checkCursor(myCursor,m_parent->m_client->readData((char*)audioBuffer2, sizeAudioBuffer2)))
                {
                    //end of file
                    memset(audioBuffer2,0,sizeAudioBuffer2);
                    m_parent->m_primaryBuffer->Unlock(audioBuffer1, sizeAudioBuffer1, audioBuffer2, sizeAudioBuffer2);
                    kWarning()<<"Reached eof";
                    break;
                }
            }

            //Unlock DirectSoundBuffer
            m_parent->m_primaryBuffer->Unlock(audioBuffer1, sizeAudioBuffer1, audioBuffer2, sizeAudioBuffer2);


        }
        if(hr != 0){
            m_parent->errorRecoveryFailed();
        }

        bool neeedCrossZero = myCursor<playCursor;
        while (hr == 0) {
            HRESULT lr = WaitForSingleObject(m_parent->m_bufferEvents, SIMON_WAIT_TIMEOUT);
            if(lr == WAIT_FAILED){
                kWarning()<<"Event loop failed";
                break;
            }
            if(FAILED(hr = m_parent->m_primaryBuffer->GetCurrentPosition(&playCursor,NULL))){
                kWarning()<<"Failed to get cursor"<<DXERR_TO_STRING(hr);
                break;
            }
            if(neeedCrossZero)
            {
                if(myCursor > playCursor)
                    neeedCrossZero = false;
            }
            else
            {
                if(playCursor >= myCursor)
                    break;
            }
        }
        kWarning() << "Exiting run loop";
        shouldRun = false;
        m_parent->closeSoundSystem();
    }
};





DirectSoundBackend::DirectSoundBackend() : 
    m_loop(0),
    m_notify(0),
    m_handle(0),
    m_primaryBuffer(0),
    m_handleC(0),
    m_primaryBufferC(0),
    m_bufferSize(1024)
{
    m_bufferEvents = CreateEvent(0, FALSE, FALSE, L"Direct_Sound_Buffer_Notify");
}

int DirectSoundBackend::bufferSize()
{
    //do not return the size of the directsound ring buffer but the size when we probably will write data
    return m_notifySize;
}

QStringList DirectSoundBackend::getAvailableInputDevices()
{
    return getDevices(SimonSound::Input);
}

QStringList DirectSoundBackend::getAvailableOutputDevices()
{
    return getDevices(SimonSound::Output);
}

BOOL CALLBACK DirectSoundEnumerateCallback(LPGUID pGUID, LPCWSTR pcName, LPCWSTR pcDriver, LPVOID pContext)
{
    Q_UNUSED(pcDriver);
    if (!pGUID) {
        //primary sound driver
        return true;
    }
    OLECHAR lplpsz[40]={0};
    StringFromGUID2(*pGUID, lplpsz, 40);
    QString uid = QString::fromWCharArray(lplpsz);

    QString deviceName(QString::fromWCharArray(pcName));
    deviceName += QString(" (%1)").arg(uid);

    kDebug() << "Found device: " << deviceName;
    ((DirectSoundBackend*) pContext)->deviceFound(deviceName);
    SetEvent(DirectSoundBackend::s_deviceCallbackEvent);
    return true;
}

void DirectSoundBackend::deviceFound(const QString& name)
{
    m_devices << name;
}

QStringList DirectSoundBackend::getDevices(SimonSound::SoundDeviceType type)
{
    m_devices.clear();

    m_devices << i18n("Default audio device");

    if (type == SimonSound::Input)
        DirectSoundCaptureEnumerate(DirectSoundEnumerateCallback, this);
    else
        DirectSoundEnumerate(DirectSoundEnumerateCallback, this);

    WaitForSingleObject(DirectSoundBackend::s_deviceCallbackEvent ,SIMON_WAIT_TIMEOUT);
    ResetEvent(DirectSoundBackend::s_deviceCallbackEvent);
    return m_devices;
}

bool DirectSoundBackend::setupNotifer( IUnknown **primaryBuffer,LPDIRECTSOUNDNOTIFY *notify){
    HRESULT hr;
    if(FAILED(hr = (*primaryBuffer)->QueryInterface(IID_IDirectSoundNotify, (void**)(notify)))){
        kWarning() << "Query interface failed"<<DXERR_TO_STRING(hr);
        freeAllResources();
        return false;
    }

    //calculate notify positions

    DSBPOSITIONNOTIFY pPosNotify[NOTIFY_NUM];
    for(int i=0;i<NOTIFY_NUM;++i){
        pPosNotify[i].dwOffset = (m_notifySize * i) + m_notifySize - 1;
        pPosNotify[i].hEventNotify = m_bufferEvents;
    }

    if ( FAILED(hr = (*notify)->SetNotificationPositions(NOTIFY_NUM, pPosNotify)) ) {
        kWarning() << "Set NotificationPosition Failed!"<<DX_SHARED_DEFINES(hr);
        freeAllResources();
        return false;
    }
    return true;
}

bool DirectSoundBackend::openOutputDevice(GUID *deviceID,LPDIRECTSOUND8* ppDS8, LPDIRECTSOUNDBUFFER8 *primaryBuffer){
    HRESULT hr;

    if(FAILED(hr = DirectSoundCreate8(deviceID, ppDS8, 0))) {
        kWarning() << "Failed to open device"<<DXERR_TO_STRING(hr);
        *ppDS8 = 0;
        emit errorOccured(SimonSound::OpenError);
        return false;
    }

    DSBUFFERDESC BufferDesc;
    HWND hWnd = GetForegroundWindow();
    if (hWnd == 0)
        hWnd = GetDesktopWindow();

    if(FAILED(hr = (*ppDS8)->SetCooperativeLevel(hWnd, DSSCL_PRIORITY))){
        kWarning() << "Failed to set CooperationLevel"<<DXERR_TO_STRING(hr);
        return false;
    }


    //init the primary buffer which will contain the raw data
    //--------------------------------
    memset( &BufferDesc, 0,sizeof(BufferDesc) );
    BufferDesc.dwSize     = sizeof(DSBUFFERDESC);
    BufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER;
    BufferDesc.dwBufferBytes = 0;
    BufferDesc.lpwfxFormat    = NULL;
    BufferDesc.guid3DAlgorithm = GUID_NULL;

    LPDIRECTSOUNDBUFFER tmp = NULL;
    if ( FAILED(hr = (*ppDS8)->CreateSoundBuffer(&BufferDesc, &tmp, NULL)) ) {
        kWarning()<<"Failed to create primary buffer"<<DXERR_TO_STRING(hr);
        return false;
    }

    if(FAILED(hr = tmp->SetFormat(&m_waveFormat))){
        kWarning()<<"failed to set wave format"<<DXERR_TO_STRING(hr);
        return false;
    }

    tmp->Release();
    //--------------------------------

    //init the secondary buffer on which we will write, if there would be multiple secondary buffers they would get autmaticaly merged in the primary buffer

    // DSBUFFERDESC
    BufferDesc.dwFlags      =	DSBCAPS_CTRLPOSITIONNOTIFY |DSBCAPS_CTRLFREQUENCY |DSBCAPS_GLOBALFOCUS;
    BufferDesc.dwBufferBytes  = m_bufferSize;
    BufferDesc.lpwfxFormat    = &m_waveFormat;

    if(FAILED((*ppDS8)->CreateSoundBuffer(&BufferDesc, &tmp, 0))) {
        kWarning() << "Couldn't create sound buffer.";
        (*ppDS8)->Release();
        *ppDS8 = 0;
        (*primaryBuffer)->Release();
        *primaryBuffer = 0;
        emit errorOccured(SimonSound::OpenError);
        tmp->Release();
        return false;
    }


    if(FAILED(hr = tmp->QueryInterface(IID_IDirectSoundBuffer8 , (void**)(primaryBuffer)))){
        kWarning() << "Query interface failed"<<DXERR_TO_STRING(hr);
        freeAllResources();
        tmp->Release();
        return false;
    }

    tmp->Release();
    return true;
}

bool DirectSoundBackend::openInputDevice(GUID *deviceID,LPDIRECTSOUNDCAPTURE8* ppDS8C, LPDIRECTSOUNDCAPTUREBUFFER8 *primaryBufferC){
    HRESULT hr;
    LPDIRECTSOUNDCAPTUREBUFFER tmp = NULL;

    if(FAILED( hr = DirectSoundCaptureCreate8(deviceID, ppDS8C, 0))) {
        kWarning() << "Failed to open capture device"<<DXERR_TO_STRING(hr);
        *ppDS8C = 0;
        emit errorOccured(SimonSound::OpenError);
        return false;
    }
    DSCBUFFERDESC CaptureBufferDesc;

    memset( &CaptureBufferDesc, 0,sizeof(CaptureBufferDesc) );
    CaptureBufferDesc.dwSize     = sizeof(DSCBUFFERDESC);
    CaptureBufferDesc.dwBufferBytes  = m_bufferSize; // 1 seconds of sound
    CaptureBufferDesc.lpwfxFormat    = &m_waveFormat;

    if(FAILED(hr = (*ppDS8C)->CreateCaptureBuffer(&CaptureBufferDesc,&tmp, NULL))) {
        kWarning() << "Failed to create primary recording buffer"<<DXERR_TO_STRING(hr);
        (*ppDS8C)->Release();
        *ppDS8C = 0;
        *primaryBufferC = 0;
        return false;
    }

    if(FAILED(hr = tmp->QueryInterface(IID_IDirectSoundCaptureBuffer8 , (void**)(primaryBufferC)))){
        kWarning() << "Query interface failed"<<DXERR_TO_STRING(hr);
        freeAllResources();
        tmp->Release();
        return false;
    }
    tmp->Release();
    return true;
}

bool DirectSoundBackend::openDevice(SimonSound::SoundDeviceType type, const QString& device, int channels, int samplerate, 
                                    LPDIRECTSOUND8* ppDS8, LPDIRECTSOUNDBUFFER8 *primaryBuffer,
                                    LPDIRECTSOUNDCAPTURE8* ppDS8C, LPDIRECTSOUNDCAPTUREBUFFER8 *primaryBufferC, LPDIRECTSOUNDNOTIFY *notify)
{
    //init sound
    // audioformat
    memset(&m_waveFormat,0, sizeof(m_waveFormat));
    m_waveFormat.wFormatTag   = WAVE_FORMAT_PCM;
    m_waveFormat.nChannels    = channels;
    m_waveFormat.nSamplesPerSec = samplerate;
    m_waveFormat.wBitsPerSample = 16; //S16_LE
    m_waveFormat.nBlockAlign    = m_waveFormat.nChannels * (m_waveFormat.wBitsPerSample /8 );
    m_waveFormat.nAvgBytesPerSec  = m_waveFormat.nSamplesPerSec * m_waveFormat.nBlockAlign;
    m_waveFormat.cbSize     = 0;

    m_notifySize= qMax( (ulong)1024, m_waveFormat.nAvgBytesPerSec / 8 );
    m_notifySize -= m_notifySize % m_waveFormat.nBlockAlign;
    m_bufferSize = m_notifySize * NOTIFY_NUM;


    m_sampleRate = samplerate;
    m_blockAlign = m_waveFormat.nBlockAlign;

    //GET GUID
    // remove everything up to (
    QString internalDeviceName = device.mid(device.lastIndexOf("(")+1);
    // remove )
    internalDeviceName = internalDeviceName.left(internalDeviceName.length()-1);

    wchar_t *internalDeviceNameW= new wchar_t[internalDeviceName.length()+1];
    internalDeviceNameW[internalDeviceName.toWCharArray(internalDeviceNameW)] = '\0';

    GUID deviceID;
    HRESULT err = CLSIDFromString(internalDeviceNameW, &deviceID);
    delete [] internalDeviceNameW;
    if (err != NOERROR) {
        kWarning() << "Couldn't parse: " << internalDeviceName << " assuming default";
        deviceID = ((type == SimonSound::Output) ? GUID_NULL : DSDEVID_DefaultVoiceCapture);
    }

    if (type == SimonSound::Output) {
        if(openOutputDevice(&deviceID, ppDS8,primaryBuffer))
            return setupNotifer((IUnknown**)primaryBuffer,notify);
    } else { // recording
        if(openInputDevice(&deviceID,ppDS8C,primaryBufferC))
            return setupNotifer((IUnknown**)primaryBufferC,notify);
    }
    return false;

}

bool DirectSoundBackend::check(SimonSound::SoundDeviceType type, const QString& device, int channels, int samplerate)
{
    Q_UNUSED(type);
    Q_UNUSED(device);
    Q_UNUSED(channels);
    Q_UNUSED(samplerate);
    return true; // FIXME? or does windows resample automatically?
}

QString DirectSoundBackend::getDefaultInputDevice()
{
    return i18n("Default audio device");
}

QString DirectSoundBackend::getDefaultOutputDevice()
{
    return i18n("Default audio device");
}

void DirectSoundBackend::errorRecoveryFailed()
{
    emit errorOccured(SimonSound::FatalError);
    stopRecording();
}

void DirectSoundBackend::freeAllResources()
{
    //destroying DS members

    if (m_primaryBuffer) {
        m_primaryBuffer->Release();
        m_primaryBuffer = 0;
    }
    if (m_handle) {
        m_handle->Release();
        m_handle = 0;
    }
    if (m_primaryBufferC) {
        m_primaryBufferC->Release();
        m_primaryBufferC = 0;
    }
    if (m_handleC) {
        m_handleC->Release();
        m_handleC = 0;
    }

}

// stop playback / recording
bool DirectSoundBackend::stop()
{
    kWarning() << "Stopping";
    if (state() != SimonSound::ActiveState)
        return true;

    Q_ASSERT(m_loop); //should be here if we are active

    m_loop->stop();
    m_loop->wait();
    m_loop->deleteLater();
    m_loop = 0;
    emit stateChanged(SimonSound::IdleState);
    return true;
}

bool DirectSoundBackend::closeSoundSystem()
{
    if (m_primaryBufferC)
        m_primaryBufferC->Stop();
    if (m_primaryBuffer)
        m_primaryBuffer->Stop();

    //Empty the buffer
    void* lpvAudio1 = 0;
    DWORD dwBytesAudio1 = 0;
    if (m_primaryBuffer) {
        HRESULT hr = m_primaryBuffer->Lock(0, 0, &lpvAudio1, &dwBytesAudio1, NULL, NULL, DSBLOCK_ENTIREBUFFER);
        if ( FAILED(hr) )  {
            kWarning() << "Couldn't lock buffer to clear"<<DXERR_TO_STRING(hr);
        } else {
            memset(lpvAudio1, 0, dwBytesAudio1);
            m_primaryBuffer->Unlock(lpvAudio1, dwBytesAudio1, 0, 0);

            //Move the current play position to begin
            m_primaryBuffer->SetCurrentPosition(0);
        }
    }

    //Reset Event
    ResetEvent(m_bufferEvents);

    freeAllResources();

    emit stateChanged(SimonSound::IdleState);
    return true;
}

///////////////////////////////////////
// Recording  /////////////////////////
///////////////////////////////////////

bool DirectSoundBackend::prepareRecording(const QString& device, int& channels, int& samplerate)
{
    if (m_handle || m_handleC || (m_loop && m_loop->isRunning())) {
        kWarning()<<"Sound system is running";
        emit errorOccured(SimonSound::BackendBusy);
        return false;
    }

    if (!openDevice(SimonSound::Input, device, channels, samplerate,
                    &m_handle, &m_primaryBuffer,
                    &m_handleC, &m_primaryBufferC,
                    &m_notify)) {
        kWarning()<<"failed to open input";
        return false;
    }

    m_loop = new DirectSoundCaptureLoop(this);
    emit stateChanged(SimonSound::PreparedState);
    kDebug() << "Prepared recording: " << m_handleC;

    return true;
}

bool DirectSoundBackend::startRecording(SoundBackendClient *client)
{
    m_client = client;

    if (!m_handleC || !m_loop) return false;

    //starting playback
    m_loop->start();
    emit stateChanged(SimonSound::ActiveState);
    kDebug() << "Started recording: " << m_handle;
    return false;
}

bool DirectSoundBackend::stopRecording()
{
    return stop();
}

///////////////////////////////////////
// Playback  //////////////////////////
///////////////////////////////////////

bool DirectSoundBackend::preparePlayback(const QString& device, int& channels, int& samplerate)
{
    if (m_handle || m_handleC || (m_loop && m_loop->isRunning())) {
        emit errorOccured(SimonSound::BackendBusy);
        return false;
    }

    if (!openDevice(SimonSound::Output, device, channels, samplerate,
                    &m_handle, &m_primaryBuffer,
                    &m_handleC, &m_primaryBufferC,&m_notify)) {
        kWarning() << "Failed to open device";
        return false;
    }

    m_loop = new DirectSoundPlaybackLoop(this);
    emit stateChanged(SimonSound::PreparedState);

    return true;
}

bool DirectSoundBackend::startPlayback(SoundBackendClient *client)
{
    m_client = client;

    if (!m_handle || !m_loop) return false;

    //starting playback
    m_loop->start();
    emit stateChanged(SimonSound::ActiveState);
    kDebug() << "Started playback: " << m_handle;
    return true;
}

bool DirectSoundBackend::stopPlayback()
{
    return stop();
}

DirectSoundBackend::~DirectSoundBackend()
{
    if (state() != SimonSound::IdleState)
        stop();
}
