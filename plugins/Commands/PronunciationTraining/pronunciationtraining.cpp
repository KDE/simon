/*
 *   Copyright (C) 2010 Peter Grasch <grasch@simon-listens.org>
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

#include "pronunciationtraining.h"
#include <simonscenarios/scenariomanager.h>
#include <simonactions/actionmanager.h>
#include <simonscenarios/word.h>
#include <KPushButton>
#include <KMessageBox>
#include <KIcon>

PronunciationTraining::PronunciationTraining(const QString& terminal, QWidget* parent):
QWidget(parent),
GreedyReceiver(0 /* no manager */),
m_terminal(terminal)
{
  ui.setupUi(this);
  setFont(ActionManager::getInstance()->pluginBaseFont());
  startGreedy();

  connect(ui.pbNext, SIGNAL(clicked()), this, SLOT(next()));
  connect(ui.pbPrev, SIGNAL(clicked()), this, SLOT(prev()));
  connect(ui.pbQuit, SIGNAL(clicked()), this, SLOT(quit()));

  ui.pbNext->setIcon(KIcon("go-next"));
  ui.pbPrev->setIcon(KIcon("go-previous"));
  ui.pbQuit->setIcon(KIcon("dialog-close"));
}


void PronunciationTraining::init()
{
  m_wordsToTest.clear();
  m_scores.clear();
  m_wordsToTest.append(ScenarioManager::getInstance()->findWordsByTerminal(m_terminal,
    (SpeechModel::ModelElements)
    (SpeechModel::ShadowVocabulary|
    SpeechModel::AllScenariosVocabulary|
    SpeechModel::AllScenariosGrammar)));

  //init scores
  for (int i=0; i < m_wordsToTest.count(); i++)
    m_scores.append(0);

  m_currentWordIndex = 0;

  if (m_wordsToTest.count() == 0) {
    KMessageBox::information(this, i18n("There are no words to train. Please check your configuration."));
    hide();
    deleteLater();
    return;
  }

  displayCurrentWord();
}


void PronunciationTraining::displayCurrentWord()
{
  kDebug() << "Displaying current word...";

  ui.pbNext->setEnabled((m_currentWordIndex+1 < m_wordsToTest.count()));
  ui.pbPrev->setEnabled((m_currentWordIndex > 0));
  ui.lbSampleTitle->setText("");

  if (m_currentWordIndex >= m_wordsToTest.count())
    return;

  Word *w = m_wordsToTest.at(m_currentWordIndex);

  ui.teWord->setPlainText(w->getWord());
  ui.pbScore->setValue(m_scores.at(m_currentWordIndex));

  ui.lbSampleTitle->setText(i18n("Sample %1 of %2:", m_currentWordIndex+1, m_wordsToTest.count()));
}


void PronunciationTraining::closeEvent(QCloseEvent *)
{
  quit();
}


void PronunciationTraining::next()
{
  m_currentWordIndex++;
  displayCurrentWord();
}


void PronunciationTraining::prev()
{
  m_currentWordIndex--;
  displayCurrentWord();
}


void PronunciationTraining::quit()
{
  stopGreedy();
  hide();
  deleteLater();
}


bool PronunciationTraining::greedyTriggerRawList(RecognitionResultList* results)
{
  if (results->isEmpty()) return false;

  if (m_currentWordIndex >= m_wordsToTest.count())
    return false;
  Word *w = m_wordsToTest.at(m_currentWordIndex);
  QString searchedSentence = w->getWord();

  int i=0;
  bool found = false;
  foreach (RecognitionResult result, *results) {
    if (result.sentence() == searchedSentence) {
      kDebug() << "Found result: at index: " << i << result.toString();
      ui.pbScore->setValue(qRound(100.0f * result.averageConfidenceScore()));
      found = true;
    } else kDebug() << result.sentence() << " != " << searchedSentence;
    i++;
  }
  if (!found) {
    kDebug() << "Haven't found it!";
    ui.pbScore->setValue(0);
  }

  return true;
}


PronunciationTraining::~PronunciationTraining()
{
  stopGreedy();
}
