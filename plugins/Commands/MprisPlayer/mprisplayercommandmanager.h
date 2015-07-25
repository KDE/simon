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

#ifndef SIMON_MPRISPLAYERCOMMANDMANAGER_H_7A7B9100FF5245329569C1B540119C37
#define SIMON_MPRISPLAYERCOMMANDMANAGER_H_7A7B9100FF5245329569C1B540119C37

#include <simonscenarios/commandmanager.h>
#include "mprisconstants.h"

#include <QDBusServiceWatcher>

class MprisPlayerConfiguration;
class Player;

/**
 *	@class MprisPlayerCommandManager
 *	@brief Plugin to control MPRIS enabled media players
 *
 *	@version 0.1
 *	@date 19.05.2014
 *	@author Ashish Madeti
 */
class MprisPlayerCommandManager : public CommandManager
{
    Q_OBJECT

  protected:
    bool shouldAcceptCommand(Command *command);

  public:
    const QString preferredTrigger() const;
    const QString iconSrc() const;
    const QString name() const;
    void finalize();

    const QStringList targetServices();
    void addToCommandlist(const TrackList& trackIdTitleMap,
                          const QString& serviceName);
    void removeFromCommandlist(const QStringList& removedTracksList,
                          const QString& serviceName);

    CreateCommandWidget* getCreateCommandWidget(QWidget *parent);

    bool deSerializeConfig(const QDomElement& elem);
    bool deSerializeCommandsPrivate(const QDomElement& elem);

    MprisPlayerCommandManager(QObject* parent, const QVariantList& args);
    ~MprisPlayerCommandManager();

  private slots:
    void serviceRegistered(const QString& serviceName);
    void serviceUnregistered(const QString& serviceName);

  private:
    const QStringList runningMediaPlayerServices();
    void cleanupCommandsAndWords();
    void cleanupCommandsAndWords(const QString& serviceName);
    void removeWordsWithCategoryPrefix(const QString& prefix);

    QStringList m_mediaPlayerList;
    QDBusServiceWatcher* m_registerWatcher;
    QDBusServiceWatcher* m_unregisterWatcher;
    QHash<QString, Player*> m_players;
};

#endif // SIMON_MPRISPLAYERCOMMANDMANAGER_H_7A7B9100FF5245329569C1B540119C37
