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


#ifndef SIMON_WAVFILEWIDGET_H_33F50DCA0C3D401FADDFBFD80B4E16F4
#define SIMON_WAVFILEWIDGET_H_33F50DCA0C3D401FADDFBFD80B4E16F4

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


namespace Ui
{
	class WavFileWidgetUi;
}

/**
 * \class WavFileWidget
 * \brief This class provides a nice Recording Widget to easily use within the application
 * 
 * It draws the GUI and uses the WavRecorderClient/WavPlayerClient classes
 * 
 * \author Peter Grasch
 * \date 26.05.2007
 */
class SIMONSOUND_EXPORT WavFileWidget : public QWidget {
	Q_OBJECT

signals:
	void playing();
	void recording();
	void sampleDeleted();
	void progress(int);
	void recordingFinished();
	void recordingError();

	void playbackFinished();
	void error(const QString& error);

private:
	Ui::WavFileWidgetUi *ui;

	QString m_device;
	QString m_filename;
	WavRecorderClient *rec;
	WavPlayerClient *play;
	PostProcessing *postProc;

	int recordingProgress;

	bool isRecording;
	bool isPlaying;
	
	void setupSignalsSlots();


public slots:
	void record();
	void stopRecording();

	void playback();
	void stopPlayback();
	void finishPlayback();
	void adjustToFinishedPlayback();
	
	bool deleteSample();
	void displayRecordingProgress(int msecs, float level);
	void displayClippingWarning();
	void displayPlaybackProgress(int msecs);
	
public:
    WavFileWidget(const QString& device, int channels, int sampleRate, const QString& filename, QWidget *parent=0);
    bool hasRecordingReady();

    bool getIsRecording() { return isRecording; }
    bool getIsPlaying() { return isPlaying; }
    QString getFileName() { return m_filename; }

    ~WavFileWidget();

};

#endif

