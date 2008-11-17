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

#ifndef TRAININGVIEWPRIVATE_H
#define TRAININGVIEWPRIVATE_H

#include "ui_trainmain.h"

#include <QWidget>

#include <speechmodelbase/word.h>

class WordListView;
class ImportTrainingDirectory;
class TrainingManager;
class RecWidget;
class ImportTrainingTexts;
/**
 *	@class TrainingViewPrivate
 *	@brief Provides the UI for the Training process
 *
 *	@version 0.1
 *	@date 23.01.2006
 *	@author Peter Grasch
 */
class TrainingViewPrivate : public QWidget {
	Q_OBJECT

private:
	Ui::TrainMain ui;	//!< UI definition - made by uic from the QTDesigner .ui
	ImportTrainingTexts *import;
	int currentPage; //!< when we train a text this will hold the page we are on

	RecWidget *recorder;
	int recordedPages; //!< this counter will hold the recorded pages; it will change, when pages are recorded or deleted
	
	void resetRecorder();
	void cleanUpTrainingSamples();

private slots:
	void increaseRecordedPages();
	void decreaseRecordedPages();

	void adaptNavigationButtons();
	void backToMain();

	void startTraining();
	void fetchPage(int page);
	void nextPage();
	void prevPage();
	void cancelReading();
	void finish();


public slots:
	void loadList();

	void deleteSelected();

	void trainSelected();
	void trainWords(const WordList& words);

	void importTexts();
	void importDirectory();

public:
	TrainingViewPrivate(QWidget *parent=0);
	~TrainingViewPrivate();

};
#endif