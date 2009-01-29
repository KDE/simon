/*
 *   Copyright (C) 2008 Peter Grasch <grasch@simon-listens.org>
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
 *
 *   Powered By:
 *
 *   Large Vocabulary Continuous Speech Recognition Engine Julius
 *   Copyright (c) 1997-2000 Information-technology Promotion Agency, Japan
 *   Copyright (c) 1991-2008 Kawahara Lab., Kyoto University
 *   Copyright (c) 2000-2005 Shikano Lab., Nara Institute of Science and Technology
 *   Copyright (c) 2005-2008 Julius project team, Nagoya Institute of Technology
 *
 */


#ifndef ADINSTREAMER_H
#define ADINSTREAMER_H

#include <QThread>
#include <QHostAddress>
#include <adinstreamer/adinstreamer_export.h>

#ifdef TRUE
#undef TRUE
#endif
#ifdef FALSE
#undef FALSE
#endif

extern "C" {
#include <julius/julius.h>
#include <julius/jconf.h>
#include <signal.h>
}
/**
 * \class AdinStreamer
 * \author Peter Grasch
 * \date 23.11.2008
 * \version 0.1
 * \brief Streams an adin stream to simond
 */
class ADINSTREAMER_EXPORT AdinStreamer : public QThread
{
Q_OBJECT

signals:
	void connected();
	void audioDeviceError();
	void requestingPause();
	void started();
	void stopped();
	
private:
	Recog *recog;
	bool shouldReStart;
	static AdinStreamer* instance;
	QHostAddress address;
	qint32 port;
        AdinStreamer(QObject* parent);

private slots:
	void reportSoundDeviceError();

public:
	static AdinStreamer* getInstance(QObject *parent=0)
	{
		if (!instance) instance = new AdinStreamer(parent);
		return instance;
	}
	static bool hasInstance()
	{
		return instance != NULL;
	}
	void run();
	void stop();
	void init(const QHostAddress& address, qint32 port, qint32 sampleRate);
	void stopSoundStream();
	void restartSoundStream();
        ~AdinStreamer();

};

#endif
