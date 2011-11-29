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

#ifndef SIMON_DIRECTSOUNDBACKEND_H_BAC60651BE6A419EA6156220815A2AAD
#define SIMON_DIRECTSOUNDBACKEND_H_BAC60651BE6A419EA6156220815A2AAD

#include <simonsound/simonsound.h>
#include <simonsound/soundbackend.h>
#include <QStringList>
#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>


class DirectSoundLoop;
class DirectSoundCaptureLoop;
class DirectSoundPlaybackLoop;

class DirectSoundBackend : public SoundBackend
{
	friend class DirectSoundPlaybackLoop;
	friend class DirectSoundCaptureLoop;

private:
	DirectSoundLoop *m_loop;

	HANDLE m_bufferEvents;
	WAVEFORMATEX    m_waveFormat;

	LPDIRECTSOUNDNOTIFY m_notify;
	DWORD m_notifySize;

	LPDIRECTSOUND8 m_handle;
	LPDIRECTSOUNDBUFFER8 m_primaryBuffer;

	LPDIRECTSOUNDCAPTURE8 m_handleC;
	LPDIRECTSOUNDCAPTUREBUFFER8 m_primaryBufferC;

	QStringList m_devices;

	int m_bufferSize;
	int m_blockAlign;
	int m_sampleRate;

	QStringList getDevices(SimonSound::SoundDeviceType type);

protected:
	void errorRecoveryFailed();
	void freeAllResources();
	bool stop();
	bool closeSoundSystem();

	bool setupNotifer( IUnknown **primaryBuffer,LPDIRECTSOUNDNOTIFY *notify);

	bool openOutputDevice(GUID *deviceID,LPDIRECTSOUND8* ppDS8, LPDIRECTSOUNDBUFFER8 *primaryBuffer);

	bool openInputDevice(GUID *deviceID,LPDIRECTSOUNDCAPTURE8* ppDS8C, LPDIRECTSOUNDCAPTUREBUFFER8 *primaryBufferC);

	bool openDevice(SimonSound::SoundDeviceType type, const QString& device, int channels, int samplerate, 
		LPDIRECTSOUND8* ppDS8, LPDIRECTSOUNDBUFFER8 *primaryBuffer,LPDIRECTSOUNDCAPTURE8* ppDS8C, LPDIRECTSOUNDCAPTUREBUFFER8 *primaryBufferC, 
		LPDIRECTSOUNDNOTIFY *notify);

public:
	static HANDLE s_deviceCallbackEvent;

	DirectSoundBackend();
	~DirectSoundBackend();

	QStringList getAvailableInputDevices();
	QStringList getAvailableOutputDevices();
	bool check(SimonSound::SoundDeviceType type, const QString& device, int channels, int samplerate);

	QString getDefaultInputDevice();
	QString getDefaultOutputDevice();

	bool prepareRecording(const QString& device, int& channels, int& samplerate);
	bool startRecording(SoundBackendClient *client);
	bool stopRecording();

	bool preparePlayback(const QString& device, int& channels, int& samplerate);
	bool startPlayback(SoundBackendClient *client);
	bool stopPlayback();

	int bufferSize();

	void deviceFound(const QString& name);
};

#endif

