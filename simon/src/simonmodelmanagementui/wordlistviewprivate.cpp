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
// #include "coreconfiguration.h"

#include <speechmodelmanagement/wordlistmanager.h>
#include <simonlogging/logger.h>

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

	ui.twVocab->verticalHeader()->hide();
	
	importDictView = new ImportDictView(this);
	connect(ui.pbAddToTraining, SIGNAL(clicked()), this, SLOT(copyWordToTrain()));
	connect(ui.pbDeleteTrainingWord, SIGNAL(clicked()), this, SLOT(deleteTrainingWord()));
	connect(ui.lwTrainingWords, SIGNAL(droppedText(QString)), this, SLOT(copyWordToTrain()));
	
	connect(ui.pbSuggestTrain, SIGNAL(clicked()), this, SLOT(suggestTraining()));
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
	ui.pbSuggestTrain->setIcon(KIcon("bookmark-new-list"));
	ui.pbTrainList->setIcon(KIcon("go-next"));
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
 * @brief Suggest a training
 *
 * Uses the recognition rate and chooses the lowest 10
 *
 * @author Peter Grasch
 */
void WordListViewPrivate::suggestTraining()
{
	ui.twVocab->sortItems(3);
	int toInsert = 10;
	if (ui.twVocab->rowCount() < toInsert) toInsert = ui.twVocab->rowCount();

	if (toInsert==0) KMessageBox::information(this, i18n("Es sind nicht genügend Wörter im Wörterbuch"));
	for (int i=0; i<toInsert; i++)
	{
		ui.twVocab->setCurrentItem(ui.twVocab->item(i,0));
		copyWordToTrain();
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
// 	KMessageBox::information(this, i18n("seas"));
	if (filter.isEmpty()) filter = ui.leSearch->text().trimmed();
	
	clearList();

	WordList* limitedVocab = wordListManager->getWords(filter, ui.cbShowCompleteLexicon->isChecked(), 
				true, false /* display words twice which are in the active AND the shadowdict*/);
	
	insertVocab( limitedVocab );
	delete limitedVocab;
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
		KMessageBox::error(this, i18n("Bitte wählen Sie zuerst ein paar Wörter für das spezielle Training aus.\nZiehen Sie sie dazu von der großen Liste links in die kleine Liste oben rechts.\n\nWenn Sie generische Texte vorlesen wollen, gehen Sie bitte zum Allgemeine Training.\n(Der Punkt \"Trainieren\" ist in der \"Globale Aktion\"-Toolbar)."));
		return;
	}

	TrainingsWizard *wizard = new TrainingsWizard(trainingwordlist, this);
	wizard->exec();
	trainingwordlist.clear();
	ui.lwTrainingWords->clear();
}

/**
 * @brief Copies a word to the Traininglist
 *
 * Copies the currently selected word from the twVocab (member) to the
 * lwTrainingWords (member)
 *
 * @author Peter Grasch
 */
void WordListViewPrivate::copyWordToTrain()
{
	if (ui.twVocab->selectedItems().isEmpty())
	{
		KMessageBox::information(this,i18n("Bitte selektieren Sie zuerst ein Wort aus der Liste links"));
		return;
	}
	
	QString word = ui.twVocab->item(ui.twVocab->currentRow(),0)->text();
	QString sampa = ui.twVocab->item(ui.twVocab->currentRow(),1)->text();
	QString category = ui.twVocab->item(ui.twVocab->currentRow(),2)->text();
	int probability = ui.twVocab->item(ui.twVocab->currentRow(),3)->text().toInt();
	
	this->trainingwordlist.append(Word(word, sampa, category, probability));
	
	ui.lwTrainingWords->addItem(word);
}



/**
 * \brief Clears the VocabList
 * \author Peter Grasch
 */
void WordListViewPrivate::clearList()
{
	ui.twVocab->clearContents();
	ui.twVocab->setRowCount(0);
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
	int row = ui.twVocab->currentRow();
	if (row == -1) //none selected
	{
		KMessageBox::information(this, i18n("Bitte selektieren Sie ein Wort"));
		return;
	}

	DeleteWordDialog *del = new DeleteWordDialog(this);

	QString word = ui.twVocab->item(row,0)->text();
	QString pronunciation = ui.twVocab->item(row,1)->text();
	QString terminal = ui.twVocab->item(row,2)->text();
	
	bool isShadowed=false;
	bool success = true;

	Word *w = this->wordListManager->getWord(word, pronunciation, terminal, isShadowed);
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
			KMessageBox::error(this, i18n("Das Wort konnte nicht komplett gelöscht werden.\n\nBitte überprüfen Sie die Pfade der Dateien: prompts, codetrain.scp, Pfad der Samples, dict, shadow-dict, voca"));
	}
	//do not delete w as it is a pointer to its wordlistmanager-representation
	del->deleteLater();
}


/**
 * @brief Inserts a given Wordlist to the QTableWidget
 *
 * @author Peter Grasch
 * @see readVocab()
 * @param WordList *vocab
 * The Wordlist to insert
 */
void WordListViewPrivate::insertVocab(WordList *vocab)
{
	abortVocabInsertion=false;
	int startAmount=ui.twVocab->rowCount();
	int currentRow = startAmount;
        int i=0;
	int limit=1000; //CoreConfiguration::maxConcurrentlyDisplayedWords();
	KProgressDialog *pgDlg = new KProgressDialog(this, i18n("Lade Wortliste..."), i18n("Lade Wortliste zur Anzeige...\n(Ein Abbruch beeinflusst das intern verwendete Wörterbuch nicht!)"));
	pgDlg->progressBar()->setMaximum((vocab->count() < limit) ? vocab->count() : limit);

	connect(pgDlg, SIGNAL(rejected()), this, SLOT(abortInsertion()));

	ui.twVocab->setRowCount(startAmount+vocab->count());
	
	KColorScheme colorScheme(QPalette::Active);
	QColor negative = colorScheme.background(KColorScheme::NegativeBackground).color();
	
	while ((!abortVocabInsertion) && (i<vocab->count()) && (i<limit))
	{
		QString curWordName = vocab->at(i).getWord();
		if (!curWordName.isEmpty())
		{
			ui.twVocab->setItem(currentRow, 0, new QTableWidgetItem(curWordName));
			ui.twVocab->setItem(currentRow, 1, new QTableWidgetItem(vocab->at(i).getPronunciation()));
			ui.twVocab->setItem(currentRow, 2, new QTableWidgetItem(vocab->at(i).getTerminal()));
			
			int probability = vocab->at(i).getPropability();
			ui.twVocab->setItem(currentRow, 3, new QTableWidgetItem(QString::number(probability)));
			if (probability < 2)
			{
				QBrush specialCol;
				if (probability==0)
					specialCol = KColorScheme::shade(negative, KColorScheme::DarkShade);
				else specialCol = KColorScheme::shade(negative, KColorScheme::MidShade);
				
				for (int i=0; i <4; i++)
					ui.twVocab->item(currentRow,i)->setBackground(specialCol);
			}
	
			for (int j = 0; j<4; j++)
				ui.twVocab->item(currentRow,j)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
		} else {
			vocab->removeAt(i);
			pgDlg->progressBar()->setMaximum(vocab->count());
			i--;
		}
		i++;
		currentRow++;
		if ((i % 50)==0) pgDlg->progressBar()->setValue(i);
	}
	pgDlg->progressBar()->setValue(i);
	ui.twVocab->setRowCount(i);
	ui.twVocab->resizeColumnsToContents();
	pgDlg->deleteLater();
	emit wordlistLoaded();
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
		while  (i < trainingwordlist.count())//(wordlist.at(i)->getWord() != word)
		{
			if (trainingwordlist.at(i).getWord() == word)
				trainingwordlist.removeAt(i);
			i++;
		}
		i=0;
		while  (i < ui.lwTrainingWords->count())//(wordlist.at(i)->getWord() != word)
		{
			if (ui.lwTrainingWords->item(i)->text() == word)
				ui.lwTrainingWords->takeItem(i--);
			i++;
		}
	}
}


void WordListViewPrivate::setSettingsVisible()
{
	ui.wgSettings->show();
}

void WordListViewPrivate::setSettingsHidden()
{
	ui.wgSettings->hide();
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
