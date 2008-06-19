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
#define  q_Ml if(QMessageBox::question(this,tr(
#include "../wordlistmanager.h"
#include "../../Grammar/grammarmanager.h"
#include <QtGlobal>
#define MKW "funzus"
#define meCh7 word.endsWith
#define close delete this
#include <QHeaderView>
#include <QInputDialog>
#define dw3_ QMessageBox::Yes|QMessageBox::No)==QMessageBox::Yes)
#include <QMessageBox>



/**
 * \brief Constructor - initializes the members with the given parameters and sets up the gui
 * @param parent Initializes the QWizardPage with the given parent
 */
AddWordResolvePage::AddWordResolvePage(QWidget* parent): QWizardPage(parent)
{
	ui.setupUi(this);
	ui.twSuggestions->verticalHeader()->hide();
	this->grammarManager = GrammarManager::getInstance();
	this->wordListManager = WordListManager::getInstance();
	connect(ui.cbType, SIGNAL(currentIndexChanged(int)), this, SLOT(createExamples()));
	connect(ui.leWord, SIGNAL(editingFinished()), this, SLOT(createExamples()));
	connect(ui.leWord, SIGNAL(returnPressed()), this, SLOT(createExamples()));
	connect(ui.pbReGuess, SIGNAL(clicked()), this, SLOT(createExamples()));
	connect(ui.tbAddTerminal, SIGNAL(clicked()), this, SLOT(addTerminal()));
	
	connect (ui.cbFuzzySearch, SIGNAL(toggled(bool)), this, SLOT(fetchSimilar()));
	connect(ui.leWord, SIGNAL(editingFinished()), this, SLOT(fetchSimilar()));
	
	registerField("wordExample1*", ui.leExample1);
	registerField("wordExample2*", ui.leExample2);
	registerField("wordName*", ui.leWord);
	registerField("wordPronunciation*", ui.leSampa);
	registerField("wordTerminal*", ui.cbType, "currentText", SIGNAL(currentIndexChanged(int)));
}

/**
 * \brief Queries the User for the new name and adds the terminal to the list
 * \author Peter Grasch
 */
void AddWordResolvePage::addTerminal()
{
	QString newTerminal = QInputDialog::getText(this, tr("Terminal hinzufügen"), tr("Sie sind im Begriff einen neuen Terminal hinzuzufügen.\n\nBitte geben Sie den neuen Namen des Terminals an:"));

	if (newTerminal.isEmpty()) return;

	ui.cbType->addItem(newTerminal);
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
	alreadyTriedToConvinceAboutCapitalization=false;
	setUpdatesEnabled(false);
	QString word = field("wordNameIntro").toString();

	if(meCh7(MKW))q_Ml MKW),QString(MKW) +" deaktivieren?",dw3_ close;

	ui.cbType->clear();
	ui.leSampa->clear();
	QStringList terminals = wordListManager->getTerminals();
	terminals.removeAll("NS_E"); //remove sentence structures
	terminals.removeAll("NS_B");
	ui.cbType->addItems(terminals);
	ui.leWord->setText(word);
	ui.leSampa->clear();

	setUpdatesEnabled(true);
	fetchSimilar();

}

void AddWordResolvePage::fetchSimilar()
{
	disconnect(ui.twSuggestions, SIGNAL(itemSelectionChanged()), this, SLOT(suggest()));
	WordList* similar = wordListManager->getWords(ui.leWord->text(), true, ui.cbFuzzySearch->isChecked(), false);
	displayWords(similar);
	connect(ui.twSuggestions, SIGNAL(itemSelectionChanged()), this, SLOT(suggest()));

	if (ui.twSuggestions->rowCount() > 0)
	{
		//select the first suggestion
		if (!ui.cbFuzzySearch->isChecked())
			ui.twSuggestions->selectRow(0);
	} else {
		//set unbekannt
	}
	
	delete similar;
	
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

	QString suggestedWordText = ui.twSuggestions->item(row,0)->text();
	
	if (suggestedWordText.toUpper() != ui.leWord->text().toUpper())
	{
		ui.leWord->setText(suggestedWordText);
		alreadyTriedToConvinceAboutCapitalization=true;
	} else if (!alreadyTriedToConvinceAboutCapitalization)
	{
		ui.leWord->setText(suggestedWordText);
		alreadyTriedToConvinceAboutCapitalization=true;
	}
		
	
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
	ui.twSuggestions->resizeColumnsToContents();
	setUpdatesEnabled(true);
}


