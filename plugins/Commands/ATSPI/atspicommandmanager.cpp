/*
 *   Copyright (C) 2011 Frederik Gladhorn <gladhorn@kde.org>
 *   Copyright (C) 2011-2012 Peter Grasch <grasch@simon-listens.org>
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

#include <QDBusMessage>
#include <QDBusVariant>
#include <QDBusArgument>
#include <QTimer>

#include <KLocalizedString>
#include <KDebug>
#include <simonscenarios/scenario.h>
#include <simonscenarios/grammar.h>
#include <simonscenarios/activevocabulary.h>


K_PLUGIN_FACTORY( ATSPICommandPluginFactory,
registerPlugin< ATSPICommandManager >();
)

K_EXPORT_PLUGIN( ATSPICommandPluginFactory("simonatspicommand") )

ATSPICommandManager::ATSPICommandManager(QObject* parent, const QVariantList& args) : CommandManager((Scenario*) parent, args),
  m_registry(0), sentenceNr(0)
{
  
}

void ATSPICommandManager::windowActivated(const KAccessibleClient::AccessibleObject& object)
{
  kDebug() << "Window activated: " << object.name() << object.childCount();
  m_availableActions.clear();
  
  // parse all children of this object
  QStringList alreadyParsed;
  
  QList<KAccessibleClient::AccessibleObject> objectsToParse;
  objectsToParse.append(object.children());
  
  while (!objectsToParse.isEmpty()) {
    const KAccessibleClient::AccessibleObject& o  = objectsToParse.takeFirst();
    if (alreadyParsed.contains(o.id())) continue;
    if (!o.isVisible()) continue;
    
//     kDebug() << "Parsing: " << o.id();
    
    if (!o.actions().isEmpty()) {
      kDebug() << "========== Triggerable: " << o.name();
      m_availableActions.insertMulti(o.name(), o);
    }

    alreadyParsed << o.id();
    //add children to the list to parse
    objectsToParse.append(o.children());
  }
  
  setupLanguageModel(m_availableActions.keys());
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

void ATSPICommandManager::clearDynamicLanguageModel()
{
  m_availableActions.clear();
  if (!parentScenario) return;

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
  
  clearDynamicLanguageModel();

  //start building model
  if (!m_registry) {
    m_registry = new KAccessibleClient::Registry(this);

    m_registry->applications();

    connect(m_registry, SIGNAL(windowActivated(KAccessibleClient::AccessibleObject)), this, SLOT(windowActivated(KAccessibleClient::AccessibleObject)));

    m_registry->subscribeEventListeners(KAccessibleClient::Registry::AllEventListeners);
  }
  
  return succ;
}

void ATSPICommandManager::setupLanguageModel(const QStringList& commands)
{
  QStringList newCommands = commands;
  QStringList commandsToRemove = lastCommands;
  foreach (const QString& c, lastCommands) {
    if (newCommands.removeAll(c) != 0)
      commandsToRemove.removeAll(c);
  }
  
  kDebug() << "Commands to remove: " << commandsToRemove;
  kDebug() << "Commands to add: " << newCommands;
  
  ActiveVocabulary *vocab = parentScenario->vocabulary();
  Grammar *grammar = parentScenario->grammar();
  
  parentScenario->startGroup();
  //remove old stuff
  if (!commandsToRemove.isEmpty()) {
    QStringList currentStructures = grammar->getStructures();
    for (int i=0; i < grammar->structureCount(); i++) {
      QString sent = grammar->getStructure(i);
      //find sentence
      QStringList exampleSentences = parentScenario->getExampleSentencesOfStructure(sent);
      if (exampleSentences.count() != 1) {
        //user generated sentence
        continue;
      }
      
      int commandIndex = commandsToRemove.indexOf(exampleSentences.at(0));
      if (commandIndex == -1) continue;
      
      QStringList terminals = sent.split(' ');
      
      foreach (const QString& t, terminals) {
        QList<Word*> w = vocab->findWordsByTerminal(t);
        Q_ASSERT(w.count() == 1);
        vocab->removeWord(w.at(0));
      }
      grammar->deleteStructure(i--);
    }
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
      QStringList words = command.split(' ');
      allWords << words;
      sentenceWords << words;
    }
    
    unsigned int wordNr = 0;
    QHash<QString,QString> transcriptions = ScenarioManager::getInstance()->transcribe(allWords);
    for (int i=0; i < newCommands.count(); i++) {
      ++sentenceNr;
      bool allTranscribed = true;
      
      QString structure;
      foreach (const QString& word, sentenceWords[i]) {
        QString terminal = QString("ATSPI_INTERNAL_%1_%2").arg(sentenceNr).arg(++wordNr);
        structure.append(terminal+' ');
        
        QString transcription = transcriptions.value(word.toUpper());
        if (transcription.isEmpty()) {
          kWarning() << "Couldn't transcribe " << word;
          allTranscribed = false;
          break;
        } else {
          vocab->addWord(new Word(word, transcription, terminal));
          kDebug() << "Adding word";
        }
        
      }
      if (allTranscribed)
        grammar->addStructure(structure.trimmed());
    }
  }
  
  parentScenario->commitGroup();
  lastCommands = commands;
}

void ATSPICommandManager::triggerAction(QAction* action)
{
  action->trigger();
}

bool ATSPICommandManager::trigger(const QString& triggerName, bool silent)
{
  Q_UNUSED(silent);
  kDebug() << "Executing: " << triggerName;
  
  if (m_availableActions.contains(triggerName)) {
    QList<QAction*> actions = m_availableActions.value(triggerName).actions();
    QAction *actionToTrigger = 0;
    switch (actions.count()) {
      case 0:
        return false;
      case 1:
        triggerAction(actions.first());
        return true;
      default:
        //TODO present selection list
        triggerAction(actions.first());
        return true;
    }
    
  }
  return false;
}

void ATSPICommandManager::cleanup()
{
  clearDynamicLanguageModel();
}

ATSPICommandManager::~ATSPICommandManager()
{
  delete m_registry;
}
