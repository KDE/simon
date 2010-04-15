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


#ifndef SIMON_RECWIDGET_H_33F50DCCCC3D401FADDFBFD80B4E16F4
#define SIMON_RECWIDGET_H_33F50DCCCC3D401FADDFBFD80B4E16F4

#include "simonsound_export.h"
#include <QWidget>
class WavRecorderClient;
class WavPlayerClient;
class PostProcessing;

class KPushButton;
class QGroupBox;
class QProgressBar;
class QLabel;
class QPlainTextEdit;
class QFont;

/**
 * \class RecWidget
 * \brief This class provides a nice Recording Widget to easily use within the application
 * 
 * It draws the GUI and uses the WavRecorderClient/WavPlayerClient classes
 * 
 * \author Peter Grasch
 * \date 26.05.2007
 */
class SIMONSOUND_EXPORT RecWidget : public QWidget {
	Q_OBJECT

signals:
	void playing();
	void recording();
	void sampleDeleted();
	void progress(int);
	void recordingFinished();
	void playbackFinished();

private:
	QGroupBox *gbContainer;
//	QLabel *toRecord;
	QPlainTextEdit *tePrompt;
	KPushButton *pbRecord;
	KPushButton *pbPlay;
	KPushButton *pbDelete;
	QProgressBar *pbProgress;


	QWidget *wgWarning;
	

	QString filename;
	WavRecorderClient *rec;
	WavPlayerClient *play;
	PostProcessing *postProc;

	int recordingProgress;

	bool isRecording;
	bool isPlaying;
	
	void setupSignalsSlots();

private slots:
	void changePromptFont(const QFont& font);
	void displayError(const QString& error);

public slots:
	void record();
	void stopRecording();

	void setTitle(QString newTitle);

	void playback();
	void stopPlayback();
	void finishPlayback();
	
	bool deleteSample();
	void displayRecordingProgress(int msecs, float level);
	void displayClippingWarning();
	void displayPlaybackProgress(int msecs);
	void resizePromptLabel();
	
public:
    RecWidget(QString name, QString text, QString filename, QWidget *parent=0);
    bool hasRecordingReady();

    ~RecWidget();

};

#endif
