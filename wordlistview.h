//
// C++ Interface: wordlistview
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef WORDLISTVIEW_H
#define WORDLISTVIEW_H
#include <QDialog>
#include <QTableWidget>
#include <QHeaderView>
#include <QString>
#include "word.h"
#include "wordlistmanager.h"
#include "dragtablewidget.h"
#include "droplistwidget.h"
#include <QMessageBox>
#include <QTimer>
#include "importdictview.h"
#include "addwordview.h"
#include "trainingview.h"
#include "ui_wordlist.h"

/**
 *	@class WordListView
 *	@brief Frontend to the Wordlist
 *
 *	@version 0.1
 *	@date 23.01.2006
 *	@author Peter Grasch
 *	@todo Implementing
 */


class WordListView : public QDialog {
	Q_OBJECT
private:
	bool dirty; //!< Determines if we changed the model since the last save
	Ui::WordList ui;	//!< UI definition - made by uic from the QTDesigner .ui
	WordList trainingwordlist;  //!< Holds all the words that are scheduled for training
	DragTableWidget *twVocab; //!< A QTableWidget that holds all vocabulary
	DropListWidget *lwTrainingWords;  //!< QListWidget that displays the words scheduled for training
	WordListManager *wordListManager; //!< Concept class
	ImportDictView *importDictView; //!< Provides the Interface to import a dictionary
	
	void initializeItems();
	bool showAddWordDialog();
	void readVocab();
	void setDirty ( bool dirty );
	
public slots:
	void suggestTraining();
	void markWordToTrain( Word *word );
	void copyWordToTrain();
	void deleteTrainingWord();
	void trainList();
	void importDict();
	void addWord();
	void filterListbyPattern(QString filter);
	void clearSearchText();
	void switchToGenericTraining();
	void insertVocab(WordList *vocab);
	void askToSave();

public:
	WordListView(QWidget *parent);

	~WordListView();

};

#endif
