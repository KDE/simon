/*
 *   Copyright (C) 2008 Peter Grasch <grasch@simon-listens.org>
 *   Copyright (C) 2010 Manfred Scheucher <deadalps@gmail.com>
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

#ifndef SIMON_SHORTCUTCOMMAND_H_51F6847BEF9D4333AA3B7713F789AB98
#define SIMON_SHORTCUTCOMMAND_H_51F6847BEF9D4333AA3B7713F789AB98

#include <QKeySequence>
#include <simonscenarios/command.h>
#include <eventsimulation/shortcutmode.h>

/**
 *	@class ShortcutCommand
 *	@brief Ressembles one shortcut-command
 *
 *	@version 0.1
 *	@date 20.05.2008
 *	@author Peter Grasch
 */
class ShortcutCommand : public Command
{

  protected:
    const QMap<QString,QVariant> getValueMapPrivate() const;

    bool triggerPrivate(int *state);

    ShortcutCommand() {}
    
  public:    
    bool deSerializePrivate(const QDomElement& commandElem);
    QDomElement serializePrivate(QDomDocument *doc, QDomElement& commandElem);

    static const QString staticCategoryText();
    static const KIcon staticCategoryIcon();

    const KIcon getCategoryIcon() const;
    const QString getCategoryText() const;

    /**
     * @brief Constructor
     *
     *	@author Peter Grasch
     */
    ShortcutCommand(const QString& name, const QString& iconSrc, const QString& description, const QKeySequence& shortcut_, EventSimulation::ShortcutMode mode_) : Command(name, iconSrc, description),
    shortcut(shortcut_), mode(mode_) {
    }

    STATIC_CREATE_INSTANCE_H(ShortcutCommand);

    /**
     * @brief Returns the shortcut
     *
     *	@author Peter Grasch
     */
    QKeySequence getShortcut() const { return shortcut; }
    EventSimulation::ShortcutMode getMode() const { return this->mode; }

    ~ShortcutCommand() {
    }
    
  private:
    QKeySequence shortcut;
    EventSimulation::ShortcutMode mode;
    
    QString prettyPrintMode() const;

};
#endif
