/***************************************************************************
 *   Copyright (C) 2006 by Peter Grasch   *
 *   bedahr@gmx.net   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


/**
 *	@class AddWordView
 *	@brief The Dialog to add a new word to the Model
 *
 *	This dialog is using a step-by-step guide through the procedure to
 *	add a new word to the Language Model.
 *
 *	@version 0.1
 *	@date 08.01.2006
 *	@author Peter Grasch
 *	@todo Implementing functions
*/
 
#ifndef ADDWORDVIEW_H
#define ADDWORDVIEW_H

#include <QMainWindow>
#include <QMessageBox>
#include "simoninfo.h"
#include "wavrecorder.h"
#include "wavplayer.h"
#include <QFile>
#include "ui_addword.h"


class AddWordView : public QDialog
{
	Q_OBJECT

	private:
		WavRecorder *rec;
		WavPlayer *play;
		QString makeTextProgress(int msecs);
		void setRecStatus(QSlider *prog, QLabel *textprog, int msecs);
		void setPlayStatus(QSlider *prog, QLabel *textprog, int msecs);
		
		Ui::AddWord ui; //!< The ui definition created with uic
	public slots:
		void nextStep();
		void prevStep();
		void startRecording(QString filename);
		void startPlayback(QString filename);
		void stopRecording();
		void stopRecording1();
		void stopRecording2();
		void stopPlayback();
		void recSample1();
		void recSample2();
		void playSample1();
		void playSample2();
		void deleteSample1();
		void deleteSample2();
		void recSample1Status(int msecs);
		void recSample2Status(int msecs);
		void playSample1Status(int msecs);
		void playSample2Status(int msecs);
		void finishPlayback1();
		void finishPlayback2();
		void finish();
		
	public:
		AddWordView(QWidget *parent=0, Qt::WFlags flags=0);
		~AddWordView();
};

#endif
