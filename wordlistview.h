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

#define sImportDict 	32

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
#include "trainingview.h"
#include "ui_wordlist.h"
#include "inlinewidget.h"



class TrainingView;
class QPoint;

/**
 *	@class WordListView
 *	@brief Frontend to the Wordlist
 *
 *	@version 0.1
 *	@date 23.01.2006
 *	@author Peter Grasch
 */


class WordListView : public InlineWidget {
	Q_OBJECT
private:
	int shownDialogs;
	QPoint importDictPos;
	
	bool abortVocabInsertion;
	bool dirty; //!< Determines if we changed the model since the last save
	Ui::WordList ui;	//!< UI definition - made by uic from the QTDesigner .ui
	WordList trainingwordlist;  //!< Holds all the words that are scheduled for training
	WordListManager *wordListManager; //!< Concept class
	ImportDictView *importDictView; //!< Provides the Interface to import a dictionary
	TrainingView *trainView; //!< Single trainingview is used for the special training
	
	void initializeItems();
	void readVocab();
	void setDirty ( bool dirty );

signals:
	void showAddWordDialog();
	void wordlistLoaded();

private slots:
	void warnAboutTempWordList();
	
public slots:
	void abortInsertion() { abortVocabInsertion = true; }
	void reloadList();
	void askForRebuild();
	void suggestTraining();
	void markWordToTrain( Word word );
	void copyWordToTrain();
	void deleteTrainingWord();
	void deleteSelectedWord();
	void trainList();
	void importDict(WordList* list);
	void clearList();
	void toggleExtraWords();
	void filterListbyPattern(QString filter="");
	void clearSearchText();
	void insertVocab(WordList *vocab);
	void askToSave();
	void show();
	void compileModel();
	void hide();
	void showImportDictDialog();
    void hideTbEditModel();
    void setTbEditModelVisible();
    
    WordListManager* getManager(){return wordListManager;}

public:
	WordListView(TrainingView *trainView, QWidget *parent);
	WordListManager* getWordListManager() { return wordListManager; }
	~WordListView();

};

#endif
