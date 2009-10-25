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

#include <simonscenarios/activevocabulary.h>
#include <simonscenarios/scenariomanager.h>
#include <simonscenarios/shadowvocabulary.h>

#include <simonscenarios/scenario.h>

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

/*	QSortFilterProxyModel *trainingVocProxy = new QSortFilterProxyModel(this);
	trainingVocProxy->setFilterKeyColumn(0);
	trainingVocProxy->setSourceModel(&trainingVocabulary);
	ui.lvTrainingWords->setModel(trainingVocProxy);*/
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

Word* VocabularyViewPrivate::getCurrentlySelectedWord(bool& isShadowed)
{
	QAbstractItemView *view;
	if (ui.twVocabularies->currentIndex() == 0) {
		view = ui.tvActiveVocab;
		isShadowed = false;
	}  else {
		view = ui.tvShadowVocab;
		isShadowed = true;
	}

	QSortFilterProxyModel* m = static_cast<QSortFilterProxyModel*>(view->model());
	QModelIndex selectedIndex = m->mapToSource(view->currentIndex());
	if (!selectedIndex.isValid()) {
		KMessageBox::information(this, i18n("Please select a word first"));
		return NULL;
	}

	return static_cast<Word*>(selectedIndex.internalPointer());
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

	bool isShadowed = true;
	
	Word *w = getCurrentlySelectedWord(isShadowed);

	DeleteWordDialog *del = new DeleteWordDialog(this);

	if (del->exec(w, isShadowed))
	{
		//delete the word
		if (del->getDeletionType() == DeleteWordDialog::MoveToShadow) {
			//this request has to come from a word that is currently in the active
			//vocabulary
			scenario->vocabulary()->removeWord(w, false /* don't delete the word */);
			ScenarioManager::getInstance()->getShadowVocabulary()->addWord(w);
		}
		if (del->getDeletionType() == DeleteWordDialog::RemoveCompletely) {
			if (ui.twVocabularies->currentIndex() == 0) {
				//active dictionary
				scenario->vocabulary()->removeWord(w);
			} else {
				//shadow vocabulary
				ScenarioManager::getInstance()->getShadowVocabulary()->removeWord(w);
			}
		}
	}
	del->deleteLater();
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
	bool isShadowed;
	Word *w = getCurrentlySelectedWord(isShadowed);
	if (!w) return;

	trainingVocabulary.append(w);
	ui.lwTrainingWords->addItem(QString("%1 (%2)").arg(w->getWord()).arg(w->getTerminal()));
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
	int index = ui.lwTrainingWords->currentRow();
	if (index == -1) {
		KMessageBox::information(this, i18n("Please select a word scheduled for training first."));
		return;
	}

	delete ui.lwTrainingWords->takeItem(index);
	trainingVocabulary.removeAt(index);
}



/**
 * \brief Trains the list of words to train
 * Tells the TrainingView to guide the user to the process of training the
 * Wordlist given in lwTrainingWords (member)
 * \author Peter Grasch
 */
void VocabularyViewPrivate::trainList()
{
	if (trainingVocabulary.count()==0) {
		KMessageBox::error(this, i18n("Please select a few words for the special training by dragging them from the "
"list on your left to the list above.\n\nIf you just want to train your model "
"using generic texts use the \"Training\" option in the Toolbar."));
		return;
	}

	TrainingsWizard *wizard = new TrainingsWizard(this);
	if (wizard->init(trainingVocabulary, ui.cbBuildSentences->isChecked())&& wizard->exec())
	{
		trainingVocabulary.clear();
		ui.lwTrainingWords->clear();
	}
	wizard->deleteLater();
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

