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

#ifndef SIMON_MPRISPLAYERCOMMAND_H_1822E03B797C494599B9629C6B47E478
#define SIMON_MPRISPLAYERCOMMAND_H_1822E03B797C494599B9629C6B47E478

#include <simonscenarios/command.h>
#include "mprisconstants.h"

#include <QDBusReply>
#include <QDomElement>
#include <QDomDocument>

/**
 *	@class MprisPlayerCommand
 *	@brief Creates a particular D-Bus call to send to media player
 *
 *	@version 0.1
 *	@date 19.05.2014
 *	@author Ashish Madeti
 */
class MprisPlayerCommand : public Command
{
  public:
    static const QString staticCategoryText();
    static const KIcon staticCategoryIcon();

    const QString getCategoryText() const;
    const KIcon getCategoryIcon() const;

    QDomElement serializePrivate(QDomDocument *doc, QDomElement& commandElem);
    bool deSerializePrivate(const QDomElement& commandElem);

    MprisPlayerCommand(const QString& name, const QString& iconSrc, const QString& description,
                       CommandRole role) :
        Command(name, iconSrc, description),
        m_role(role)
    {
    }
    MprisPlayerCommand(const QString& name, const QString& iconSrc,
                       const QString& description, const QString& serviceName,
                       const QString& trackId) :
        Command(name, iconSrc, description),
        m_role(PlayParticular),
        m_serviceName(serviceName),
        m_trackId(trackId)
    {
    }

    ~MprisPlayerCommand() {}

    CommandRole role();
    QString trackId();
    QString serviceName();

    STATIC_CREATE_INSTANCE_H(MprisPlayerCommand)

  protected:
    const QMap<QString, QVariant> getValueMapPrivate() const;
    bool triggerPrivate(int *state);
    MprisPlayerCommand() {}

  private:
    QDBusReply<QVariant> getPropertyValue(const QString& service, const QString& interfaceName,
                                          const QString& propertyName);
    bool setPropertyValue(const QString& service, const QString& interfaceName,
                          const QString& propertyName, const QVariant& propValue);

    CommandRole m_role;

    //These members used only for "PlayParticular" CommandRole
    QString m_serviceName;
    QDBusObjectPath m_trackId;
};

#endif // SIMON_MPRISPLAYERCOMMAND_H_1822E03B797C494599B9629C6B47E478
