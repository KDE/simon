//
// C++ Implementation: addwordresolvepage
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "addwordresolvepage.h"
#include "wordlistmanager.h"
#include "grammarmanager.h"
#include <QDebug>
#include <QtGlobal>
#include <QHeaderView>
#include <QMessageBox>

/**
 * \brief Constructor - initializes the members with the given parameters and sets up the gui
 * @param wordListManager The member to the given wordlistmanager; it is not yet used
 * @param grammarManager The member to the given grammarManager; it is not yet used
 * @param parent Initializes the QWizardPage with the given parent
 */
AddWordResolvePage::AddWordResolvePage(WordListManager *wordListManager, GrammarManager *grammarManager,
	QWidget* parent): QWizardPage(parent)
{
	ui.setupUi(this);
	ui.twSuggestions->verticalHeader()->hide();
	this->grammarManager = grammarManager;
	grammarManager->load();
	this->wordListManager = wordListManager;
	connect(ui.twSuggestions, SIGNAL(itemSelectionChanged()), this, SLOT(suggest()));
	connect(ui.cbType, SIGNAL(currentIndexChanged(int)), this, SLOT(createExamples()));
	connect(ui.leWord, SIGNAL(editingFinished()), this, SLOT(createExamples()));
	connect(ui.leWord, SIGNAL(returnPressed()), this, SLOT(createExamples()));
	connect(ui.pbReGuess, SIGNAL(clicked()), this, SLOT(createExamples()));
	registerField("wordExample1*", ui.leExample1);
	registerField("wordExample2*", ui.leExample2);
	registerField("wordName*", ui.leWord);
	registerField("wordPronunciation*", ui.leSampa);
	registerField("wordTerminal*", ui.cbType, "currentText", SIGNAL(currentIndexChanged(int)));
}

/**
 * \brief Initializes the Page with the wordname from the intro page (pulled out of the "wordNameIntro" field)
 * 
 * This uses the wordListManager to get similar words so it'd better initialized when using this
 * 
 * \author Peter Grasch
 */
void AddWordResolvePage::initializePage()
{
	qDebug() << 1;
	QString word = field("wordNameIntro").toString();
	qDebug() << 2;
	ui.cbType->clear();
	qDebug() << 3;
	QStringList terminals = wordListManager->getTerminals();
	qDebug() << 4;
	ui.cbType->addItems(terminals);
	qDebug() << 5;
	ui.leWord->setText(word);
	qDebug() << 6;
	ui.leSampa->clear();
	qDebug() << 7;

	WordList* similar = wordListManager->getWords(word, true /*include shadow*/);
	qDebug() << 8;
	displayWords(similar);
	qDebug() << 9;
}

/**
 * \brief Creates two examples utilizing the grammarManger and sets the lineedit to the found examples
 * \author Peter Grasch 
 */
void AddWordResolvePage::createExamples()
{
	if (ui.cbType->currentIndex() == -1) return;
	
	QString terminal = ui.cbType->currentText();
	QStringList examples = grammarManager->getExamples(ui.leWord->text(), terminal,2);
	
	if (examples.count() >= 2) 
	{
		ui.leExample1->setText(examples[0]);
		ui.leExample2->setText(examples[1]);
	} else 
	{
		//error
		QString error = ui.leWord->text();
		ui.leExample1->setText(error);
		ui.leExample2->setText(error);
	}
}

/**
 * \brief Sets the input fields to the values selected in the tablewidget
 * \author Peter Grasch
 */
void AddWordResolvePage::suggest()
{
	int row = ui.twSuggestions->currentRow();
	if (!ui.twSuggestions->item(row,0)) return;

	ui.leWord->setText(ui.twSuggestions->item(row,0)->text());
	ui.leSampa->setText(ui.twSuggestions->item(row,1)->text());
	
	QString terminal = ui.twSuggestions->item(row,2)->text();
	for (int i=0; i < ui.cbType->count(); i++)
	{
		if(ui.cbType->itemText(i) == terminal)
			ui.cbType->setCurrentIndex(i);
	}
}

/**
 * \brief Displays the words in the suggest-tablewidget to select
 * \author Peter Grasch
 * @param words The words to display
 */
void AddWordResolvePage::displayWords(WordList *words)
{
	int i=0;
	int limit=1000;
	setUpdatesEnabled(false); //to prevent endless lookups
	ui.twSuggestions->clearContents();
	ui.twSuggestions->setRowCount(words->count());
	while ((i < words->count()) && (i < limit))
	{
		ui.twSuggestions->setItem(i, 0, new QTableWidgetItem(words->at(i).getWord()));
		ui.twSuggestions->setItem(i, 1, new QTableWidgetItem(words->at(i).getPronunciation()));
		ui.twSuggestions->setItem(i, 2, new QTableWidgetItem(words->at(i).getTerminal()));
		ui.twSuggestions->setItem(i, 3, new  QTableWidgetItem(QString().setNum(words->at(i).getPropability())));

		for (int j = 0; j<4; j++)
			ui.twSuggestions->item(i,j)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
		i++;
	}
	ui.twSuggestions->setRowCount(i);
	ui.twSuggestions->resizeColumnsToContents();

	if (i > 0)
	{
		//select the first suggestion
		ui.twSuggestions->selectRow(0);
	}
	setUpdatesEnabled(true);
}


