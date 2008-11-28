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


#include "wordlistviewprivate.h"
#include "deleteworddialog.h"
#include "ImportDict/importdictview.h"
#include "TrainSamples/trainingswizard.h"

#include <speechmodelmanagement/wordlistmanager.h>
#include <simonlogging/logger.h>
#include <speechmodelmanagement/wordlistmodel.h>

#include <QTableWidget>
#include <QHeaderView>
#include <QString>
#include <KMessageBox>
#include <KProgressDialog>
#include <QCoreApplication>
#include <KIcon>
#include <KColorScheme>

/**
 * @brief Constructor
 *
 * Creates a new WordlistView Objects, creates the Links
 * and initializes the list of vocabulary
 *
 * @author Peter Grasch
 */
WordListViewPrivate::WordListViewPrivate(QWidget *parent) : QWidget(parent)
{
	shownDialogs = 0;
	abortVocabInsertion = false;
	
	ui.setupUi(this);

	importDictView = new ImportDictView(this);
	connect(ui.pbAddToTraining, SIGNAL(clicked()), this, SLOT(copyWordToTrain()));
	connect(ui.pbDeleteTrainingWord, SIGNAL(clicked()), this, SLOT(deleteTrainingWord()));
	connect(ui.lwTrainingWords, SIGNAL(droppedText(QString)), this, SLOT(copyWordToTrain()));
	
	connect(ui.pbRemoveWord, SIGNAL(clicked()), this, SLOT(deleteSelectedWord()));
	connect(ui.leSearch, SIGNAL(returnPressed()), this, SLOT(filterListbyPattern()));
	connect(ui.leSearch, SIGNAL(clearButtonClicked()), this, SLOT(filterListbyPattern()));
	
// 	connect(ui.leSearch, SIGNAL(editingFinished()), this, SLOT(filterListbyPattern()));
	//we can't filter on editingFinished() LineEdit because that would cause simon to
	//run in the following infinite loop:
	//	1. We press return in the LineEdit which activates the returnPressed() signal which
	//	   in turn activates filterListbyPattern()
	//	2. filterListbyPattern() displays a progressdialog while filtering the wordlist
	//	   which of course gets the focus.
	//	3. This means we get a hit on editingFinished() which starts to filter the Wordlist again
	//	4. Which shows the progressdialog again
	//	5. Which, when shown will activate editingFinished() again, etc.
	// so we just use returnPressed() for now, which makes more sense anyways
	
	connect (ui.pbTrainList, SIGNAL(clicked()), this, SLOT(trainList()));
	connect(ui.pbImport, SIGNAL(clicked()), this, SLOT(showImportDictDialog()));
	connect(importDictView, SIGNAL(dictGenerated(WordList*)), this, SLOT(importDict(WordList*)));
	
	connect(ui.cbShowCompleteLexicon, SIGNAL(toggled(bool)), this, SLOT(filterListbyPattern()));

	this->wordListManager = WordListManager::getInstance();
	connect(this->wordListManager, SIGNAL(wordlistChanged()), this, SLOT(filterListbyPattern()));
	
	connect(this->wordListManager, SIGNAL(shadowListChanged()), this, SLOT(reloadShadowList()));
	this->filterListbyPattern();

	ui.pbImport->setIcon(KIcon("document-import"));
	ui.pbRemoveWord->setIcon(KIcon("edit-delete"));
	ui.pbAddToTraining->setIcon(KIcon("list-add"));
	ui.pbDeleteTrainingWord->setIcon(KIcon("list-remove"));
	ui.pbTrainList->setIcon(KIcon("go-next"));

	ui.tvVocab->setSelectionBehavior(QAbstractItemView::SelectRows);
}



/**
 * \brief Reloads the wordlist if the shadowlist is displayed
 * \author Peter Grasch
 * \note Respects filters (by calling filterListbyPattern())
 */
void WordListViewPrivate::reloadShadowList()
{
	//if the shadowlist is not even shown - why bother?
	if (!ui.cbShowCompleteLexicon->isChecked()) return;

	//else, reload
	filterListbyPattern(); //this will take care of the currently active filter
	// if there is none set, we will just load the list with loadList
}

void WordListViewPrivate::showImportDictDialog()
{
	importDictView->restart();
	importDictView->show();
}

/**
 * \brief Import an existing dictionary
 * \author Peter Grasch
 */
void WordListViewPrivate::importDict(WordList* list)
{
	if (list)
	{
		wordListManager->addWords(list, true,true);
	}
}


/**
 * @brief Marks a word for Training
 *
 * @author Peter Grasch
 */
void WordListViewPrivate::markWordToTrain(Word word)
{
	this->trainingwordlist.append( word );
	ui.lwTrainingWords->addItem( word.getWord() );
}



/**
 * @brief Filters the QList
 *
 * @param QString filter
 * The pattern which the Vocablist is matched against
 * @author Peter Grasch
 */
void WordListViewPrivate::filterListbyPattern(QString filter)
{
	if (filter.isEmpty()) filter = ui.leSearch->text().trimmed();
	
	WordList* limitedVocab = wordListManager->getWords(filter, ui.cbShowCompleteLexicon->isChecked(), 
				true, false /* display words twice which are in the active AND the shadowdict*/);
	
	WordListModel *model = dynamic_cast<WordListModel*>(ui.tvVocab->model());
	if (!model)
		ui.tvVocab->setModel(new WordListModel(limitedVocab));
	else model->updateWordList(limitedVocab);
}

/**
 * \brief Trains the list of words to train
 * Tells the TrainingView to guide the user to the process of training the
 * Wordlist given in lwTrainingWords (member)
 * \author Peter Grasch
 */
void WordListViewPrivate::trainList()
{
	if (this->trainingwordlist.count()==0)
	{
		KMessageBox::error(this, i18n("Please select a few words for the special training by dragging them from the "
"list on your left to the list above.\n\nIf you just want to train your model "
"using generic texts use the \"Training\" option in the Toolbar."));
		return;
	}

	TrainingsWizard *wizard = new TrainingsWizard(this);
	if (wizard->init(trainingwordlist)&& wizard->exec())
	{
		trainingwordlist.clear();
		ui.lwTrainingWords->clear();
	}
	wizard->deleteLater();
}

/**
 * @brief Copies a word to the Traininglist
 *
 * Copies the currently selected word from the tvVocab to the
 * lwTrainingWords (member)
 *
 * @author Peter Grasch
 */
void WordListViewPrivate::copyWordToTrain()
{
	if (!ui.tvVocab->currentIndex().isValid())
	{
		KMessageBox::information(this,i18n("Please select a word first"));
		return;
	}
	Word *w = static_cast<Word*>(ui.tvVocab->currentIndex().internalPointer());
	if (!w) return;
	
	this->trainingwordlist.append(*w);
	
	ui.lwTrainingWords->addItem(w->getWord());
}



void WordListViewPrivate::show()
{
	if (sImportDict & shownDialogs)
	{
		importDictView->show();
		importDictView->move(importDictPos);
	}
	QWidget::show();
	shownDialogs = 0;
}

void WordListViewPrivate::hide()
{
	if (importDictView->isVisible())
	{
		shownDialogs = shownDialogs | sImportDict;
		importDictPos = importDictView->pos();
		importDictView->hide();
	}
	
	QWidget::hide();
}


/**
 * \brief Displays a dialog to ask the user what to do
 * \author Peter Grasch
 * 
 * Available options: 
 * 	Remove completely
 * 	Move to shadow list
 */
void WordListViewPrivate::deleteSelectedWord()
{
	if (!ui.tvVocab->currentIndex().isValid())
	{
		KMessageBox::information(this,i18n("Please select a word first"));
		return;
	}
	
	Word *wordTemp = static_cast<Word*>(ui.tvVocab->currentIndex().internalPointer());
	
	DeleteWordDialog *del = new DeleteWordDialog(this);

	bool isShadowed=false;
	bool success = true;

	Word *w = this->wordListManager->getWord(wordTemp->getWord(), wordTemp->getPronunciation(), wordTemp->getTerminal(), isShadowed);
	if (!w)
	{
		return; //word not found?!
	}
	
	if (del->exec(*w, isShadowed))
	{
		//delete the word
		if (del->getDeletionType() == DeleteWordDialog::MoveToShadow)
		{
			success = wordListManager->moveToShadow(w);
		}
		if (del->getDeletionType() == DeleteWordDialog::RemoveCompletely)
		{
			success = wordListManager->deleteCompletely(w, isShadowed);
		}
		
		if (!success)
			KMessageBox::error(this, i18n("The word could not be completely removed."));
	}
	//do not delete w as it is a pointer to its wordlistmanager-representation
	del->deleteLater();
}

/**
 * @brief Deletes the selected word from the Training-List
 *
 * @todo The list only knows the word - not the pronunciation - if there are more than one it should open a dialog and ask what pronunciation should be deleted
 * @author Peter Grasch
 * @note This deletes every pronunciation of the word in the list
 */
void WordListViewPrivate::deleteTrainingWord()
{
	if (ui.lwTrainingWords->item(ui.lwTrainingWords->currentRow()))
	{
		QString word = ui.lwTrainingWords->takeItem(ui.lwTrainingWords->currentRow())->text();
		
		int i=0;
		while  (i < trainingwordlist.count())
		{
			if (trainingwordlist.at(i).getWord() == word)
				trainingwordlist.removeAt(i--);
			i++;
		}
		i=0;
		while  (i < ui.lwTrainingWords->count())
		{
			if (ui.lwTrainingWords->item(i)->text() == word)
				ui.lwTrainingWords->takeItem(i--);
			i++;
		}
	}
}



/**
 * @brief Destructor
 *
 * Empty Destructor
 *
 * @author Peter Grasch
 */
WordListViewPrivate::~WordListViewPrivate()
{
    importDictView->deleteLater();
}
