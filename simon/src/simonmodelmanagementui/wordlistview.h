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


#ifndef WORDLISTVIEW_H
#define WORDLISTVIEW_H

#include "inlinewidget.h"
#include "simonmodelmanagementui_export.h"

#include <speechmodelbase/word.h>


class WordListViewPrivate;
class TrainingView;

/**
 *	@class WordListView
 *	@brief Frontend to the Wordlist
 *
 *	@version 0.1
 *	@date 23.01.2006
 *	@author Peter Grasch
 */class SIMONMODELMANAGEMENTUI_EXPORT WordListView : public InlineWidget {
	Q_OBJECT
signals:
	void wordlistLoaded();
private:
	WordListViewPrivate *d;



// public slots:
// 	void abortInsertion() { abortVocabInsertion = true; }
// 	void reloadShadowList();
// 	void suggestTraining();
// 	void markWordToTrain( Word word );
// 	void copyWordToTrain();
// 	void deleteTrainingWord();
// 	void deleteSelectedWord();
// 	void trainList();
// 	void importDict(WordList* list);
// 	void clearList();
// 	void filterListbyPattern(QString filter="");
// 	void insertVocab(WordList *vocab);
// 	void show();
// 	void hide();
// 	void showImportDictDialog();
// 
// 	void setSettingsVisible();
// 	void setSettingsHidden();
// 
// 	WordListManager* getManager(){return wordListManager;}

public slots:
	void setSettingsHidden();
	void setSettingsVisible();
	void filterListbyPattern();

public:
	WordListView(TrainingView *trainingView, QWidget *parent);
	
	~WordListView();

};

#endif
