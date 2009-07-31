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


#ifndef RECWIDGET_H
#define RECWIDGET_H

#include "simonsound_export.h"
#include <QWidget>
class WavRecorder;
class WavPlayer;
class PostProcessing;

class KPushButton;
class QGroupBox;
class QProgressBar;
class QLabel;
class KTextEdit;
class QFont;

/**
 * \class RecWidget
 * \brief This class provides a nice Recording Widget to easily use within the application
 * 
 * It draws the GUI and uses the WavRecorder/WavPlayer classes
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
	KTextEdit *tePrompt;
	KPushButton *pbRecord;
	KPushButton *pbPlay;
	KPushButton *pbDelete;
	QProgressBar *pbProgress;
	

	QString filename;
	WavRecorder *rec;
	WavPlayer *play;
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
	void displayPlaybackProgress(int msecs);
	void resizePromptLabel();
	
public:
    RecWidget(QString name, QString text, QString filename, QWidget *parent=0);
    bool hasRecordingReady();

    ~RecWidget();

};

#endif
