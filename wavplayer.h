//
// C++ Interface: wavplayer
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef WAVPLAYER_H
#define WAVPLAYER_H

#include <QThread>
#include <QMessageBox>
#include <QTimer>
#include "soundcontrol.h"
#include "wav.h"

/**
	\class WavPlayer
	
	\brief This class is used to get the needed data out of existing wav files to play them
	
	\author Peter Grasch
	\version 0.1
	\todo Replace the dummy methods with implemented ones
*/
class WavPlayer : public QThread {
	Q_OBJECT
private:
	SoundControl *sound;
	QTimer *progressTimer;
	char* data;
	int length;
	int progress;
signals:
	void currentProgress(int);
public:
    WavPlayer(QWidget *parent=0);
    bool play(QString filename);
    void run();
    ~WavPlayer();
public slots:
	void increaseProgress();
	void exec();
	void stop();
	


};

#endif
