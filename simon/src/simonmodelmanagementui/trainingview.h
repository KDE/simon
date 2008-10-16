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


#ifndef TRAININGVIEW_H
#define TRAININGVIEW_H

#include "inlinewidget.h"
#include "ui_trainmain.h"
#include "simonmodelmanagementui_export.h"

#include <speechmodelbase/word.h>

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
class SIMONMODELMANAGEMENTUI_EXPORT TrainingView : public InlineWidget {
	Q_OBJECT

signals:
	void trainingCompleted();

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
	
	~TrainingView();
	
	void setSettingsHidden();
	void setSettingsVisible();
	void cleanUpTrainingSamples();

};

#endif
