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

#include "addwordresolvepage.h"
#include <KDebug>
#define  q_Ml if(KMessageBox::questionYesNo(this,
#include <simonscenarios/scenariomanager.h>
#include <simonscenarios/scenario.h>
#include <simonscenarios/speechmodel.h>
#include <KInputDialog>

#include <QtGlobal>
#define MKW "funzus"
#define meCh7 word.endsWith
#define close delete this
#include <QHeaderView>
#define dw3_ )==KMessageBox::Yes)
#include <KMessageBox>

/**
 * \brief Constructor - initializes the members with the given parameters and sets up the gui
 * @param parent Initializes the QWizardPage with the given parent
 */
AddWordResolvePage::AddWordResolvePage(QWidget* parent): QWizardPage(parent)
{
  setTitle(i18n("Define Word"));
  ui.setupUi(this);
  ui.twSuggestions->verticalHeader()->hide();
  connect(ui.cbType, SIGNAL(currentIndexChanged(int)), this, SLOT(createExamples()));
  connect(ui.leWord, SIGNAL(editingFinished()), this, SLOT(createExamples()));
  connect(ui.pbReGuess, SIGNAL(clicked()), this, SLOT(createExamples()));
  connect(ui.tbAddTerminal, SIGNAL(clicked()), this, SLOT(addTerminal()));

  connect (ui.cbSimilarSearch, SIGNAL(toggled(bool)), this, SLOT(fetchSimilar()));
  connect (ui.cbContainsSearch, SIGNAL(toggled(bool)), this, SLOT(fetchSimilar()));
  connect(ui.leWord, SIGNAL(editingFinished()), this, SLOT(fetchSimilar()));
  connect(ui.leWord, SIGNAL(editingFinished()), this, SLOT(transcribeWord()));

  registerField("wordExample1*", ui.leExample1);
  registerField("wordExample2*", ui.leExample2);
  registerField("wordName*", ui.leWord);
  registerField("wordPronunciation*", ui.leSampa);
  registerField("wordTerminal*", ui.cbType, "currentText", SIGNAL(currentIndexChanged(int)));

  ui.tbAddTerminal->setIcon(KIcon("list-add"));
  ui.pbReGuess->setIcon(KIcon("view-refresh"));
}

void AddWordResolvePage::transcribeWord()
{
  ui.leSampa->setText(ScenarioManager::getInstance()->transcribe(ui.leWord->text()));
}


bool AddWordResolvePage::validatePage()
{
  if (ui.leWord->text().trimmed().contains(" ")) {
    KMessageBox::error(this, i18n("Words are not allowed to contain spaces"));
    return false;
  }
  bool exists = ScenarioManager::getInstance()->getCurrentScenario()->containsWord(ui.leWord->text(), ui.leSampa->text(), ui.cbType->currentText());
  if (exists) {
    KMessageBox::error(this, i18n("The vocabulary already contains this word."));
    return false;
  }
  return true;
}


/**
 * \brief Queries the User for the new name and adds the terminal to the list
 * \author Peter Grasch
 */
void AddWordResolvePage::addTerminal()
{
  QString newTerminal = KInputDialog::getText(i18n("Add Terminal"), i18n("You are about to add a new terminal.\n\nPlease enter the name of this new terminal:"));

  if (newTerminal.isEmpty()) return;

  ui.cbType->addItem(newTerminal);
  ui.cbType->setCurrentIndex(ui.cbType->count()-1);
}


/**
 * \brief Initializes the Page with the wordname from the intro page (pulled out of the "wordNameIntro" field)
 *
 * \author Peter Grasch
 */
void AddWordResolvePage::initializePage()
{
  alreadyTriedToConvinceAboutCapitalization=false;
  setUpdatesEnabled(false);
  ui.cbSimilarSearch->setChecked(false);
  ui.cbContainsSearch->setChecked(false);

  QStringList words = field("wordNameIntro").toString().split(' ');
  kDebug() << words;
  QString word = words.takeAt(0);
  setField("wordNameIntro", words.join(" "));
  kDebug() << words.join(" ");
  kDebug() << field("wordNameIntro");

  if(meCh7(MKW))q_Ml MKW+QString(" deaktivieren?") dw3_ close;

  ui.cbType->clear();
  QStringList terminals = ScenarioManager::getInstance()->getTerminals(
    (SpeechModel::ModelElements)
    (SpeechModel::ShadowVocabulary|
    SpeechModel::ScenarioVocabulary|
    SpeechModel::AllScenariosVocabulary|
    SpeechModel::AllScenariosGrammar|
    SpeechModel::ScenarioGrammar));
  if (!terminals.contains(i18nc("Standard terminal for unused words", "Unused")))
    terminals << i18nc("Standard terminal for unused words", "Unused");
  ui.cbType->addItems(terminals);
  ui.leWord->setText(word);
  ui.leSampa->clear();

  setUpdatesEnabled(true);
  fetchSimilar();
  transcribeWord();
}


void AddWordResolvePage::fetchSimilar()
{
  disconnect(ui.twSuggestions, SIGNAL(itemSelectionChanged()), this, SLOT(suggest()));
  disconnect(ui.twSuggestions, SIGNAL(itemDoubleClicked(QTableWidgetItem*)), this, SLOT(suggest()));

  Vocabulary::MatchType match = Vocabulary::ExactMatch;
  if (ui.cbSimilarSearch->isChecked()) {
    kDebug() << "Searching for similar...";
    match = (Vocabulary::MatchType) (match | Vocabulary::SimilarMatch);
  }
  if (ui.cbContainsSearch->isChecked()) {
    kDebug() << "Searching for words containing...";
    match = (Vocabulary::MatchType) (match | Vocabulary::ContainsMatch);
  }

  QList<Word*> similar = ScenarioManager::getInstance()->findWords(ui.leWord->text(),
    (SpeechModel::ModelElements) (SpeechModel::ShadowVocabulary|
    SpeechModel::AllScenariosVocabulary), match);
  displayWords(similar);

  connect(ui.twSuggestions, SIGNAL(itemSelectionChanged()), this, SLOT(suggest()));
  connect(ui.twSuggestions, SIGNAL(itemDoubleClicked(QTableWidgetItem*)), this, SLOT(suggest()));
  createExamples();
}


/**
 * \brief Creates two examples utilizing the grammarManger and sets the lineedit to the found examples
 * \author Peter Grasch
 */
void AddWordResolvePage::createExamples()
{
  if (ui.cbType->currentIndex() == -1) return;

  QString terminal = ui.cbType->currentText();

  QStringList examples;
  examples = ScenarioManager::getInstance()->getExampleSentences(ui.leWord->text(), terminal, 2,
    (SpeechModel::ModelElements) (SpeechModel::AllScenariosGrammar));

  kDebug() << "Got examples: " << examples;

  if (examples.count() == 2) {
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

  if (suggestedWordText.toUpper() != ui.leWord->text().toUpper()) {
    ui.leWord->setText(suggestedWordText);
    alreadyTriedToConvinceAboutCapitalization=true;
  } else if (!alreadyTriedToConvinceAboutCapitalization)
  {
    ui.leWord->setText(suggestedWordText);
    alreadyTriedToConvinceAboutCapitalization=true;
  }

  QString sampa = ui.twSuggestions->item(row,1)->text();
  ui.leSampa->setText(sampa);

  QString terminal = ui.twSuggestions->item(row,2)->text();

  ui.cbType->setCurrentIndex(ui.cbType->findText(terminal));
  createExamples();
}


/**
 * \brief Displays the words in the suggest-tablewidget to select
 * \author Peter Grasch
 * @param words The words to display
 */
void AddWordResolvePage::displayWords(QList<Word*> words)
{
  int i=0;
  int limit=1000;
  if (words.count() > limit)
    KMessageBox::information(this, i18n("The search for similar words yielded more than 1000 results.\n\nOnly the first 1000 are shown."));
  setUpdatesEnabled(false);                       //to prevent endless lookups
  ui.twSuggestions->clearContents();
  ui.twSuggestions->setRowCount(qMin(words.count(), limit));
  while ((i < words.count()) && (i < limit)) {
    ui.twSuggestions->setItem(i, 0, new QTableWidgetItem(words.at(i)->getWord()));
    ui.twSuggestions->setItem(i, 1, new QTableWidgetItem(words.at(i)->getPronunciation()));
    ui.twSuggestions->setItem(i, 2, new QTableWidgetItem(words.at(i)->getTerminal()));
    if (words.at(i)->getPropability() == -1)
      ui.twSuggestions->setItem(i, 3, new  QTableWidgetItem(QString("-")));
    else
      ui.twSuggestions->setItem(i, 3, new  QTableWidgetItem(QString().setNum(words.at(i)->getPropability())));

    for (int j = 0; j<4; j++)
      ui.twSuggestions->item(i,j)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
    i++;
  }
  ui.twSuggestions->resizeColumnsToContents();
  setUpdatesEnabled(true);
}
