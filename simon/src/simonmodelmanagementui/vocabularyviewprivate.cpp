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


#include "vocabularyviewprivate.h"
#include "deleteworddialog.h"
#include "ImportDict/importdictview.h"
#include "TrainSamples/trainingswizard.h"

#include <speechmodelmanagement_scenario/wordlistmanager.h>
#include <speechmodelmanagement_scenario/wordlistmodel.h>

#include <speechmodelbase_scenario/scenario.h>

#include <simonlogging/logger.h>

#include <QTableWidget>
#include <QHeaderView>
#include <QString>
#include <KMessageBox>
#include <KProgressDialog>
#include <QCoreApplication>
#include <KIcon>
#include <KColorScheme>
#include <KDebug>

/**
 * @brief Constructor
 *
 * Creates a new WordlistView Objects, creates the Links
 * and initializes the list of vocabulary
 *
 * @author Peter Grasch
 */
VocabularyViewPrivate::VocabularyViewPrivate(QWidget *parent) : QWidget(parent)
{
	abortVocabInsertion = false;
	
	ui.setupUi(this);

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
	//	2. filterListbyPattern() displays a progressdialog while filtering the vocabulary
	//	   which of course gets the focus.
	//	3. This means we get a hit on editingFinished() which starts to filter the Wordlist again
	//	4. Which shows the progressdialog again
	//	5. Which, when shown will activate editingFinished() again, etc.
	// so we just use returnPressed() for now, which makes more sense anyways
	
	connect (ui.pbTrainList, SIGNAL(clicked()), this, SLOT(trainList()));
	connect(ui.pbImport, SIGNAL(clicked()), this, SLOT(showImportDictDialog()));
	
	connect(ui.cbShowCompleteLexicon, SIGNAL(toggled(bool)), this, SLOT(filterListbyPattern()));

	//this->wordListManager = WordListManager::getInstance();
	//connect(this->wordListManager, SIGNAL(vocabularyChanged()), this, SLOT(filterListbyPattern()));
	
	//connect(this->wordListManager, SIGNAL(shadowListChanged()), this, SLOT(reloadShadowList()));
	this->filterListbyPattern();

	ui.pbImport->setIcon(KIcon("document-import"));
	ui.pbRemoveWord->setIcon(KIcon("edit-delete"));
	ui.pbAddToTraining->setIcon(KIcon("list-add"));
	ui.pbDeleteTrainingWord->setIcon(KIcon("list-remove"));
	ui.pbTrainList->setIcon(KIcon("go-next"));

	ui.tvVocab->setSelectionBehavior(QAbstractItemView::SelectRows);
}

void VocabularyViewPrivate::displayScenarioPrivate(Scenario *scenario)
{
	kDebug() << "Displaying scenario " << scenario->name();

}


/**
 * \brief Reloads the vocabulary if the shadowlist is displayed
 * \author Peter Grasch
 * \note Respects filters (by calling filterListbyPattern())
 */
void VocabularyViewPrivate::reloadShadowList()
{
	//if the shadowlist is not even shown - why bother?
	if (!ui.cbShowCompleteLexicon->isChecked()) return;

	//else, reload
	filterListbyPattern(); //this will take care of the currently active filter
	// if there is none set, we will just load the list with loadList
}

void VocabularyViewPrivate::showImportDictDialog()
{
	ImportDictView *importDictView = new ImportDictView(this);
	//connect(importDictView, SIGNAL(dictGenerated(Vocabulary*, VocabularyTarget::VocabularyType)), this, SLOT(importDict(Vocabulary*, VocabularyTarget::VocabularyType)));
	importDictView->exec();
	importDictView->deleteLater();
}

/**
 * \brief Import an existing dictionary
 * \author Peter Grasch
 */
void VocabularyViewPrivate::importDict(WordList* list, WordListTarget::WordListType type)
{
	//if (list) {
	//	wordListManager->addWords(list, true, ((type == VocabularyTarget::ShadowList) ? true : false));
	//}
}


/**
 * @brief Marks a word for Training
 *
 * @author Peter Grasch
 */
void VocabularyViewPrivate::markWordToTrain(Word word)
{
	//this->trainingvocabulary.append( word );
	//ui.lwTrainingWords->addItem( word.getWord() );
}



/**
 * @brief Filters the QList
 *
 * @param QString filter
 * The pattern which the Vocablist is matched against
 * @author Peter Grasch
 */
void VocabularyViewPrivate::filterListbyPattern(QString filter)
{
	//if (filter.isEmpty()) filter = ui.leSearch->text().trimmed();
	
	//Vocabulary* limitedVocab = wordListManager->getWords(filter, ui.cbShowCompleteLexicon->isChecked(), 
	//			WordListManager::PartialMatch, false /* display words twice which are in the active AND the shadowdict*/);
	
	//VocabularyModel *model = dynamic_cast<VocabularyModel*>(ui.tvVocab->model());
	//if (!model)
//		ui.tvVocab->setModel(new VocabularyModel(limitedVocab, ui.tvVocab));
//	else model->updateVocabulary(limitedVocab);
}

/**
 * \brief Trains the list of words to train
 * Tells the TrainingView to guide the user to the process of training the
 * Wordlist given in lwTrainingWords (member)
 * \author Peter Grasch
 */
void VocabularyViewPrivate::trainList()
{
/*	if (this->trainingvocabulary.count()==0)
	{
		KMessageBox::error(this, i18n("Please select a few words for the special training by dragging them from the "
"list on your left to the list above.\n\nIf you just want to train your model "
"using generic texts use the \"Training\" option in the Toolbar."));
		return;
	}

	TrainingsWizard *wizard = new TrainingsWizard(this);
	if (wizard->init(trainingvocabulary)&& wizard->exec())
	{
		trainingvocabulary.clear();
		ui.lwTrainingWords->clear();
	}
	wizard->deleteLater();*/
}

/**
 * @brief Copies a word to the Traininglist
 *
 * Copies the currently selected word from the tvVocab to the
 * lwTrainingWords (member)
 *
 * @author Peter Grasch
 */
void VocabularyViewPrivate::copyWordToTrain()
{
/*	if (!ui.tvVocab->currentIndex().isValid())
	{
		KMessageBox::information(this,i18n("Please select a word first"));
		return;
	}
	Word *w = static_cast<Word*>(ui.tvVocab->currentIndex().internalPointer());
	if (!w) return;
	
	this->trainingvocabulary.append(*w);
	
	ui.lwTrainingWords->addItem(w->getWord());*/
}


/**
 * \brief Displays a dialog to ask the user what to do
 * \author Peter Grasch
 * 
 * Available options: 
 * 	Remove completely
 * 	Move to shadow list
 */
void VocabularyViewPrivate::deleteSelectedWord()
{
/*	if (!ui.tvVocab->currentIndex().isValid())
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
	//do not delete w as it is a pointer to its vocabularymanager-representation
	del->deleteLater();*/
}

/**
 * @brief Deletes the selected word from the Training-List
 *
 * @todo The list only knows the word - not the pronunciation - if there are more than one it should open a dialog and ask what pronunciation should be deleted
 * @author Peter Grasch
 * @note This deletes every pronunciation of the word in the list
 */
void VocabularyViewPrivate::deleteTrainingWord()
{
	/*
	if (ui.lwTrainingWords->item(ui.lwTrainingWords->currentRow()))
	{
		QString word = ui.lwTrainingWords->takeItem(ui.lwTrainingWords->currentRow())->text();
		
		int i=0;
		while  (i < trainingvocabulary.count())
		{
			if (trainingvocabulary.at(i).getWord() == word)
				trainingvocabulary.removeAt(i--);
			i++;
		}
		i=0;
		while  (i < ui.lwTrainingWords->count())
		{
			if (ui.lwTrainingWords->item(i)->text() == word)
				ui.lwTrainingWords->takeItem(i--);
			i++;
		}
	} else
		KMessageBox::information(this, i18n("Please select a word scheduled for training first."));
		*/
}



/**
 * @brief Destructor
 *
 * Empty Destructor
 *
 * @author Peter Grasch
 */
VocabularyViewPrivate::~VocabularyViewPrivate()
{
}
