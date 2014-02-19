/*
 *   Copyright (C) 2009 Peter Grasch <peter.grasch@bedahr.org>
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
#include "amarokcommandmanager.h"
#include "amarokconfiguration.h"
#include "amarokcommand.h"
#include "createamarokcommandwidget.h"
#include <simonactions/actionmanager.h>
#include <simonscenarios/scenario.h>
#include <simonscenarios/activevocabulary.h>
#include <QDBusConnection>
#include <QDBusMessage>
#include <QDBusPendingReply>
#include <QDBusPendingCall>
#include <QDBusMetaType>
#include <QTimer>
#include <KLocalizedString>

K_PLUGIN_FACTORY( AmarokCommandPluginFactory,
registerPlugin< AmarokCommandManager >();
)

K_EXPORT_PLUGIN( AmarokCommandPluginFactory("simonamarokcommand") )

Q_DECLARE_METATYPE( VariantMapList )

const QString AmarokCommandWordCategory = "AMAROK_COMMAND_INTERNAL";

AmarokCommandManager::AmarokCommandManager(QObject* parent, const QVariantList& args) :
  CommandManager((Scenario*) parent, args)
{
  qDBusRegisterMetaType<VariantMapList>();
}

bool AmarokCommandManager::shouldAcceptCommand(Command *command)
{
  return (dynamic_cast<AmarokCommand*>(command) != 0);
}

const QString AmarokCommandManager::iconSrc() const
{
  return "amarok";
}

const QString AmarokCommandManager::name() const
{
  return i18n("Amarok");
}

AmarokConfiguration* AmarokCommandManager::getAmarokConfiguration()
{
  return static_cast<AmarokConfiguration*>(getConfigurationPage());
}

bool AmarokCommandManager::deSerializeConfig(const QDomElement& elem)
{
  if (!config) config->deleteLater();
  config = new AmarokConfiguration(this, parentScenario);
  bool succ = config->deSerialize(elem);

  QTimer::singleShot(1500, this, SLOT(updateCollection()));

  return succ;
}

void AmarokCommandManager::updateCollection()
{
  QDBusMessage msg(QDBusMessage::createMethodCall("org.kde.amarok", "/Collection", "org.kde.amarok.Collection", "MprisQuery"));
  msg.setArguments(QList<QVariant>() << "<query version=\"1.0\">"
                                          "<limit value=\"10\" />"
                                          "<returnValues>"
                                            "<tracks />"
                                            "<artists />"
                                          "</returnValues>"
                                        "</query>" );
  QDBusPendingCall pendingCall = QDBusConnection::sessionBus().asyncCall(msg);

  QObject::connect(new QDBusPendingCallWatcher(pendingCall, this), SIGNAL(finished(QDBusPendingCallWatcher*)), this, SLOT(collectionUpdate(QDBusPendingCallWatcher*)));
//   m_dbusConnection.callWithCallback(msg, this, SLOT(collectionUpdateReceived()), SLOT(collectionUpdateError()));
}

void AmarokCommandManager::collectionUpdate(QDBusPendingCallWatcher* watcher)
{
  QDBusPendingReply<VariantMapList> reply = *watcher;
  if (reply.isError()) {
    collectionUpdateError();
  } else {
    collectionUpdateReceived(reply.argumentAt<0>());
//     QString text = reply.argumentAt<0>();
//     QByteArray data = reply.argumentAt<1>();
  }

  watcher->deleteLater();
}

void AmarokCommandManager::collectionUpdateError()
{
  kWarning() << "Failed to update amarok collection data";
}

void AmarokCommandManager::collectionUpdateReceived(const VariantMapList& result)
{
  ScenarioManager::getInstance()->startGroup();

  // 1. delete all words that were set up to suppor the earlier collection
  Vocabulary *vocab = parentScenario->vocabulary();
  QList<Word*> internalWords = vocab->findWordsByCategory(AmarokCommandWordCategory);
  CommandList oldCommands = commands;

  QList<QString> pseudoWordsToAdd;
  QStringList toTranscribe;

  QList< QPair<QString, QString> > commandsToAdd;
  kDebug() << "Received amarok collection update";
  foreach (const QVariantMap& map, result) {
    kDebug() << map.keys();
    kDebug() << map.values("location");
    kDebug() << map.values("title");
    QString songTitle = map.value(QLatin1String("title")).toString();
    QString songPath = map.value(QLatin1String("location")).toString();
    //create safe trigger word
    songTitle.remove(QRegExp(QLatin1String("\\(.*\\)")));
    songTitle.replace(' ', '_');
    songTitle = songTitle.trimmed();
    bool isNew = true;
    foreach (Word *w, internalWords) {
      if (w->getWord() == songTitle) {
        isNew = false;
        internalWords.removeAll(w);
        break;
      }
    }
    if (isNew) {
      toTranscribe << songTitle.split('_', QString::SkipEmptyParts);
      pseudoWordsToAdd.append(songTitle);
    }

    isNew = true;
    foreach (Command *c, oldCommands) {
      if (dynamic_cast<AmarokCommand*>(c)->getPath() == songPath) {
        isNew = false;
        oldCommands.removeAll(c);
        break;
      }
    }
    if (isNew)
      commandsToAdd.append(QPair<QString,QString>(songTitle, songPath));
  }

  //remove words no longer needed
  foreach (Word *w, internalWords)
    vocab->removeWord(w, true);

  //remove the commands no longer needed
  foreach (Command *c, oldCommands)
    deleteCommand(c);

  //add the (pseudo) words
  QHash<QString,QString> transcriptions = ScenarioManager::getInstance()->transcribe(toTranscribe);
  foreach (const QString& wordToBe, pseudoWordsToAdd) {
    QStringList transcription;
    foreach (const QString realWord, wordToBe.split('_'))
      transcription.append(transcriptions.value(realWord.toUpper()));
    parentScenario->addWord(new Word(wordToBe, transcription.join(" "), AmarokCommandWordCategory));
  }

  //add the commands
  for (int i = 0; i < commandsToAdd.count(); ++i) {
    const QPair<QString,QString>& commandToBe = commandsToAdd[i];
    addCommand(new AmarokCommand(commandToBe.first, "amarok", i18n("Play song"), commandToBe.second));
  }

  ScenarioManager::getInstance()->commitGroup();
}

CreateCommandWidget* AmarokCommandManager::getCreateCommandWidget(QWidget *parent)
{
  return new CreateAmarokCommandWidget(this, parent);
}

AmarokCommandManager::~AmarokCommandManager()
{
}
