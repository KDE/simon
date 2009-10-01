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


#ifndef SIMON_WORDLISTVIEWPRIVATE_H_0DC39320EB754A838727BB423019F0F5
#define SIMON_WORDLISTVIEWPRIVATE_H_0DC39320EB754A838727BB423019F0F5


#include "ui_wordlist.h"

#include <speechmodelbase/word.h>
#include <speechmodelmanagement/wordlisttype.h>


class QPoint;
class ImportDictView;
class WordListManager;

/**
 *	@class WordListViewPrivate
 *	@brief Frontend to the Wordlist
 *
 *	@version 0.1
 *	@date 23.01.2006
 *	@author Peter Grasch
 */


class WordListViewPrivate : public QWidget {
	Q_OBJECT
private:
	bool abortVocabInsertion;
	Ui::WordList ui;	//!< UI definition - made by uic from the QTDesigner .ui
	WordList trainingwordlist;  //!< Holds all the words that are scheduled for training
	WordListManager *wordListManager; //!< Concept class

	void setDirty ( bool dirty );


public slots:
	void abortInsertion() { abortVocabInsertion = true; }
	void reloadShadowList();
	void markWordToTrain( Word word );
	void copyWordToTrain();
	void deleteTrainingWord();
	void deleteSelectedWord();
	void trainList();
	void importDict(WordList* list, WordListTarget::WordListType type);
	void filterListbyPattern(QString filter="");
	void showImportDictDialog();

	WordListManager* getManager(){return wordListManager;}

public:
	WordListViewPrivate(QWidget *parent);
	
	~WordListViewPrivate();

};

#endif
