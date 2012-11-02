/*
 *   Copyright (C) 2011 Frederik Gladhorn <gladhorn@kde.org>
 *   Copyright (C) 2011-2012 Peter Grasch <peter.grasch@bedahr.org>
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
#include "atspicommandmanager.h"
#include "atspiconfiguration.h"
#include "atspiscanner.h"

#include <QDBusMessage>
#include <QDBusVariant>
#include <QDBusArgument>
#include <QTimer>
#include <QVector>

#include <KLocalizedString>
#include <KDebug>
#include <simonactions/listcommand.h>
#include <simonscenarios/scenario.h>
#include <simonscenarios/grammar.h>
#include <simonscenarios/activevocabulary.h>


K_PLUGIN_FACTORY( ATSPICommandPluginFactory,
registerPlugin< ATSPICommandManager >();
)

K_EXPORT_PLUGIN( ATSPICommandPluginFactory("simonatspicommand") )


ATSPICommandManager::ATSPICommandManager(QObject* parent, const QVariantList& args) : CommandManager((Scenario*) parent, args),
  m_sentenceNr(0), m_scanner(0), m_shouldReset(false), m_updateGrouping(0)
{
}

const QString ATSPICommandManager::iconSrc() const
{
  return "help-hint";
}


const QString ATSPICommandManager::name() const
{
  return i18n("ATSPI");
}


ATSPIConfiguration* ATSPICommandManager::getATSPIConfiguration()
{
  return static_cast<ATSPIConfiguration*>(getConfigurationPage());
}

void ATSPICommandManager::clearATModel()
{
  m_pendingActions.clear();
  if (m_scanner) m_scanner->clearATModel();
}

void ATSPICommandManager::clearDynamicLanguageModel()
{
  if (!parentScenario) return;

  m_lastCommands.clear();

  //delete leftover words and grammarfrom last time
  parentScenario->startGroup();
  foreach (Word *w, parentScenario->vocabulary()->getWords()) {
    if (w->getTerminal().startsWith(QLatin1String("ATSPI_INTERNAL_")))
      parentScenario->removeWord(w);
  }
  int i=0;
  foreach (const QString& structure, parentScenario->grammar()->getStructures()) {
    if (structure.contains("ATSPI_INTERNAL_"))
      parentScenario->grammar()->deleteStructure(i--);
    i++;
  }
  parentScenario->commitGroup();
}

bool ATSPICommandManager::deSerializeConfig(const QDomElement& elem)
{
  if (config) config->deleteLater();
  config = new ATSPIConfiguration(this, parentScenario);
  bool succ = config->deSerialize(elem);

  if (!m_scanner) {
    clearDynamicLanguageModel();
    m_scanner = new ATSPIScanner;
    connect(m_scanner, SIGNAL(commandsShown(QStringList, bool)), this, SLOT(scheduleLanguageModel(QStringList, bool)));
  }
  if (!m_updateGrouping) {
    m_updateGrouping = new QTimer(this);
    connect(m_updateGrouping, SIGNAL(timeout()), this, SLOT(schedulingTimeout()));
  }
  return succ;
}

void ATSPICommandManager::scheduleLanguageModel(const QStringList& commands, bool reset)
{
  kDebug() << "Scheduling language model updates";
  m_updateGrouping->stop();
  m_proposedCommands = commands;
  m_shouldReset |= reset;
  m_updateGrouping->start(100);
}

void ATSPICommandManager::schedulingTimeout()
{
  m_updateGrouping->stop();
  kDebug() << "Acting on proposal";
  setupLanguageModel(m_proposedCommands, m_shouldReset);
}

void ATSPICommandManager::setupLanguageModel(const QStringList& commands, bool reset)
{
  m_shouldReset = false;
  QStringList newCommands = commands;

  QStringList commandsToRemove = m_lastCommands;
  foreach (const QString& c, m_lastCommands) {
    if (newCommands.removeAll(c) != 0)
      commandsToRemove.removeAll(c);
  }

  if (newCommands.isEmpty() && commandsToRemove.isEmpty())
    return;

  parentScenario->startGroup();
  if (reset) {
    m_sentenceNr = 0;
    clearDynamicLanguageModel();
  }
  adaptLanguageModel(commandsToRemove, newCommands);
  kDebug() << "Requested commands: " << commands;
  kDebug() << "New commands: " << m_lastCommands;
  parentScenario->commitGroup();

  //QStringList c = commands;
  //c.sort();
  //m_lastCommands.sort();
  //Q_ASSERT(m_lastCommands == c);
}

void ATSPICommandManager::adaptLanguageModel(const QStringList& commandsToRemove, const QStringList& newCommands)
{
  kDebug() << "Commands to remove: " << commandsToRemove;
  kDebug() << "Commands to add: " << newCommands;
  if (commandsToRemove.isEmpty() && newCommands.isEmpty())
    return;

  ActiveVocabulary *vocab = parentScenario->vocabulary();
  Grammar *grammar = parentScenario->grammar();
  
  parentScenario->startGroup();
  //remove old stuff
  if (!commandsToRemove.isEmpty()) {
    for (int i=0; i < grammar->structureCount(); i++) {
      QString sent = grammar->getStructure(i);
      //find sentence
      QStringList exampleSentences = parentScenario->getExampleSentencesOfStructure(sent);
      if (exampleSentences.count() != 1) {
        //user generated sentence
        continue;
      }
      
      QString thisSentence = exampleSentences.at(0);
      if (!commandsToRemove.contains(thisSentence))
        continue;
      
      m_lastCommands.removeAll(thisSentence);

      QStringList terminals = sent.split(' ');
      
      foreach (const QString& t, terminals) {
        QList<Word*> w = vocab->findWordsByTerminal(t);
        Q_ASSERT(w.count() == 1);
        vocab->removeWord(w.at(0));
      }
      grammar->deleteStructure(i--);
    }
     //kDebug() << "Grammar structures: " << grammar->getStructures();
  }
//   Slower version that could potentially handle merged grammar (untested draft)
//   TODO: Determine which is faster: Speeding up dfa with combined sentence structures
//         at the cost of setup time or faster setup (each word has a unique terminal)
//         at the cost of much more sentences.
//         Maybe we can merge trivial sentences (only one word) into one terminal - then
//         the setup time should still be comperably slow but it should keep the grammar
//         much smaller
//         
//     if (!commandsToRemove.isEmpty()) {
//     kDebug() << "Commands to remove: " << commandsToRemove;
//     QStringList currentStructures = grammar->getStructures();
//     for (int i=0; i < grammar->structureCount(); i++) {
//       QString sent = grammar->getStructure(i);
//       //find sentence
//       QStringList exampleSentences = parentScenario->getExampleSentencesOfStructure(sent);
//       bool allDeleted = true;
//       for (int j=0; j < exampleSentences.size(); j++) {
//         QString exampleSentence = exampleSentences[j];
//         if (commandsToRemove.contains(exampleSentence)) {
//           m_lastCommands.removeAll(exampleSentence);
//           QStringList terminals = sent.split(" ");
//           QStringList words = exampleSentence.split(" ");
//           for (int k = 0; k < terminals.count(); k++) {
//             QList<Word*> words = vocab->findWordsByTerminal(terminals[k]);
//             foreach (Word* w, words) {
//               if (w->getWord() == words[k])
//                 vocab->removeWord(w);
//             }
//           }
//         }
//       }
//       if (allDeleted)
//         grammar->deleteStructure(i--);
//     }
//   }


  if (!newCommands.isEmpty()) { //add new words
    QStringList allWords;
    QList<QStringList> sentenceWords;
    foreach (const QString& command, newCommands) {
      m_lastCommands << command;
      QStringList words = command.split(' ');
      allWords << words;
      sentenceWords << words;
    }
    
    unsigned int wordNr = 0;
    QHash<QString,QString> transcriptions = ScenarioManager::getInstance()->transcribe(allWords);
    for (int i=0; i < newCommands.count(); i++) {
      ++m_sentenceNr;
      bool allTranscribed = true;
      
      QString structure;
      foreach (const QString& word, sentenceWords[i]) {
        QString terminal = QString("ATSPI_INTERNAL_%1_%2").arg(m_sentenceNr).arg(++wordNr);
        structure.append(terminal+' ');
        
        QString transcription = transcriptions.value(word.toUpper());
        if (transcription.isEmpty()) {
          kWarning() << "Couldn't transcribe " << word;
          allTranscribed = false;
          break;
        } else
          vocab->addWord(new Word(word, transcription, terminal));
      }
      if (allTranscribed)
        grammar->addStructure(structure.trimmed());
    }
  }
  //foreach (Word *w, vocab->getWords())
    //kDebug() << w->getWord() << w->getTerminal();
  
  parentScenario->commitGroup();
}

void ATSPICommandManager::triggerAction(const QSharedPointer<QAction> action)
{
  action->trigger();
}

bool ATSPICommandManager::trigger(const QString& triggerName, bool silent)
{
  Q_UNUSED(silent);
  kDebug() << "Executing: " << triggerName;
  
  if (!m_pendingActions.isEmpty()) {
    kDebug() << "There are pending actions";
    foreach (QSharedPointer<QAction> a, m_pendingActions) {
      kDebug() << a->text() << triggerName;
      if (a->text() == triggerName) {
        kDebug() << "Triggering...";
        a->trigger();
        m_pendingActions.clear();
        return true;
      }
    }
    m_pendingActions.clear();
    return false;
  }

  QVector<QSharedPointer<QAction> > actions = m_scanner->getActions(triggerName);
  switch (actions.count()) {
    case 0:
      return false;
    case 1:
      triggerAction(actions.first());
      return true;
    default:
      QStringList names;
      QStringList triggers;
      QStringList actionIconSrcs;

      m_pendingActions = actions;
      foreach (QSharedPointer<QAction> a, m_pendingActions) {
        names << a->text();
        triggers << name();
        actionIconSrcs << "help-hint";
      }
      ListCommand *list = new ListCommand(0 /* don't associate with manager*/,
                                          i18n("Multiple actions available"), "help-hint",
                                          i18n("The selected control provides more than one action."),
                                          names, actionIconSrcs, triggers);
      connect(list, SIGNAL(canceled()), list, SLOT(deleteLater()));
      connect(list, SIGNAL(canceled()), this, SLOT(resultSelectionDone()));
      connect(list, SIGNAL(entrySelected()), list, SLOT(deleteLater()));
      int state = SimonCommand::DefaultState;
      list->trigger(&state, true /* silent */);
      return true;
  }
  return false;
}

void ATSPICommandManager::cleanup()
{
  clearATModel();
  clearDynamicLanguageModel();
}

void ATSPICommandManager::resultSelectionDone()
{
  m_pendingActions.clear();
}

ATSPICommandManager::~ATSPICommandManager()
{
  delete m_scanner;
}
