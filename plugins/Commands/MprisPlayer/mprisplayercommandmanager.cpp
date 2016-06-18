/*
 *   Copyright (C) 2014 Ashish Madeti <ashishmadeti@gmail.com>
 *   Copyright (C) 2014 Peter Grasch <peter.grasch@bedahr.org>
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

#include "mprisplayercommandmanager.h"
#include "mprisplayerconfiguration.h"
#include "mprisplayercommand.h"
#include "createmprisplayercommandwidget.h"
#include "player.h"
#include <simonactions/actionmanager.h>
#include <simonscenarios/scenario.h>
#include <simonscenarios/activevocabulary.h>
#include <simonscenarios/grammar.h>

#include <QDBusConnection>
#include <QDBusConnectionInterface>
#include <QDBusReply>
#include <KI18n/klocalizedstring.h>
#include <KPluginFactory>

K_PLUGIN_FACTORY( MprisPlayerPluginFactory,
                  registerPlugin< MprisPlayerCommandManager >();
)

// K_EXPORT_PLUGIN( MprisPlayerPluginFactory("simoncommandmprisplayer") )

const QString MprisCategoryPrefix("MPRIS_");

MprisPlayerCommandManager::MprisPlayerCommandManager(QObject *parent, const QVariantList& args)
    : CommandManager((Scenario*) parent, args),
      m_registerWatcher(0),
      m_unregisterWatcher(0)
{
}

bool MprisPlayerCommandManager::shouldAcceptCommand(Command *command)
{
    return (dynamic_cast<MprisPlayerCommand*>(command) != 0);
}

const QString MprisPlayerCommandManager::preferredTrigger() const
{
    return QString();
}

const QString MprisPlayerCommandManager::iconSrc() const
{
    return "applications-multimedia";
}

const QString MprisPlayerCommandManager::name() const
{
    return i18n("Media Player Control");
}

void MprisPlayerCommandManager::finalize()
{
    cleanupCommandsAndWords();
}

const QStringList MprisPlayerCommandManager::targetServices()
{
    if (static_cast<MprisPlayerConfiguration*>(config)->supportAll()) {
        return m_mediaPlayerList;
    } else {
        return QStringList(static_cast<MprisPlayerConfiguration*>(config)->selectedMediaService());
    }

}

const QStringList MprisPlayerCommandManager::runningMediaPlayerServices()
{
    return m_mediaPlayerList;
}

void MprisPlayerCommandManager::cleanupCommandsAndWords()
{
    if (!parentScenario)
        return;

    removeWordsWithCategoryPrefix(MprisCategoryPrefix);

    ScenarioManager::getInstance()->startGroup();

    CommandList oldCommands = commands;
    foreach (Command *c, oldCommands) {
        MprisPlayerCommand *mpCommand = static_cast<MprisPlayerCommand*>(c);
        if (mpCommand && mpCommand->role() == PlayParticular) {
            deleteCommand(c);
        }
    }

    ScenarioManager::getInstance()->commitGroup();
}

void MprisPlayerCommandManager::cleanupCommandsAndWords(const QString& serviceName)
{
    QString categoryPrefix = MprisCategoryPrefix + serviceName.mid(MprisPlayerPrefix.size());
    removeWordsWithCategoryPrefix(categoryPrefix);

    ScenarioManager::getInstance()->startGroup();

    CommandList oldCommands = commands;
    foreach (Command *c, oldCommands) {
        MprisPlayerCommand *mpCommand = static_cast<MprisPlayerCommand*>(c);
        if (mpCommand && mpCommand->serviceName() == serviceName) {
            deleteCommand(c);
            oldCommands.removeAll(c);
        }
    }

    ScenarioManager::getInstance()->commitGroup();
}

void MprisPlayerCommandManager::removeWordsWithCategoryPrefix(const QString& prefix)
{
    if (!parentScenario)
        return;

    ScenarioManager::getInstance()->startGroup();

    Vocabulary *vocab = parentScenario->vocabulary();
    QList<Word*> internalWords = vocab->getWords();
    foreach (Word *w, internalWords) {
        if (w->getCategory().startsWith(prefix)) {
            //remove the sentence from grammar
            parentScenario->grammar()->deleteStructure(w->getCategory());

            vocab->removeWord(w, true);
            internalWords.removeAll(w);
        }
    }

    ScenarioManager::getInstance()->commitGroup();
}

CreateCommandWidget* MprisPlayerCommandManager::getCreateCommandWidget(QWidget *parent)
{
    return new CreateMprisPlayerCommandWidget(this, parent);
}

bool MprisPlayerCommandManager::deSerializeConfig(const QDomElement& elem)
{
    config = new MprisPlayerConfiguration(parentScenario);
    config->deSerialize(elem);

    return true;
}

bool MprisPlayerCommandManager::deSerializeCommandsPrivate(const QDomElement& elem)
{
    if (elem.isNull()) {
        return false;
    }
    QDomElement commandElem = elem.firstChildElement("command");
    while(!commandElem.isNull())
    {
        Command *c = MprisPlayerCommand::createInstance(commandElem);
        if (c) {
            commands.append(c);
        }
        commandElem = commandElem.nextSiblingElement("command");
    }

    QDBusConnection sessionConn = QDBusConnection::sessionBus();
    QDBusReply<QStringList> reply = sessionConn.interface()->registeredServiceNames();
    if (reply.isValid()) {
        QStringList services = reply.value();
        foreach (const QString& serviceName, services) {
            serviceRegistered(serviceName);
        }
    }

    m_registerWatcher = new QDBusServiceWatcher(QString(), sessionConn,
                                                QDBusServiceWatcher::WatchForRegistration, this);
    m_unregisterWatcher = new QDBusServiceWatcher(QString(), sessionConn,
                                                  QDBusServiceWatcher::WatchForUnregistration, this);

    connect(m_registerWatcher, SIGNAL(serviceRegistered(QString)), this, SLOT(serviceRegistered(QString)));
    connect(m_unregisterWatcher, SIGNAL(serviceUnregistered(QString)), this, SLOT(serviceUnregistered(QString)));

    return true;
}

MprisPlayerCommandManager::~MprisPlayerCommandManager()
{
    delete m_registerWatcher;
    delete m_unregisterWatcher;
}

void MprisPlayerCommandManager::serviceRegistered(const QString& serviceName)
{
    Player *player;
    if (serviceName.startsWith(MprisPlayerPrefix) && !m_mediaPlayerList.contains(serviceName)) {
        m_mediaPlayerList << serviceName;

        player = new Player(serviceName, this);
        m_players.insert(serviceName, player);
    }
}

void MprisPlayerCommandManager::serviceUnregistered(const QString& serviceName)
{
    m_mediaPlayerList.removeAll(serviceName);

    Player *player = m_players.take(serviceName);
    if (player != 0) {
        player->deleteLater();
        cleanupCommandsAndWords(serviceName);
    }
}

void MprisPlayerCommandManager::addToCommandlist(const TrackList& trackList, const QString& serviceName)
{
    ScenarioManager::getInstance()->startGroup();

    Vocabulary *vocab = parentScenario->vocabulary();
    QList<Word*> internalWords = vocab->getWords();
    CommandList oldCommands = commands;

    //pairs of word and category name
    QList< QPair<QString, QString> > pseudoWordsToAdd;
    QStringList toTranscribe;

    QString categoryPrefix = MprisCategoryPrefix + serviceName.mid(MprisPlayerPrefix.size()) +  "_";

    QString trackTitle;
    QString trackId;
    foreach (const Track& t, trackList) {
        trackId = t.first;
        trackTitle = t.second;

        //create safe trigger word
        trackTitle.remove(QRegExp(QLatin1String("\\(.*\\)")));
        trackTitle.remove(QLatin1String("("));
        trackTitle.remove(QLatin1String(")"));
        trackTitle.remove(QLatin1String("$"));
        trackTitle.remove(QLatin1String("/"));
        trackTitle.replace(' ', '_');
        trackTitle = trackTitle.trimmed();


        QString category = categoryPrefix + trackId.mid(trackId.lastIndexOf("/") + 1);
        toTranscribe << trackTitle.split('_', QString::SkipEmptyParts);
        pseudoWordsToAdd.append(QPair<QString, QString>(trackTitle, category));

        //add the sentence to scenario's grammar
        parentScenario->grammar()->addStructure(category);

        //check if we already have the required command
        bool isNew = true;
        foreach (Command *c, oldCommands) {
            if (static_cast<MprisPlayerCommand*>(c)->trackId() == trackId
                    && static_cast<MprisPlayerCommand*>(c)->serviceName() == serviceName) {
                isNew = false;
                break;
            }
        }

        //if not, add the command
        if (isNew) {
            addCommand(new MprisPlayerCommand(trackTitle,
                                              "applications-multimedia",
                                              i18n("Play media: %1", trackTitle), serviceName,
                                              trackId));
        }
    }

    //add the (pseudo) words
    QHash<QString,QString> transcriptions = ScenarioManager::getInstance()->transcribe(toTranscribe);
    QPair<QString, QString> wordToBe;
    foreach (wordToBe, pseudoWordsToAdd) {
        QStringList transcription;
        foreach (const QString realWord, wordToBe.first.split('_'))
            transcription.append(transcriptions.value(realWord.toUpper()));
        parentScenario->addWord(new Word(wordToBe.first, transcription.join(" "), wordToBe.second));
    }

    ScenarioManager::getInstance()->commitGroup();
}

void MprisPlayerCommandManager::removeFromCommandlist(const QStringList& removedTracksList, const QString& serviceName)
{
    ScenarioManager::getInstance()->startGroup();

    Vocabulary *vocab = parentScenario->vocabulary();
    QList<Word*> internalWords = vocab->getWords();
    CommandList oldCommands = commands;

    QString categoryPrefix = MprisCategoryPrefix + serviceName.mid(MprisPlayerPrefix.size()) +  "_";

    foreach (const QString& trackId, removedTracksList) {
        foreach (Command *c, oldCommands) {
            MprisPlayerCommand *mpCommand = static_cast<MprisPlayerCommand*>(c);
            if (mpCommand && mpCommand->trackId() == trackId &&
                    mpCommand->serviceName() == serviceName) {
                deleteCommand(c);
                oldCommands.removeAll(c);
                break;
            }
        }

        QString category = categoryPrefix + trackId.mid(trackId.lastIndexOf("/") + 1);
        foreach (Word *w, internalWords) {
            if (w->getCategory().startsWith(category)) {
                vocab->removeWord(w, true);
                internalWords.removeAll(w);
            }
        }

        //remove the sentence from scenario's grammar
        parentScenario->grammar()->deleteStructure(category);
    }

    ScenarioManager::getInstance()->commitGroup();
}

#include "mprisplayercommandmanager.moc"
