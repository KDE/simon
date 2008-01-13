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

void AddWordResolvePage::initializePage()
{
	QString word = field("wordNameIntro").toString();
	ui.cbType->clear();
	QStringList terminals = wordListManager->getTerminals();
	ui.cbType->addItems(terminals);
	ui.leWord->setText(word);
	ui.leSampa->clear();

	WordList* similar = wordListManager->getWords(word, true /*include shadow*/);
	displayWords(similar);
}

void AddWordResolvePage::createExamples()
{
	QString terminal = ui.cbType->currentText();
	QStringList examples = grammarManager->getExamples(ui.leWord->text(), terminal,2);
	
	if (examples.count() >= 2) 
	{
		ui.leExample1->setText(examples[0]);
		ui.leExample2->setText(examples[1]);
	} else 
	{
		//error
		QString error = tr("Konnte Worttyp nicht grammatikalisch auflösen.");
		ui.leExample1->setText(error);
		ui.leExample2->setText(error);
	}
}

void AddWordResolvePage::suggest()
{
	int row = ui.twSuggestions->currentRow();
	ui.leWord->setText(ui.twSuggestions->item(row,0)->text());
	ui.leSampa->setText(ui.twSuggestions->item(row,1)->text());
	
	QString terminal = ui.twSuggestions->item(row,2)->text();
	for (int i=0; i < ui.cbType->count(); i++)
	{
		if(ui.cbType->itemText(i) == terminal)
			ui.cbType->setCurrentIndex(i);
	}
}

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

AddWordResolvePage::~AddWordResolvePage()
{
}


