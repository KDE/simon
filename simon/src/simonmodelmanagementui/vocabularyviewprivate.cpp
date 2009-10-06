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

#include <speechmodelmanagement/activevocabulary.h>
#include <speechmodelmanagement/scenariomanager.h>
#include <speechmodelmanagement/shadowvocabulary.h>

#include <speechmodelmanagement/scenario.h>

#include <simonlogging/logger.h>

#include <QString>
#include <KMessageBox>
#include <KProgressDialog>
#include <QCoreApplication>
#include <KIcon>
#include <KDebug>
#include <QSortFilterProxyModel>

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
	connect(ui.leActiveVocabSearch, SIGNAL(returnPressed()), this, SLOT(refreshActiveView()));
	connect(ui.leShadowVocabSearch, SIGNAL(returnPressed()), this, SLOT(refreshShadowView()));
	connect(ui.leActiveVocabSearch, SIGNAL(clearButtonClicked()), this, SLOT(refreshActiveView()));
	connect(ui.leShadowVocabSearch, SIGNAL(clearButtonClicked()), this, SLOT(refreshShadowView()));
	
	connect (ui.pbTrainList, SIGNAL(clicked()), this, SLOT(trainList()));
	connect(ui.pbImport, SIGNAL(clicked()), this, SLOT(showImportDictDialog()));
	
	ui.pbImport->setIcon(KIcon("document-import"));
	ui.pbRemoveWord->setIcon(KIcon("edit-delete"));
	ui.pbAddToTraining->setIcon(KIcon("list-add"));
	ui.pbDeleteTrainingWord->setIcon(KIcon("list-remove"));
	ui.pbTrainList->setIcon(KIcon("go-next"));

	ui.tvActiveVocab->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui.tvActiveVocab->setSortingEnabled(true);
	ui.tvShadowVocab->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui.tvShadowVocab->setSortingEnabled(true);

	activeProxy = new QSortFilterProxyModel(this);
	activeProxy->setFilterKeyColumn(0);
	ui.tvActiveVocab->setModel(activeProxy);
	
	
	shadowProxy = new QSortFilterProxyModel(this);
	shadowProxy->setFilterKeyColumn(0);
	shadowProxy->setSourceModel(ScenarioManager::getInstance()->getShadowVocabulary());
	ui.tvShadowVocab->setModel(shadowProxy);
}

void VocabularyViewPrivate::refreshActiveView()
{
	kDebug() << "Refreshing view with filter: " << ui.leActiveVocabSearch->text();

	activeProxy->setFilterRegExp(ui.leActiveVocabSearch->text());
	activeProxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
}

void VocabularyViewPrivate::refreshShadowView()
{
	kDebug() << "Refreshing view with filter: " << ui.leShadowVocabSearch->text();

	shadowProxy->setFilterRegExp(ui.leShadowVocabSearch->text());
	shadowProxy->setFilterCaseSensitivity(Qt::CaseInsensitive);
}

void VocabularyViewPrivate::displayScenarioPrivate(Scenario *scenario)
{
	kDebug() << "Displaying scenario " << scenario->name();

	activeProxy->setSourceModel(scenario->vocabulary());
}


void VocabularyViewPrivate::showImportDictDialog()
{
	ImportDictView *importDictView = new ImportDictView(this);
	Vocabulary::VocabularyType type;
	QList<Word*>* words = importDictView->importDict(type);
	bool worked = true;
	if (words) {
		switch (type) {
			case Vocabulary::ActiveVocabulary:
				worked = scenario->vocabulary()->addWords(words);
				break;
			case Vocabulary::ShadowVocabulary:
				worked = ScenarioManager::getInstance()->getShadowVocabulary()->addWords(words);
				break;
		}
	}
	if (!worked)  KMessageBox::error(this, i18n("Couldn't add words to the list"));

	importDictView->deleteLater();
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
	if (!scenario) return;

	QModelIndex selectedIndex = ui.tvActiveVocab->currentIndex();
	if (!selectedIndex.isValid()) {
		KMessageBox::information(this, i18n("Please select a word first"));
		return;
	}

	Word *w = static_cast<Word*>(ui.tvActiveVocab->currentIndex().internalPointer());
	DeleteWordDialog *del = new DeleteWordDialog(this);

	//TODO: shadow list display is not supported yet so this is always false
	bool isShadowed = false;

	if (del->exec(*w, isShadowed))
	{
		//delete the word
		if (del->getDeletionType() == DeleteWordDialog::MoveToShadow) {
			KMessageBox::information(this, i18n("Not yet supported"));
			//scenario->vocabulary()->removeWord(w);
			//success = wordListManager->moveToShadow(w);
		}
		if (del->getDeletionType() == DeleteWordDialog::RemoveCompletely) {
			scenario->vocabulary()->removeWord(w);
			delete w;
		}
	}
	del->deleteLater();
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
//void VocabularyViewPrivate::filterListbyPattern(QString filter)
//{
	//if (filter.isEmpty()) filter = ui.leSearch->text().trimmed();
	
	//Vocabulary* limitedVocab = wordListManager->getWords(filter, ui.cbShowCompleteLexicon->isChecked(), 
	//			WordListManager::PartialMatch, false /* display words twice which are in the active AND the shadowdict*/);
	
	//VocabularyModel *model = dynamic_cast<VocabularyModel*>(ui.tvActiveVocab->model());
	//if (!model)
//		ui.tvActiveVocab->setModel(new VocabularyModel(limitedVocab, ui.tvActiveVocab));
//	else model->updateVocabulary(limitedVocab);
//}

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
 * Copies the currently selected word from the tvActiveVocab to the
 * lwTrainingWords (member)
 *
 * @author Peter Grasch
 */
void VocabularyViewPrivate::copyWordToTrain()
{
/*	if (!ui.tvActiveVocab->currentIndex().isValid())
	{
		KMessageBox::information(this,i18n("Please select a word first"));
		return;
	}
	Word *w = static_cast<Word*>(ui.tvActiveVocab->currentIndex().internalPointer());
	if (!w) return;
	
	this->trainingvocabulary.append(*w);
	
	ui.lwTrainingWords->addItem(w->getWord());*/
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

