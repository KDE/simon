//
// C++ Interface: recwidget
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RECWIDGET_H
#define RECWIDGET_H

#include <QWidget>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QString>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QIcon>
#include <QMessageBox>
#include <QFile>
#include <QChar>
#include "wavrecorder.h"
#include "wavplayer.h"

/**
	\class RecWidget
	\brief This class provides a nice Recording Widget to easily use within the application
	
	It draws the GUI and uses the WavRecorder/WavPlayer classes
	
	\author Peter Grasch
	\date 26.05.2007
*/
class RecWidget : public QGroupBox {
	Q_OBJECT

signals:
	void playing();
	void playBackStopped();
	void recording();
	void recordingStopped();
	void sampleDeleted();
	void progress(int);
	void recordingFinished();
	void playbackFinished();

private:
	QGroupBox *container;
	QPushButton *pbRecord;
	QPushButton *pbPlay;
	QPushButton *pbDelete;
	QSlider *hsProgress;
	QLabel *lbProgress;
	
	QString filename;
	WavRecorder *rec;
	WavPlayer *play;
	
	void setupLayout();
	void setupWidgets();
	void setupSignalsSlots();

public slots:
	void record();
	void stopRecording();
	
	void playback();
	void stopPlayback();
	void finishPlayback();
	
	void deleteSample();
	void displayPosition(int msecs);
	void displayRecordingProgress(int msecs);
	void displayPlaybackProgress(int msecs);
	
public:
    RecWidget(QString name, QString filename, QWidget *parent=0);
    
    bool hasRecordingReady() { return QFile::exists(this->filename); }

    ~RecWidget();

};

#endif
