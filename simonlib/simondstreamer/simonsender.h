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


#ifndef SIMON_SIMONSENDER_H_0AC05651BE6A419EA6256220815A2AAD
#define SIMON_SIMONSENDER_H_0AC05651BE6A419EA6256220815A2AAD

#include <QtGlobal>

class QByteArray;

class SimonSender {

public:
	SimonSender() {}
	virtual ~SimonSender() {}
	virtual void startSampleToRecognize(qint8 channels, qint32 sampleRate)=0;
	virtual void sendSampleToRecognize(const QByteArray& data)=0;
	virtual void recognizeSample()=0;

};

#endif


