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

class MprisPlayerConfiguration;

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

    CreateCommandWidget* getCreateCommandWidget(QWidget *parent);

    bool deSerializeConfig(const QDomElement& elem);
    DEFAULT_DESERIALIZE_COMMANDS_PRIVATE_H

    MprisPlayerCommandManager(QObject* parent, const QVariantList& args);
    ~MprisPlayerCommandManager();

};

#endif // SIMON_MPRISPLAYERCOMMANDMANAGER_H_7A7B9100FF5245329569C1B540119C37
