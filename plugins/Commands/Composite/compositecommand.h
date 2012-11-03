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

#ifndef SIMON_COMPOSITECOMMAND_H_A199F7E674344957A6099B02965126F1
#define SIMON_COMPOSITECOMMAND_H_A199F7E674344957A6099B02965126F1

#include <simonscenarios/command.h>
#include <QList>
#include <KUrl>

/**
 *	@class CompositeCommand
 *	@brief Describes a composite command; Consists of several other commands
 *
 *	@version 0.1
 *	@date 19.05.2008
 *	@author Peter Grasch
 */
class CompositeCommand : public Command
{
  private:
    QStringList commands;
    QStringList commandTypes;
    bool passThrough;

  protected:
    const QMap<QString,QVariant> getValueMapPrivate() const;
    bool triggerPrivate(int *state);
    void triggerPrivateThread();
    bool deSerializePrivate(const QDomElement& commandElem);

    CompositeCommand() {}

  public:
    STATIC_CREATE_INSTANCE_H(CompositeCommand);
    static const QString staticCategoryText();
    static const KIcon staticCategoryIcon();

    const KIcon getCategoryIcon() const;
    const QString getCategoryText() const;

    QDomElement serializePrivate(QDomDocument *doc, QDomElement& commandElem);

    /**
     * @brief Constructor
     *
     *	@author Peter Grasch
     */
    CompositeCommand(const QString& name, const QString& iconSrc, const QString& description, bool passThrough_,
                     const QStringList& commands_, const QStringList& commandTypes_) : Command(name, iconSrc, description),
      commands(commands_),
      commandTypes(commandTypes_),
      passThrough(passThrough_)
    {
    }

    bool getPassThrough() const { return this->passThrough; }
    QStringList getCommands() const { return this->commands; }
    QStringList getCommandTypes() const { return this->commandTypes; }

    ~CompositeCommand() {}

};
#endif
