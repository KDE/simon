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
#include "importtrainingtexts.h"
#include "recwidget.h"
#include "ui_trainmain.h"
#include "inlinewidget.h"


class WordListView;
class ImportTrainingDirectory;

/**
 *	@class TrainingView
 *	@brief Provides the UI for the Training process
 *
 *	@version 0.1
 *	@date 23.01.2006
 *	@author Peter Grasch
 */
class TrainingView : public InlineWidget {
	Q_OBJECT

signals:
	void trainingCompleted();

private:
	Ui::TrainMain ui;	//!< UI definition - made by uic from the QTDesigner .ui
	TrainingManager *trainMgr;
	int currentPage; //!< when we train a text this will hold the page we are on
	WordListView *wordlistView;
	int oldId; //!< where did we come from?
	
	RecWidget *recorder;
	
	QString makeTextProgress(int msecs);
public slots:
	void switchToWordList();
	void finish();
	void importTexts();
	void trainSelected();
	
	void nextPage();
	void prevPage();
	void loadList();
	void cancelReading();
	void cancelTraining();
	void resetRecorder();
	void deleteSelected();
	void fetchPage(int page);
	void trainWords(WordList *words);
	void startTraining();
	void importDirectory();
public:
	
    TrainingView(WordListView *wordlistView, QWidget *parent=0);
	
    ~TrainingView();

};

#endif
