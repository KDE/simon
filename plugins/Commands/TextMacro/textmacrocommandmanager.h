/*
 *   Copyright (C) 2008 Peter Grasch <grasch@simon-listens.org>
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

#ifndef SIMON_TEXTMACROCOMMANDMANAGER_H_EDFE3225C4B0465491338847D911E13A
#define SIMON_TEXTMACROCOMMANDMANAGER_H_EDFE3225C4B0465491338847D911E13A

#include <simonscenarios/commandmanager.h>

class CreateCommandWidget;
/**
 *	@class PlaceCommandManager
 *	@brief Manager for the place-commands
 *
 *	@version 0.1
 *	@date 20.05.2008
 *	@author Peter Grasch
 */
class TextMacroCommandManager : public CommandManager
{
  Q_OBJECT
    protected:
    bool shouldAcceptCommand(Command *command);
  public:
    const QString name() const;
    const QString iconSrc() const;
    CreateCommandWidget* getCreateCommandWidget(QWidget *parent);

    DEFAULT_DESERIALIZE_COMMANDS_PRIVATE_H;

    /**
     * @brief Constructor
     *
     *	@author Peter Grasch
     */
    TextMacroCommandManager(QObject* parent, const QVariantList& args);

    ~TextMacroCommandManager();

};
#endif
