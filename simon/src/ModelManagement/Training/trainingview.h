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

#include "inlinewidget.h"
#include "ui_trainmain.h"
#include "../WordList/word.h"

class WordListView;
class ImportTrainingDirectory;
class TrainingManager;
class RecWidget;
class ImportTrainingTexts;

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
	void displayMe();

private:
	static TrainingView *instance;
	Ui::TrainMain ui;	//!< UI definition - made by uic from the QTDesigner .ui
	TrainingManager *trainMgr;
	ImportTrainingTexts *import;
	int currentPage; //!< when we train a text this will hold the page we are on
	int oldId; //!< where did we come from?

	RecWidget *recorder;
    int recordedPages; //!< this counter will hold the recorded pages; it will change, when pages are recorded or deleted
	
	QString makeTextProgress(int msecs);

protected:
	TrainingView(QWidget *parent=0);

public slots:
	void finish();
	void importTexts();
	void trainSelected();
	
	void nextPage();
	void prevPage();
	void loadList();
	void cancelReading();
    void cancelTraining();
	void resetRecorder();
	void backToMain();
	void deleteSelected();
	void fetchPage(int page);
	void trainWords(WordList words);
	void startTraining();
	void importDirectory();
	TrainingManager* getManager() { return trainMgr; }

private slots:
    void increaseRecordedPages();
    void decreaseRecordedPages();

	void adaptNavigationButtons();

public:
	static TrainingView* getInstance(){
		if (!instance) instance = new TrainingView();
		return instance;
	}
	void exec();
	
	~TrainingView();
	
	void setSettingsHidden();
	void setSettingsVisible();
	void cleanUpTrainingSamples();

};

#endif
