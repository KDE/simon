/*
 *   Copyright (C) 2014 Ashish Madeti <ashishmadeti@gmail.com>
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
#include "mprisconstants.h"
#include "createmprisplayercommandwidget.h"

#include <QDBusConnection>
#include <QDBusConnectionInterface>
#include <QDBusReply>
#include <KLocalizedString>

K_PLUGIN_FACTORY( MprisPlayerPluginFactory,
                  registerPlugin< MprisPlayerCommandManager >();
)

K_EXPORT_PLUGIN( MprisPlayerPluginFactory("simoncommandmprisplayer") )

MprisPlayerCommandManager::MprisPlayerCommandManager(QObject *parent, const QVariantList &args)
    : CommandManager((Scenario*) parent, args)
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

CreateCommandWidget* MprisPlayerCommandManager::getCreateCommandWidget(QWidget *parent)
{
    return new CreateMprisPlayerCommandWidget(this, parent);
}

bool MprisPlayerCommandManager::deSerializeConfig(const QDomElement &elem)
{
    config = new MprisPlayerConfiguration(parentScenario);
    config->deSerialize(elem);

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

DEFAULT_DESERIALIZE_COMMANDS_PRIVATE_C(MprisPlayerCommandManager, MprisPlayerCommand)

MprisPlayerCommandManager::~MprisPlayerCommandManager()
{
}

void MprisPlayerCommandManager::serviceRegistered(const QString &serviceName)
{
    if (serviceName.startsWith(MprisPlayerPrefix) && !m_mediaPlayerList.contains(serviceName)) {
        m_mediaPlayerList << serviceName;
    }
}

void MprisPlayerCommandManager::serviceUnregistered(const QString &serviceName)
{
    m_mediaPlayerList.removeAll(serviceName);
}
