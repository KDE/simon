/*
 *   Copyright (C) 2009 Peter Grasch <grasch@simon-listens.org>
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

#ifndef SIMON_DBUSCOMMANDMANAGER_H_7A7B9100FF5245329569C1B540119C37
#define SIMON_DBUSCOMMANDMANAGER_H_7A7B9100FF5245329569C1B540119C37

#include <simonscenarios/commandmanager.h>
#include <QVariantList>

/**
 *	@class DBusCommandManager
 *	@brief Manager for the dbus commands
 *
 *	@version 0.1
 *	@date 4.10.2010
 *	@author Peter Grasch
 */
class DBusCommandManager : public CommandManager
{
  Q_OBJECT

  protected:
    bool shouldAcceptCommand(Command *command);

  public:
    const QString preferredTrigger() const { return QString(); }
    const QString iconSrc() const;
    const QString name() const;

    CreateCommandWidget* getCreateCommandWidget(QWidget *parent);

    DEFAULT_DESERIALIZE_COMMANDS_PRIVATE_H;

    /**
     * @brief Constructor
     *
     *	@author Peter Grasch
     */
    DBusCommandManager(QObject* parent, const QVariantList& args);

    ~DBusCommandManager();

};
#endif
