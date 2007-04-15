//
// C++ Interface: trainingview
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef TRAININGVIEW_H
#define TRAININGVIEW_H

#include <QDialog>
#include <QWidget>
#include <QString>
#include <QTableWidget>
#include <QHeaderView>
#include "trainingtext.h"
#include "trainingmanager.h"
#include "wordlistview.h"
#include "trainingtext.h"
#include "ui_trainmain.h"

/**
 *	@class TrainingView
 *	@brief Provides the UI for the Training process
 *
 *	@version 0.1
 *	@date 23.01.2006
 *	@author Peter Grasch
 *	@todo Implementing
 */
class TrainingView : public QDialog{
	Q_OBJECT
private:
	Ui::TrainMain ui;	//!< UI definition - made by uic from the QTDesigner .ui
	TrainingManager *trainMgr;
	int currentPage; //!< when we train a text this will hold the page we are on
	QList<int> *lengthList; //!< Stores the length of the pages in msecs
	WavRecorder *rec;
	WavPlayer *play;
	QString makeTextProgress(int msecs);
public slots:
	void switchToWordList();
	void finish();
	void trainSelected();
	void startPlayback(QString filename);
	void startRecording(QString filename);
	void stopRecording();
	void stopPlayback();
	void setRecStatus(int msecs);
	void setPlayStatus(int msecs);
	void nextPage();
	void prevPage();
	void playSample();
	void deleteSample();
	void recordPage();
	void makeRecControlsReflectStatus();
	void loadList();
	void cancelReading();
	void cancelTraining();
	void fetchPage(int page);
	void trainWords(WordList *words);
	void startTraining();
public:
	
    TrainingView(QWidget *parent = 0, WordList *trainWords=NULL);
	
    ~TrainingView();

};

#endif
