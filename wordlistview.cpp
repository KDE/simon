//
// C++ Implementation: wordlistview
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include <QProgressDialog>
#include <QCoreApplication>
#include <QIcon>
#include "logger.h"
#include "wordlistview.h"
#include "settings.h"
#include "deleteworddialog.h"

/**
 * @brief Constructor
 *
 * Creates a new WordlistView Objects, creates the Links
 * and initializes the list of vocabulary
 *
 * @author Peter Grasch
 */
WordListView::WordListView(QWidget *parent) : InlineWidget(tr("Wortliste"), 
	QIcon(":/images/icons/format-justify-fill.svg"), 
	tr("Betrachten und bearbeiten der Wortliste"), parent)
{

	shownDialogs = 0;
	abortVocabInsertion = false;
	this->wordListManager = new WordListManager();
	
	ui.setupUi(this);

	ui.twVocab->verticalHeader()->hide();
	
	this->initializeItems();
	importDictView = new ImportDictView(this);
	
	connect(ui.pbAddToTraining, SIGNAL(clicked()), this, SLOT(copyWordToTrain()));
	connect(ui.pbDeleteTrainingWord, SIGNAL(clicked()), this, SLOT(deleteTrainingWord()));
	connect(ui.lwTrainingWords, SIGNAL(droppedText(QString)), this, SLOT(copyWordToTrain()));
	connect(ui.pbAddWord, SIGNAL(clicked()), this, SIGNAL(showAddWordDialog()));
	
	connect(ui.pbSuggestTrain, SIGNAL(clicked()), this, SLOT(suggestTraining()));
	connect(ui.pbRemoveWord, SIGNAL(clicked()), this, SLOT(deleteSelectedWord()));
	connect(ui.leSearch, SIGNAL(returnPressed()), this, SLOT(filterListbyPattern()));
	connect(ui.pbClearSearch, SIGNAL(clicked()), this, SLOT(clearSearchText()));
	connect (ui.pbTrainList, SIGNAL(clicked()), this, SLOT(trainList()));
	connect(ui.pbImport, SIGNAL(clicked()), this, SLOT(showImportDictDialog()));
	connect(importDictView, SIGNAL(dictGenerated(WordList*)), this, SLOT(importDict(WordList*)));
	
	connect(ui.cbShowCompleteLexicon, SIGNAL(toggled(bool)), this, SLOT(toggleExtraWords()));

	connect(this->wordListManager, SIGNAL(wordlistChanged()), this, SLOT(reloadList()));
	dirty = false;
	hide();
}

void WordListView::reloadList()
{
	this->clearList();
	readVocab();
}

/**
 * @brief Clears the Search-field
 *
 * @author Peter Grasch
 */
void WordListView::clearSearchText()
{
	ui.leSearch->setText("");
	filterListbyPattern("");
	ui.leSearch->setFocus();
}

void WordListView::showImportDictDialog()
{
	importDictView->show();
}

/**
 * \brief Compiles the language model
 * \author Peter Grasch
 */
void WordListView::compileModel()
{
	this->wordListManager->compileModel();
}

/**
 * \brief Import an existing dictionary
 * \author Peter Grasch
 */
void WordListView::importDict(WordList* list)
{
	if (list)
	{
		wordListManager->addWords(list, true,true);
		
		
// 		if (!wordListManager->save())
// 		{	
// 			QMessageBox::critical(this, tr("Fehler beim Speichern"), tr("Das Wörterbuch konnte nicht nach %1 geschrieben werden. Bitte überprüfen Sie Ihre Berechtigungen.").arg(Settings::get("Model/PathToLexicon").toString()));
// 			Logger::log("[ERR] Fehler beim schreiben des Wörterbuches");
// 		}
		
// 		insertVocab(wordListManager->getWordList());
	}
}

/**
 * @brief Suggest a training
 *
 * Uses the recognition rate and chooses the lowest 10
 *
 * @author Peter Grasch
 */
void WordListView::suggestTraining()
{
	ui.twVocab->sortItems(3);
	int toInsert = 10;
	if (ui.twVocab->rowCount() < toInsert) toInsert = ui.twVocab->rowCount();

	if (toInsert==0) QMessageBox::information(this, ("Keine Wörter in der Wortliste"), tr("Es sind nicht genügend Wörter im wörterbuch"));
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
void WordListView::markWordToTrain(Word word)
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
void WordListView::filterListbyPattern(QString filter)
{
	if (filter.isEmpty()) filter = ui.leSearch->text();
	WordList *vocab = this->wordListManager->getWordList();
	
	WordList *limitedVocab = new WordList();

	int i=0;
	while (i < vocab->count())
	{
		if (vocab->at(i).getWord().contains(filter, Qt::CaseInsensitive))
			limitedVocab->append(vocab->at(i));
		
		i++;
	}
	insertVocab( limitedVocab );
}

/**
 * \brief Trains the list of words to train
 * Tells the TrainingView to guide the user to the process of training the
 * Wordlist given in lwTrainingWords (member)
 * \author Peter Grasch
 */
void WordListView::trainList()
{
	if (!trainView) return;
	if (this->trainingwordlist.count()==0)
	{
		QMessageBox::critical(this, tr("Keine Wörter selektiert"), tr("Bitte wählen Sie zuerst ein paar Wörter für das spezielle Training aus.\nZiehen Sie sie dazu von der großen Liste links in die kleine Liste oben rechts.\n\nWenn Sie generische Texte vorlesen wollen, gehen Sie bitte in das Allgemeine Training.\n(Der Punkt \"Trainieren\" links neben dem simon-Logo)."));
		return;
	}
	trainView->trainWords(&trainingwordlist);
	trainView->exec();
}

/**
 * @brief Copys a word to the Traininglist
 *
 * Copys the currently selected word from the twVocab (member) to the
 * lwTrainingWords (member)
 *
 * @author Peter Grasch
 */
void WordListView::copyWordToTrain()
{
	if (ui.twVocab->selectedItems().isEmpty())
	{
		QMessageBox::information(this,tr("Nichts ausgewählt"),tr("Bitte selektieren Sie zuerst ein Wort aus der Liste links"));
		return;
	}
	
	QString word = ui.twVocab->item(ui.twVocab->currentRow(),0)->text();
	QString sampa = ui.twVocab->item(ui.twVocab->currentRow(),1)->text();
	QString category = ui.twVocab->item(ui.twVocab->currentRow(),2)->text();
	int probability = ui.twVocab->item(ui.twVocab->currentRow(),3)->text().toInt();
	
	this->trainingwordlist.append(Word(word, sampa, category, probability));
	
	ui.lwTrainingWords->addItem(word);
}


void WordListView::toggleExtraWords()
{
	clearList();
	ui.leSearch->setText("");
	if (ui.cbShowCompleteLexicon->checkState() == Qt::Checked)
	{
		insertVocab(this->wordListManager->getWordList());
		insertVocab(this->wordListManager->getShadowList());
	} else {
		insertVocab(this->wordListManager->getWordList());
	}
}


/**
 * \brief Clears the VocabList
 * \author Peter Grasch
 */
void WordListView::clearList()
{
	ui.twVocab->setRowCount(0);
}

/**
 * \brief Sets the Member and lets the user know about it
 * \author Peter Grasch
 * \param bool dirty
 */
void WordListView::setDirty(bool dirty)
{
	this->dirty = dirty;
	if (dirty)
	{
		setWindowTitle(tr("Wortliste - Ungesicherte Änderungen"));
	} else setWindowTitle(tr("Wortliste"));
}

void WordListView::show()
{
	if (sImportDict & shownDialogs)
	{
		importDictView->show();
		importDictView->move(importDictPos);
	}
	QWidget::show();
	shownDialogs = 0;
}

void WordListView::hide()
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
 * \brief Asks the user if he wants to save
 * \author Peter Grasch
 */
void WordListView::askToSave()
{
	if (!dirty) return;
	
	if (QMessageBox::question(this, tr("Änderungen speichern"), tr("Sie haben das Sprachmodell verändert. Möchten Sie die Änderungen speichern?"), QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
	{
		//save the dict
		QString filename = "test.out";
		if (this->wordListManager->save(filename))
			setDirty(false);
		else 
		{	
			QMessageBox::critical(this, tr("Fehler beim Speichern"), tr("Das Wörterbuch konnte nicht nach %1 geschrieben werden. Bitte überprüfen Sie Ihre Berechtigungen.").arg(filename));
			Logger::log("[ERR] Fehler beim schreiben des Wörterbuches");
		}
	}
}


/**
 * \brief Displays a dialog to ask the user what to do
 * \author Peter Grasch
 * 
 * Available options: 
 * 	Remove completely
 * 	Move to shadow list
 */
void WordListView::deleteSelectedWord()
{
	DeleteWordDialog *del = new DeleteWordDialog(this);
	int wordId = ui.twVocab->item(ui.twVocab->currentRow(),0)->data(Qt::UserRole).toInt();
	
	del->exec(this->wordListManager->getWord(wordId));
}

/**
 * @brief Asks the WordListManager for the whole Vocabulary and sends it to insertVocab
 *
 * @author Peter Grasch
 * @see insertVocab()
 */
void WordListView::readVocab()
{
	WordList *vocab = this->wordListManager->getWordList();
	
	if (!vocab)
	{
		QMessageBox::critical(0, tr("Lesefehler"), tr("Konnte benötigte Dateien nicht einlesen. Bitte überprüfen Sie die Einstellungen und stellen Sie sicher das sie die nötigen Leserechte haben."));
		return;
	}
	
	insertVocab( vocab );
}

/**
 * @brief Inserts a given Wordlist to the QTableWidget
 *
 * @author Peter Grasch
 * @see readVocab()
 * @param WordList *vocab
 * The Wordlist to insert
 */
void WordListView::insertVocab(WordList *vocab)
{
	abortVocabInsertion=false;
	ui.twVocab->setRowCount(vocab->count());
	QProgressDialog *pgDlg = new QProgressDialog(tr("Lade Wortliste zur Anzeige...\n(Ein Abbruch beeinflusst das intern verwendete Wörterbuch nicht!)"), tr("Abbrechen"), 0, 
			((vocab->count() < 1000) ? vocab->count() : 1000), this);

	connect(pgDlg, SIGNAL(canceled()), this, SLOT(abortInsertion()));

        int i=0;
	int limit=1000;
	
	
	ui.twVocab->setRowCount(vocab->count());
	while ((!abortVocabInsertion) && (i<vocab->count()) && (i<limit))
	{
		if (!vocab->at(i).getWord().isEmpty())
		{
			QTableWidgetItem *wordName = new QTableWidgetItem(vocab->at(i).getWord());
			wordName->setData(Qt::UserRole, i);
			ui.twVocab->setItem(i, 0, wordName);
			ui.twVocab->setItem(i, 1, new QTableWidgetItem(vocab->at(i).getPronunciation()));
			ui.twVocab->setItem(i, 2, new QTableWidgetItem(vocab->at(i).getTerminal()));
			
			QTableWidgetItem *prob = new QTableWidgetItem(QString().setNum(vocab->at(i).getPropability()));
			if (vocab->at(i).getPropability() == 0)
				prob->setBackgroundColor( QColor(255,0,0) );
			else 
				if (vocab->at(i).getPropability() < 2)
					prob->setBackgroundColor( QColor( 241, 134, 134 ) );
			ui.twVocab->setItem(i, 3, prob);
			
	
			for (int j = 0; j<4; j++)
				ui.twVocab->item(i,j)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
		} else {
			vocab->removeAt(i);
			pgDlg->setMaximum(vocab->count());
			i--;
		}
		pgDlg->setValue(++i);
	}
	ui.twVocab->setRowCount(i);
	pgDlg->hide();
	emit wordlistLoaded();
}


/**
 * @brief Deletes the selected word from the Training-List
 *
 * @todo The list only knows the word - not the pronounciation - if there are more than one it should open a dialog and ask what pronounciation should be deleted
 * @author Peter Grasch
 * @note This deletes every pronounciation of the word in the list
 */
void WordListView::deleteTrainingWord()
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

/**
 * @brief Loads the Vocab
 *
 * @author Peter Grasch
 */
void WordListView::initializeItems()
{

	this->readVocab();
}

/**
 * @brief Hides the tab (editModel), until the pbKeyed Button is checked and the password where given
 *
 * @autor Susanne Tschernegg
 */
void WordListView::hideTbEditModel()
{
    ui.twCurrentAction->removeTab(0);
}

/**
 * @brief Sets the tab (editModel) visible, until the pbKeyed Button is unchecked
 *
 * @autor Susanne Tschernegg
 */
void WordListView::setTbEditModelVisible()
{
    ui.twCurrentAction->insertTab(0,ui.tbEditModel, tr("Wortliste ändern"));
}


/**
 * @brief Destructor
 *
 * Empty Destructor
 *
 * @author Peter Grasch
 */
WordListView::~WordListView()
{
}
