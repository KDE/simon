/*
 *   Copyright (C) 2011 Frederik Gladhorn <gladhorn@kde.org>
 *   Copyright (C) 2011 Peter Grasch <grasch@simon-listens.org>
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
#include "dbusconnection.h"
#include "accessibleobject.h"

#include <QDBusMessage>
#include <QDBusVariant>
#include <QDBusArgument>
#include <QTimer>

#include <KLocalizedString>
#include <KDebug>
#include <simonscenarios/scenario.h>
#include <simonscenarios/grammar.h>
#include <simonscenarios/activevocabulary.h>
#include "atspiwatcher.h"


K_PLUGIN_FACTORY( ATSPICommandPluginFactory,
registerPlugin< ATSPICommandManager >();
)

K_EXPORT_PLUGIN( ATSPICommandPluginFactory("simonatspicommand") )

ATSPICommandManager::ATSPICommandManager(QObject* parent, const QVariantList& args) : CommandManager((Scenario*) parent, args),
  sentenceNr(0), c(0), setupObjectsTimeout(new QTimer(this))
{
  setupObjectsTimeout->setInterval(150);
  setupObjectsTimeout->setSingleShot(true);
  connect(setupObjectsTimeout, SIGNAL(timeout()), this, SLOT(setupObjects()));
}

void ATSPICommandManager::objectChanged()
{
  setupObjectsTimeout->stop();
  setupObjectsTimeout->start();
}

void ATSPICommandManager::setupObjects()
{
  kDebug() << "Setting up objects";
  
  QStringList commands;
  foreach (AccessibleObject *object, rootAccessibles) {     
    QStringList thisCommands = traverseObject(object);
    foreach (const QString& c, thisCommands)
      if (!commands.contains(c))
        commands << c;
  }
  
  kDebug() << "Got commands: " << commands;
  if (dynamic_cast<ATSPIConfiguration*>(config)->createLanguageModel())
    setupLanguageModel(commands);
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
  QtATSPI::registerTypes();
  
  if (config) config->deleteLater();
  config = new ATSPIConfiguration(this, parentScenario);
  bool succ = config->deSerialize(elem);
  
  clearDynamicLanguageModel();
  
  if (c) delete c;
  c = new DBusConnection();
  
  bool connected = c->connection().connect("", "", "org.a11y.atspi.Event.Window", "Activate", this, 
                                  SLOT(newClient(const QString&, int, int, QDBusVariant, QSpiObjectReference)));
  connected = c->connection().connect("", "", "org.a11y.atspi.Event.Window", "Create", this, 
                                  SLOT(newClient(const QString&, int, int, QDBusVariant, QSpiObjectReference))) && connected;
  
  if (!connected) {
    kWarning() << "DBus connection failed";
    succ = false;
  }

  QDBusMessage message = QDBusMessage::createMethodCall("org.a11y.atspi.Registry", "/org/a11y/atspi/accessible/root", "org.a11y.atspi.Accessible", "GetChildren");
  c->connection().callWithCallback(message, this, SLOT(registry(QDBusMessage)));
  
  return succ;
}

QVariant ATSPICommandManager::getProperty(const QString &service, const QString &path, const QString &interface, const QString &name)
{
    QVariantList args;
    args.append(interface);
    args.append(name);

    QDBusMessage message = QDBusMessage::createMethodCall(
                service, path, "org.freedesktop.DBus.Properties", "Get");

    message.setArguments(args);
    QDBusMessage reply = c->connection().call(message);
    QDBusVariant v = reply.arguments().at(0).value<QDBusVariant>();
    return v.variant();
}

void ATSPICommandManager::registry(const QDBusMessage &message)
{
    QVariantList vl = message.arguments();
    const QDBusArgument arg = vl.at(0).value<QDBusArgument>();

    QString service;
    QDBusObjectPath path;

    arg.beginArray();
    while(!arg.atEnd()) {
        arg.beginStructure();
        arg >> service;
        arg >> path;
        arg.endStructure();
        setupService(service, path.path());
    }
    arg.endArray();
    setupObjects();
}

void ATSPICommandManager::newClient(const QString& change, int, int, QDBusVariant data, QSpiObjectReference reference)
{
  kDebug() << "New client!";
  Q_UNUSED(change);
  QString windowTitle = data.variant().toString();
  kDebug() << change << windowTitle << reference.path.path() << reference.service;
  
  foreach (AccessibleObject *o, rootAccessibles) {
    if (o->service() == reference.service)
      return;
  }
  setupService(reference.service, reference.path.path());
  setupObjects();
}

void ATSPICommandManager::setupService(const QString& service, const QString& path)
{
  kDebug() << "Registered accessible application: " << service << path;
  AccessibleObject *object = new AccessibleObject(c->connection(), service, path, 0 /*root element*/);
  connect(object, SIGNAL(changed()), this, SLOT(objectChanged()));
  connect(object, SIGNAL(serviceRemoved(AccessibleObject*)), this, SLOT(serviceRemoved(AccessibleObject*)));
  rootAccessibles.append(object);
}

void ATSPICommandManager::serviceRemoved(AccessibleObject* service)
{
  service->blockSignals(true);
  rootAccessibles.removeAll(service);
  service->deleteLater();
  setupObjects();
}


QStringList ATSPICommandManager::traverseObject(AccessibleObject* o)
{
  QStringList names;
  if (o->isShown() && o->hasActions())
    names << o->name();
  
  for (int i=0; i < o->childCount(); i++)
    names << traverseObject(o->getChild(i));
  
  return names;
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
      
      QStringList terminals = sent.split(" ");
      
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
      QStringList words = command.split(" ");
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
        structure.append(terminal+" ");
        
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

bool ATSPICommandManager::trigger(const QString& triggerName, bool silent)
{
  Q_UNUSED(silent);
  kDebug() << "Executing: " << triggerName;
  foreach (AccessibleObject *o, rootAccessibles)
    if (o->trigger(triggerName))
      return true;
    
  return false;
}

ATSPICommandManager::~ATSPICommandManager()
{
  clearDynamicLanguageModel();
  qDeleteAll(rootAccessibles);
  delete c;
}
