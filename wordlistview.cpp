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
	
	this->initializeItems();
	importDictView = new ImportDictView(this);
	
	connect(ui.pbAddToTraining, SIGNAL(clicked()), this, SLOT(copyWordToTrain()));
	connect(ui.pbDeleteTrainingWord, SIGNAL(clicked()), this, SLOT(deleteTrainingWord()));
	connect(this->lwTrainingWords, SIGNAL(droppedText(QString)), this, SLOT(copyWordToTrain()));
	connect(ui.pbAddWord, SIGNAL(clicked()), this, SIGNAL(showAddWordDialog()));
	
	connect(ui.pbBack, SIGNAL(clicked()), this, SLOT(askToSave()));
	connect(ui.pbBack, SIGNAL(clicked()), this, SLOT(close()));
	
	connect(ui.pbSuggestTrain, SIGNAL(clicked()), this, SLOT(suggestTraining()));
	connect(ui.leSearch, SIGNAL(returnPressed()), this, SLOT(filterListbyPattern()));
	connect(ui.pbClearSearch, SIGNAL(clicked()), this, SLOT(clearSearchText()));
	connect(ui.pbSwitchToTraining, SIGNAL(clicked()), this, SLOT(switchToGenericTraining()));
	connect (ui.pbTrainList, SIGNAL(clicked()), this, SLOT(trainList()));
	connect(ui.pbImport, SIGNAL(clicked()), this, SLOT(showImportDictDialog()));
	connect(importDictView, SIGNAL(dictGenerated(WordList*)), this, SLOT(importDict(WordList*)));
	
	connect(ui.cbShowCompleteLexicon, SIGNAL(toggled(bool)), this, SLOT(toggleExtraWords()));
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
 * \brief Import an existing dictionary
 * \author Peter Grasch
 */
void WordListView::importDict(WordList* list)
{
	if (list)
	{
		setDirty(true);
		wordListManager->addWords(list, true);
		insertVocab(wordListManager->sortList(wordListManager->getWordList()));
	}
}

/**
 * \brief Escape closes "dialog"
 * \author Peter Grasch
 * @param e 
 */
void WordListView::keyPressEvent( QKeyEvent *e )
{
        if ( e->key() == Qt::Key_Escape )
                this->close();
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
	this->twVocab->sortItems(3);
	for (int i=0; i<10; i++)
	{
		this->twVocab->setCurrentItem(this->twVocab->item(i,0));
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
	this->lwTrainingWords->addItem( word.getWord() );
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
	if (this->twVocab->selectedItems().isEmpty())
	{
		QMessageBox::information(this,tr("Nichts ausgewählt"),tr("Bitte selektieren Sie zuerst ein Wort aus der Liste links"));
		return;
	}
	
	QString word = this->twVocab->item(this->twVocab->currentRow(),0)->text();
	QString sampa = this->twVocab->item(this->twVocab->currentRow(),1)->text();
	QString category = this->twVocab->item(this->twVocab->currentRow(),2)->text();
	int probability = this->twVocab->item(this->twVocab->currentRow(),3)->text().toInt();
	
	this->trainingwordlist.append(Word(word, sampa, category, probability));
	
	this->lwTrainingWords->addItem(word);
}



/**
 * @brief Switches to the Generic Training Dialog
 * 
 * Discards the Dialog and opens the Training Dialog
 *
 *	@author Peter Grasch
 */
void WordListView::switchToGenericTraining()
{
	close();
	trainView->show();
}


void WordListView::toggleExtraWords()
{
	clearList();
	ui.leSearch->setText("");
	if (ui.cbShowCompleteLexicon->checkState() == Qt::Checked)
	{
		insertVocab(this->wordListManager->getWordList());
// 		insertVocab(this->wordListManager->getExtraWords());
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
	this->twVocab->setRowCount(0);
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
	twVocab->setRowCount(vocab->count());
	QProgressDialog *pgDlg = new QProgressDialog(tr("Lade Wortliste zur Anzeige...\n(Ein Abbruch beeinflusst das intern verwendete Wörterbuch nicht!)"), tr("Abbrechen"), 0, 
			((vocab->count() < 1000) ? vocab->count() : 1000), this);

	connect(pgDlg, SIGNAL(canceled()), this, SLOT(abortInsertion()));

	twVocab->hide();
        int i=0;
	int limit=1000;
	
	while ((!abortVocabInsertion) && (i<vocab->count()) && (i<limit))
	{
		QCoreApplication::processEvents();
		if (!vocab->at(i).getWord().isEmpty())
		{
		twVocab->setRowCount(i+1);
		twVocab->setItem(i, 0, new QTableWidgetItem(vocab->at(i).getWord()));
		twVocab->setItem(i, 1, new QTableWidgetItem(*(vocab->at(i).getPronunciation(0))));
		twVocab->setItem(i, 2, new QTableWidgetItem(vocab->at(i).getTerminal()));
		
		QTableWidgetItem *prob = new QTableWidgetItem(QString().setNum(vocab->at(i).getPropability()));
		if (vocab->at(i).getPropability() == 0)
			prob->setBackgroundColor( QColor(255,0,0) );
		else 
			if (vocab->at(i).getPropability() < 2)
				prob->setBackgroundColor( QColor( 241, 134, 134 ) );
		twVocab->setItem(i, 3, prob);
		

		for (int j = 0; j<4; j++)
			twVocab->item(i,j)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
		} else {
			vocab->removeAt(i);
			twVocab->setRowCount(vocab->count());
			i--;
		}
		pgDlg->setValue(++i);
	}
	twVocab->setRowCount(i);
	twVocab->show();
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
	if (this->lwTrainingWords->item(this->lwTrainingWords->currentRow()))
	{
		QString word = this->lwTrainingWords->takeItem(this->lwTrainingWords->currentRow())->text();
		
		int i=0;
		while  (i < trainingwordlist.count())//(wordlist.at(i)->getWord() != word)
		{
			if (trainingwordlist.at(i).getWord() == word)
				trainingwordlist.removeAt(i);
			i++;
		}
		i=0;
		while  (i < lwTrainingWords->count())//(wordlist.at(i)->getWord() != word)
		{
			if (this->lwTrainingWords->item(i)->text() == word)
				this->lwTrainingWords->takeItem(i--);
			i++;
		}
	}
}

/**
 * @brief Creates the QTableWidget and the QListWidget
 *
 * Creates the Table Wiget and the QList Widget,
 * creates the columns and adds the values
 *
 * @author Peter Grasch
 */
void WordListView::initializeItems()
{
	lwTrainingWords = new DropListWidget(this);
	lwTrainingWords->setObjectName(QString::fromUtf8("lwTrainingWords"));
	ui.wTrainList->layout()->addWidget(lwTrainingWords);
	
	twVocab = new DragTableWidget(this);
	twVocab->setObjectName("twVocab");
	twVocab->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	twVocab->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	twVocab->setSelectionBehavior(QAbstractItemView::SelectRows);
	twVocab->setSelectionMode(QAbstractItemView::SingleSelection);
	
	twVocab->verticalHeader()->hide();
	
	twVocab->setColumnCount(4);
	QTableWidgetItem *__colItem = new QTableWidgetItem();
	__colItem->setText(QApplication::translate("WordList", "Wort", 0, QApplication::UnicodeUTF8));
	twVocab->setHorizontalHeaderItem(0, __colItem);
	QTableWidgetItem *__colItem1 = new QTableWidgetItem();
	__colItem1->setText(QApplication::translate("WordList", "Sam-Pa", 0, QApplication::UnicodeUTF8));
	twVocab->setHorizontalHeaderItem(1, __colItem1);

	QTableWidgetItem *__colItem2 = new QTableWidgetItem();
	__colItem2->setText(QApplication::translate("WordList", "Kategorie", 0, QApplication::UnicodeUTF8));
	twVocab->setHorizontalHeaderItem(2, __colItem2);

	QTableWidgetItem *__colItem3 = new QTableWidgetItem();
	__colItem3->setText(QApplication::translate("WordList", "Erkennungsrate", 0, QApplication::UnicodeUTF8));
	twVocab->setHorizontalHeaderItem(3, __colItem3);
	
	ui.wList->layout()->addWidget(twVocab);

	this->readVocab();
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
