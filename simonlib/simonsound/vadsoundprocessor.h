/*
 *   Copyright (C) 2010 Peter Grasch <grasch@simon-listens.org>
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


#ifndef SIMON_VADSOUNDPROCESSOR_H_BAC60251BE6A419EA1236280815A2AAD
#define SIMON_VADSOUNDPROCESSOR_H_BAC60251BE6A419EA1236280815A2AAD

#include "simonsound_export.h"
#include "simonsound.h"
#include "loudnessmetersoundprocessor.h"

#include <QObject>
#include <QByteArray>

class SIMONSOUND_EXPORT VADSoundProcessor : public QObject, public LoudnessMeterSoundProcessor {
	Q_OBJECT

signals:
	void listening();
	void complete();

private:
	SimonSound::DeviceConfiguration m_deviceConfiguration;

	qint64 lastLevel;
	qint64 lastTimeUnderLevel;
	qint64 lastTimeOverLevel;

	bool waitingForSampleToStart;
	bool waitingForSampleToFinish;

	bool currentlyRecordingSample;



	// when the system detects that the current input is over the
	// silence threshold it will fill up this buffer to a certain
	// extend before we can be sure that it wasn't just a "blib".
	//
	// When this happens we will begin to empty this buffer by sending
	// it to the server, adding live input
	QByteArray currentSample;



public:
	VADSoundProcessor(SimonSound::DeviceConfiguration deviceConfiguration);

	void process(QByteArray& data, qint64& currentTime);

	bool voiceActivity() { return currentlyRecordingSample; }

	void reset();
};

#endif





