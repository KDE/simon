/*
*   Copyright (C) 2011 Patrick von Reth <patrick.vonreth@gmail.com>
*   Copyright (C) 2011 Peter Grasch <grasch@simon-listens.org>
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

#include <unistd.h>
#include <simonsound/soundbackendclient.h>
#include <QThread>
#include <KDebug>
#include <KLocalizedString>
#include "directsoundbackend.h"

#define SAFE_RELEASE(x) if (x) { x->Release(); x = 0; }
#define DXERR_TO_STRING(x) QString::fromWCharArray(DXGetErrorDescription(x))<<QString::fromWCharArray(DXGetErrorString(x))

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

		HRESULT hr;
		void* capture1 =NULL;
		void* capture2=NULL;
		DWORD captureLength1, captureLength2;
		UINT dataWritten = 0;
		DWORD dwMyReadCursor = 0;  
		DWORD dwReadPos;
		DWORD dwReadPosOld = 0;
		DWORD lockSize;
		qint64 readCount;
		int bufferSize = 1024;

		if(FAILED(hr = m_parent->m_primaryBufferC->Start( DSCBSTART_LOOPING ) )){
			kWarning()<<"Failed to start recording"<<DXERR_TO_STRING(hr);
			shouldRun = false;
		}


		while(shouldRun && !FAILED(m_parent->m_primaryBufferC->GetCurrentPosition(NULL,&dwReadPos))){
			if(dwReadPosOld == dwReadPos){
				//TODO:could cause lags?
				Sleep(100);
				continue;
			}
			dwReadPosOld = dwReadPos;
			lockSize = dwReadPos - dwMyReadCursor;
			if (lockSize <= 0)
				lockSize += bufferSize;
 
			lockSize -= (lockSize % bufferSize);  


			if (FAILED(hr = m_parent->m_primaryBufferC->Lock(dwMyReadCursor, lockSize,&capture1, &captureLength1, &capture2, &captureLength2, NULL))){
				kWarning()<<"Capture lock failure"<<DXERR_TO_STRING(hr);
				shouldRun = false;
				continue;
			} 



			//Copy AudioBuffer to DirectSoundBuffer
			dataWritten  = m_parent->m_client->writeData((char*)capture1, captureLength1);
			readCount = captureLength1;

			if (capture2 != NULL){
				dataWritten  += m_parent->m_client->writeData((char*)capture2, captureLength2);
				readCount += captureLength2;
			}
			
			if(dataWritten == 0 || dataWritten != readCount)
				kWarning()<<"Writing captured data failed";
			
			m_parent->m_primaryBufferC->Unlock(capture1,captureLength1,capture2,captureLength2);  
			dwMyReadCursor += readCount;
			dwMyReadCursor %= bufferSize;


		}
		kWarning()<<"Record loop ended";
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

	void run()
	{
		shouldRun = true;

		//init for first run
		////////////////////
		qint64 written = m_parent->m_client->readData((char*) m_parent->m_audioBuffer, m_parent->m_bufferSize);
		kWarning() << "First written: " << written;
		if (written < 0) {
			kWarning() << "exiting prematurely";
			shouldRun = false;
			return;
		}

		//Write the audio data to DirectSoundBuffer
		void* lpvAudio1 = 0;
		void* lpvAudio2 = 0;
		DWORD dwBytesAudio1 = 0;
		DWORD dwBytesAudio2 = 0; 

		//Lock DirectSoundBuffer
		//Locking with 0,  written, 0, 0, 0, 0, 0
		HRESULT hr = m_parent->m_secondaryBuffer->Lock(0, written, &lpvAudio1, &dwBytesAudio1, &lpvAudio2, &dwBytesAudio2, 0);
		if (FAILED(hr)) {
			kWarning() << "Lock DirectSoundBuffer Failed!";
			m_parent->errorRecoveryFailed();
			return;
		}

		//Init lpvAudio1
		if (lpvAudio1) {      
			memset(lpvAudio1, 0, dwBytesAudio1);      
		}

		//Init lpvAudio2
		if (lpvAudio2) {      
			memset(lpvAudio2, 0, dwBytesAudio2);      

			memcpy(lpvAudio1, m_parent->m_audioBuffer, dwBytesAudio1);
			memcpy(lpvAudio2, m_parent->m_audioBuffer + dwBytesAudio1, dwBytesAudio2);
		} else {
			//Copy Audio Buffer to DirectSoundBuffer
			memcpy(lpvAudio1, m_parent->m_audioBuffer, written);
		}

		//Unlock DirectSoundBuffer
		m_parent->m_secondaryBuffer->Unlock(lpvAudio1, dwBytesAudio1, lpvAudio2, dwBytesAudio2);

		//Begin playback
		m_parent->m_secondaryBuffer->Play(0, 0, DSBPLAY_LOOPING);

		//Playback Loop
		///////////////
		while (shouldRun) {
			Sleep(300);

			kWarning() << "In loop";
			lpvAudio1 = 0;
			lpvAudio2 = 0;
			dwBytesAudio1 = 0;
			dwBytesAudio2 = 0;

			HRESULT hr = WaitForMultipleObjects(2, m_parent->m_bufferEvents, FALSE, 0);
			if(hr == WAIT_OBJECT_0) {
				//Lock DirectSoundBuffer Second Part
				HRESULT hr = m_parent->m_secondaryBuffer->Lock(m_parent->m_bufferSize, m_parent->m_bufferSize, &lpvAudio1, &dwBytesAudio1, &lpvAudio2, &dwBytesAudio2, 0);
				if ( FAILED(hr) ) {
					kWarning() << "Lock DirectSoundBuffer Failed!";
					break;
				}   
			} else if (hr == WAIT_OBJECT_0 + 1) {   
				//Lock DirectSoundBuffer First Part
				HRESULT hr = m_parent->m_secondaryBuffer->Lock(0, m_parent->m_bufferSize, &lpvAudio1, &dwBytesAudio1, &lpvAudio2, &dwBytesAudio2, 0);
				if (FAILED(hr)) {
					kWarning() << "Lock DirectSoundBuffer Failed!";
					break;
				}   
			} else {
				kWarning() << "Not yet: " << hr;
				continue;
			}

			//Get 1 Second Audio Buffer 
			written = m_parent->m_client->readData((char*) m_parent->m_audioBuffer, m_parent->m_bufferSize);
			kWarning() << "Written: " << written;
			if (written < 0) {
				kWarning() << "Reached the end of the data";
				shouldRun = false;
				break;
			}

			//If near the end of the audio data set undefined block of buffer to 0
			if (written < m_parent->m_bufferSize)
				memset(m_parent->m_audioBuffer+written, 0, m_parent->m_bufferSize - written);       

			//Copy AudioBuffer to DirectSoundBuffer
			if (!lpvAudio2) {
				memcpy(lpvAudio1, m_parent->m_audioBuffer, dwBytesAudio1);
			} else {
				memcpy(lpvAudio1, m_parent->m_audioBuffer, dwBytesAudio1);
				memcpy(lpvAudio2, m_parent->m_audioBuffer + dwBytesAudio1, dwBytesAudio2);
			}

			//Unlock DirectSoundBuffer
			m_parent->m_secondaryBuffer->Unlock(lpvAudio1, dwBytesAudio1, lpvAudio2, dwBytesAudio2);
		}
		kWarning() << "Exiting run loop";
		shouldRun = false;
		m_parent->closeSoundSystem();
	}
};





DirectSoundBackend::DirectSoundBackend() : 
m_loop(0),
	m_audioBuffer(0),
	m_notify(0),
	m_handle(0),
	m_primaryBuffer(0),
	m_secondaryBuffer(0),
	m_handleC(0),
	m_primaryBufferC(0),
	m_bufferSize(1024)
{
	ZeroMemory(&m_waveFormat, sizeof(m_waveFormat));
	m_bufferEvents[0] = CreateEvent(0, FALSE, FALSE, L"Direct_Sound_Buffer_Notify_0");
	m_bufferEvents[1] = CreateEvent(0, FALSE, FALSE, L"Direct_Sound_Buffer_Notify_1");
}

int DirectSoundBackend::bufferSize()
{
	return m_bufferSize;
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
	if (!pGUID) {
		//primary sound driver
		return TRUE;
	}
	OLECHAR lplpsz[40]={0};
	StringFromGUID2(*pGUID, lplpsz, 40);
	QString uid = QString::fromWCharArray(lplpsz);

	QString deviceName(QString::fromWCharArray(pcName));
	deviceName += QString(" (%1)").arg(uid);

	kDebug() << "Found device: " << deviceName;
	((DirectSoundBackend*) pContext)->deviceFound(deviceName);
	return TRUE;
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


	Sleep(1000); // yes, this is the ugliest hack in the universe // have seen more ugly thingss
	return m_devices;
}

bool DirectSoundBackend::openOutputDevice(GUID *deviceID,LPDIRECTSOUND8* ppDS8, LPDIRECTSOUNDBUFFER *primaryBuffer,LPDIRECTSOUNDBUFFER *secondaryBuffer,LPDIRECTSOUNDNOTIFY *notify){
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

	kWarning() << "Setting cooperation level";
	(*ppDS8)->SetCooperativeLevel(hWnd, DSSCL_PRIORITY);



	memset( &BufferDesc, 0,sizeof(BufferDesc) );
	BufferDesc.dwSize     = sizeof(DSBUFFERDESC);
	BufferDesc.dwFlags      = DSBCAPS_PRIMARYBUFFER;
	if(FAILED(hr = (*ppDS8)->CreateSoundBuffer(&BufferDesc,primaryBuffer, 0))) {
		kWarning() << "Failed to create primary sound buffer for playback"<<DXERR_TO_STRING(hr);
		(*ppDS8)->Release();
		*ppDS8 = 0;
		*primaryBuffer = 0;
		return false;
	}

	kWarning() << "Creating sound buffer";

	// DSBUFFERDESC
	BufferDesc.dwFlags      =	DSBCAPS_CTRLPOSITIONNOTIFY |
								DSBCAPS_CTRLFREQUENCY |
								DSBCAPS_GLOBALFOCUS;
	BufferDesc.dwBufferBytes  = m_waveFormat.nAvgBytesPerSec * 2; // 2 seconds of sound
	BufferDesc.lpwfxFormat    = &m_waveFormat;


	if(FAILED((*ppDS8)->CreateSoundBuffer(&BufferDesc, secondaryBuffer, 0))) {
		kWarning() << "Couldn't create sound buffer.";
		(*ppDS8)->Release();
		*ppDS8 = 0;
		(*primaryBuffer)->Release();
		*primaryBuffer = 0;
		emit errorOccured(SimonSound::OpenError);
		return false;
	}


	kWarning() << "Query interface";
	if(FAILED(hr = (*secondaryBuffer)->QueryInterface(IID_IDirectSoundNotify, (void**)(notify)))){

		kWarning() << "Query interface failed"<<DXERR_TO_STRING(hr);
		freeAllResources();
		return false;
	}



	kWarning() << "Notify positions";
	//calculate notify positions
	DSBPOSITIONNOTIFY pPosNotify[2];
	pPosNotify[0].dwOffset = m_waveFormat.nAvgBytesPerSec/2 - 1;
	pPosNotify[1].dwOffset = 3*m_waveFormat.nAvgBytesPerSec/2 - 1;   
	pPosNotify[0].hEventNotify = m_bufferEvents[0];
	pPosNotify[1].hEventNotify = m_bufferEvents[1];  

	kWarning() << "Calling SetNotificationPositions on notify";
	if ( FAILED(hr = (*notify)->SetNotificationPositions(2, pPosNotify)) ) {
		kWarning() << "Set NotificationPosition Failed!"<<DX_SHARED_DEFINES(hr);
		freeAllResources();
		return false;
	} 



	kWarning() << "Allocating buffer";
	//New audio buffer
	//if (m_audioBuffer != 0)
	delete[] m_audioBuffer;
	m_audioBuffer = new BYTE[m_waveFormat.nAvgBytesPerSec];
	m_bufferSize = m_waveFormat.nAvgBytesPerSec;

	//Init Audio Buffer
	memset(m_audioBuffer, 0, m_waveFormat.nAvgBytesPerSec);



	kWarning() << "Opened device";
	return true;
}
bool DirectSoundBackend::openInputDevice(GUID *deviceID,LPDIRECTSOUNDCAPTURE8* ppDS8C, LPDIRECTSOUNDCAPTUREBUFFER *primaryBufferC){
	HRESULT hr;

	if(FAILED( hr = DirectSoundCaptureCreate8(deviceID, ppDS8C, 0))) {
		kWarning() << "Failed to open capture device"<<DXERR_TO_STRING(hr);
		*ppDS8C = 0;
		emit errorOccured(SimonSound::OpenError);
		return false;
	}
	DSCBUFFERDESC CaptureBufferDesc;

	memset( &CaptureBufferDesc, 0,sizeof(CaptureBufferDesc) ); 
	CaptureBufferDesc.dwSize     = sizeof(DSCBUFFERDESC);
	CaptureBufferDesc.dwBufferBytes  = m_waveFormat.nAvgBytesPerSec * 2; // 2 seconds of sound
	CaptureBufferDesc.lpwfxFormat    = &m_waveFormat;

	if(FAILED(hr = (*ppDS8C)->CreateCaptureBuffer(&CaptureBufferDesc,primaryBufferC, NULL))) {
		kWarning() << "Failed to create primary recording buffer"<<DXERR_TO_STRING(hr);
		(*ppDS8C)->Release();
		*ppDS8C = 0;
		*primaryBufferC = 0;
		return false;
	}
	kWarning() << "Opened device";
	return true;
}

bool DirectSoundBackend::openDevice(SimonSound::SoundDeviceType type, const QString& device, int channels, int samplerate, 
	LPDIRECTSOUND8* ppDS8, LPDIRECTSOUNDBUFFER *primaryBuffer, LPDIRECTSOUNDBUFFER *secondaryBuffer, 
	LPDIRECTSOUNDCAPTURE8* ppDS8C, LPDIRECTSOUNDCAPTUREBUFFER *primaryBufferC, LPDIRECTSOUNDNOTIFY *notify)
{
	HRESULT hr;


	//init sound
	kWarning() << "Creating audio format";
	// audioformat
	m_waveFormat.wFormatTag   = WAVE_FORMAT_PCM;
	m_waveFormat.nChannels    = channels;
	m_waveFormat.nSamplesPerSec = samplerate;
	m_waveFormat.wBitsPerSample = 16; //S16_LE
	m_waveFormat.nBlockAlign    = m_waveFormat.nChannels * (m_waveFormat.wBitsPerSample >> 3);
	m_waveFormat.nAvgBytesPerSec  = m_waveFormat.nSamplesPerSec * m_waveFormat.nBlockAlign;
	m_waveFormat.cbSize     = 0;

	m_sampleRate = samplerate;
	m_blockAlign = m_waveFormat.nBlockAlign;

	//GET GUID
	// remove everything up to (
	QString internalDeviceName = device.mid(device.lastIndexOf("(")+1);
	// remove )
	internalDeviceName = internalDeviceName.left(internalDeviceName.length()-1);

	kDebug() << "Opening device: " << internalDeviceName; // contains the GUID or "" for default
	wchar_t *internalDeviceNameW= new wchar_t[internalDeviceName.length()+1];
	internalDeviceNameW[internalDeviceName.toWCharArray(internalDeviceNameW)] = '\0';

	GUID deviceID;
	HRESULT err = CLSIDFromString(internalDeviceNameW, &deviceID);
	delete [] internalDeviceNameW;
	if (err != NOERROR) {
		kWarning() << "Couldn't parse: " << internalDeviceName << " assuming default";
		deviceID = ((type == SimonSound::Output) ? GUID_NULL : DSDEVID_DefaultVoiceCapture);
	}



	kWarning() << "Opened the device";
	if (type == SimonSound::Output) {
		return openOutputDevice(&deviceID, ppDS8,primaryBuffer,secondaryBuffer,notify);
	} else { // recording
		return openInputDevice(&deviceID,ppDS8C,primaryBufferC);
	}


}

bool DirectSoundBackend::check(SimonSound::SoundDeviceType type, const QString& device, int channels, int samplerate)
{
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
	if (m_secondaryBuffer) {
		m_secondaryBuffer->Release();
		m_secondaryBuffer = 0;
	}
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

	delete[] m_audioBuffer;
	m_audioBuffer = 0;    
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
	return true;
}

bool DirectSoundBackend::closeSoundSystem()
{
	if (m_secondaryBuffer)
		m_secondaryBuffer->Stop();

	//Empty the buffer
	LPVOID lpvAudio1 = 0;
	DWORD dwBytesAudio1 = 0;
	if (m_secondaryBuffer) {
		HRESULT hr = m_secondaryBuffer->Lock(0, 0, &lpvAudio1, &dwBytesAudio1, 0, 0, DSBLOCK_ENTIREBUFFER);
		if ( FAILED(hr) )  {
			kWarning() << "Couldn't lock buffer to clear";
		} else {
			memset(lpvAudio1, 0, dwBytesAudio1);
			m_secondaryBuffer->Unlock(lpvAudio1, dwBytesAudio1, 0, 0);

			//Move the current play position to begin
			m_secondaryBuffer->SetCurrentPosition(0);	
		}
	}

	//Reset Event
	ResetEvent(m_bufferEvents[0]);
	ResetEvent(m_bufferEvents[1]);

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
		&m_handle, &m_primaryBuffer, &m_secondaryBuffer, 
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
		&m_handle, &m_primaryBuffer, &m_secondaryBuffer, 
		&m_handleC, &m_primaryBufferC,&m_notify)) {
			kWarning() << "Failed to open device";
			return false;
	}

	m_loop = new DirectSoundPlaybackLoop(this);
	emit stateChanged(SimonSound::PreparedState);
	kWarning() << "Prepared playback: " << m_handle;

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
