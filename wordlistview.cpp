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
#include "wordlistview.h"

/**
 * @brief Constructor
 *
 * Creates a new WordlistView Objects, creates the Links
 * and initializes the list of vocabulary
 *
 * @author Peter Grasch
 */
WordListView::WordListView(QWidget *parent) : QDialog(parent)
{
	this->wordListManager = new WordListManager();
	
	ui.setupUi(this);
	
	this->initializeItems();
	
	connect(ui.pbAddToTraining, SIGNAL(clicked()), this, SLOT(copyWordToTrain()));
	connect(ui.pbDeleteTrainingWord, SIGNAL(clicked()), this, SLOT(deleteTrainingWord()));
	connect(this->lwTrainingWords, SIGNAL(droppedText(QString)), this, SLOT(copyWordToTrain()));
	connect(ui.pbAddWord, SIGNAL(clicked()), this, SLOT(addWord()));
	connect(ui.pbBack, SIGNAL(clicked()), this, SLOT(close()));
	connect(ui.pbSuggestTrain, SIGNAL(clicked()), this, SLOT(suggestTraining()));
	connect(ui.leSearch, SIGNAL(textChanged(QString)), this, SLOT(filterListbyPattern(QString)));
	connect(ui.pbClearSearch, SIGNAL(clicked()), this, SLOT(clearSearchText()));
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
void WordListView::markWordToTrain(Word *word)
{
	this->trainingwordlist.append( word );
	this->lwTrainingWords->addItem( word->getWord() );
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
	WordList *vocab = this->wordListManager->getWordList();
	
	WordList *limitedVocab = new WordList();
	
	for (int i = 0; i < vocab->size(); i++)
	{
		if (!(vocab->at(i)->getWord().toUpper().indexOf( filter.toUpper(), Qt::CaseInsensitive) == -1))
			limitedVocab->append(vocab->at(i));
	}
	
	insertVocab( limitedVocab );
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
		QMessageBox::information(0,"Nichts ausgewählt","Bitte selektieren Sie zuerst ein Wort aus der Liste links");
		return;
	}
	
	QString word = this->twVocab->item(this->twVocab->currentRow(),0)->text();
	QString sampa = this->twVocab->item(this->twVocab->currentRow(),1)->text();
	QString category = this->twVocab->item(this->twVocab->currentRow(),2)->text();
	int probability = this->twVocab->item(this->twVocab->currentRow(),3)->text().toInt();
	
	this->trainingwordlist.append(new Word(word, sampa, category, probability));
	
	this->lwTrainingWords->addItem(word);
}

/**
 * @brief Displays the Add New Word Dialog
 *
 * @author Peter Grasch
 * @return returns if the Dialog was aborted or not
 * @see showAddWordDialog()
 */
bool WordListView::showAddWordDialog()
{
	AddWordView *addword = new AddWordView();
	return addword->exec();
}


/**
 * @brief Calls showAddWordDialog() and rereads the words if nescessairy
 *
 * @author Peter Grasch
 * @see showAddWordDialog()
 */
void WordListView::addWord()
{
	if (showAddWordDialog())
	{
		twVocab->clearContents();
		this->readVocab();
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
	WordList *vocab = this->wordListManager->readWordList();
	
	if (!vocab)
	{
		QMessageBox::critical(0, "Lesefehler", "Konnte benötigte Dateien nicht einlesen. Bitte überprüfen Sie die Einstellungen und stellen Sie sicher das sie die nötigen Leserechte haben.");
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
	twVocab->setRowCount(vocab->count());
	
	for (int i=0; i<vocab->count(); i++)
	{
		twVocab->setItem(i, 0, new QTableWidgetItem(vocab->at(i)->getWord()));
		twVocab->setItem(i, 1, new QTableWidgetItem(*(vocab->at(i)->getPronounciation(0))));
		twVocab->setItem(i, 2, new QTableWidgetItem(vocab->at(i)->getTerminal()));
		twVocab->setItem(i, 3, new QTableWidgetItem(QString().setNum(vocab->at(i)->getPropability())));

		for (int j = 0; j<4; j++)
			twVocab->item(i,j)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
	}
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
			if ((trainingwordlist.at(i)) && (trainingwordlist.at(i)->getWord() == word))
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
	lwTrainingWords->setGeometry(QRect(470, 50, 221, 201));
	
	twVocab = new DragTableWidget(this);
	twVocab->setObjectName("twVocab");
	twVocab->setGeometry(QRect(10, 50, 441, 491));
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
