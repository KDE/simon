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

#ifndef SIMON_JSONCOMMANDMANAGER_H_7A7B9100FF5245329569C1B540119C37
#define SIMON_JSONCOMMANDMANAGER_H_7A7B9100FF5245329569C1B540119C37

#include <simonscenarios/commandmanager.h>
#include <QVariantList>

class JsonConfiguration;

/**
 *	@class JsonCommandManager
 *	@brief Manager for the json commands
 *
 *	@version 0.1
 *	@date 20.05.2008
 *	@author Peter Grasch
 */
class JsonCommandManager : public CommandManager
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
    bool deSerializeConfig(const QDomElement& elem);

    /**
     * @brief Constructor
     *
     *	@author Peter Grasch
     */
    JsonCommandManager(QObject* parent, const QVariantList& args);

    ~JsonCommandManager();

};
#endif
