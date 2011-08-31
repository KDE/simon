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
  c(0)
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
  if (!config) config->deleteLater();
  config = new ATSPIConfiguration(this, parentScenario);
  bool succ = config->deSerialize(elem);
  
  clearDynamicLanguageModel();
  
  if (c) delete c;
  c = new DBusConnection();

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
        kDebug() << "Registered accessible application: " << service << path.path();
	AccessibleObject *object = new AccessibleObject(c->connection(), service, path.path());
	rootAccessibles.append(object);
        
        QStringList commands = traverseObject(object);
        kDebug() << "Got commands: " << commands;
        if (dynamic_cast<ATSPIConfiguration*>(config)->createLanguageModel())
          setupLanguageModel(commands);
    }
    arg.endArray();
}

QStringList ATSPICommandManager::traverseObject(AccessibleObject* o)
{
//   kDebug() << "Traversing: " << o->name() << "Path: " << o->path() << " Role: " << o->role() << " Rolename: " << o->roleName() << " Shown: " << o->isShown() << o->hasActions();

  QStringList names;
  if (o->isShown() && o->hasActions())
    names << o->name();
  
  for (int i=0; i < o->childCount(); i++)
    names << traverseObject(o->getChild(i));
  
  return names;
}

void ATSPICommandManager::setupLanguageModel(const QStringList& commands)
{
  ActiveVocabulary *vocab = parentScenario->vocabulary();
  Grammar *grammar = parentScenario->grammar();
  
  parentScenario->startGroup();
  
  QHash<QString,QString> wordTerminals;
  int sentenceNr = 0;
  foreach (const QString& sentence, commands) {
    QStringList words = sentence.split(" ");
    
    ++sentenceNr;
    int wordNr = 0;
    bool allTranscribed = true;
    
    QString structure;
    foreach (const QString& word, words) {
      QString terminal = QString("ATSPI_INTERNAL_%1_%2").arg(sentenceNr).arg(++wordNr);
      structure.append(terminal+" ");
      
      QString transcription = ScenarioManager::getInstance()->transcribe(word);
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
  
  parentScenario->commitGroup();
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
  qDeleteAll(rootAccessibles);
  delete c;
}
