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
 */

#ifndef RECOGNITIONCONTROL_H
#define RECOGNITIONCONTROL_H

#include <QObject>


class RecognitionControl : public QObject
{
	Q_OBJECT

	signals:
		void recognitionReady();
		void recognitionError(const QString& error);
		void recognitionWarning(const QString& warning);
		void recognitionStarted();
		void recognitionStopped();
		void recognitionTemporarilyUnavailable(const QString& reason);

	public:
		RecognitionControl(QObject *parent=0);

		virtual bool initializeRecognition()=0;
			       
		~RecognitionControl();
		
};

#endif
