/*
 *   Copyright (C) 2008 Peter Grasch <peter.grasch@bedahr.org>
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

#ifndef SIMON_COMPOSITECOMMANDMANAGER_H_F6E45596DD464FAFAB75EE58A7A9D2E9
#define SIMON_COMPOSITECOMMANDMANAGER_H_F6E45596DD464FAFAB75EE58A7A9D2E9

#include <simonscenarios/commandmanager.h>
#include <QVariantList>

class CreateCompositeCommandWidget;
/**
 *	@class CompositeCommandManager
 *	@brief Manager for the composite commands
 *
 *	@version 0.1
 *	@date 20.05.2008
 *	@author Peter Grasch
 */
class CompositeCommandManager : public CommandManager
{
  Q_OBJECT
    protected:
    bool shouldAcceptCommand(Command *command);
  public:
    const QString iconSrc() const;
    const QString name() const;
    CreateCommandWidget* getCreateCommandWidget(QWidget *parent);

    DEFAULT_DESERIALIZE_COMMANDS_PRIVATE_H;

    /**
     * @brief Constructor
     *
     *	@author Peter Grasch
     */
    CompositeCommandManager(QObject* parent, const QVariantList& args);

    ~CompositeCommandManager();

};
#endif
