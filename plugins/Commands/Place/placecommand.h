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

#ifndef SIMON_PLACECOMMAND_H_DFA997ECD2A140B19F98DACE4C6CCA9A
#define SIMON_PLACECOMMAND_H_DFA997ECD2A140B19F98DACE4C6CCA9A

#include <simonscenarios/command.h>
#include <KUrl>
#include <KIcon>

/**
 *	@class PlaceCommand
 *	@brief Ressembles one place-command
 *
 *	@version 0.1
 *	@date 20.05.2008
 *	@author Peter Grasch
 */
class PlaceCommand : public Command
{

  private:
    KUrl url;

  protected:
    const QMap<QString,QVariant> getValueMapPrivate() const;
    bool triggerPrivate(int *state);
    bool deSerializePrivate(const QDomElement& commandElem);

    PlaceCommand() {}

  public:
    static const QString staticCategoryText();
    static const KIcon staticCategoryIcon();

    const KIcon getCategoryIcon() const;
    const QString getCategoryText() const;

    STATIC_CREATE_INSTANCE_H(PlaceCommand);

    QDomElement serializePrivate(QDomDocument *doc, QDomElement& commandElem);

    /**
     * @brief Constructor
     *
     *	@author Peter Grasch
     */
    PlaceCommand(const QString& name, const QString& iconSrc, const QString& description, const KUrl& url_) : Command(name, iconSrc, description),
    url(url_) {
    }

    /**
     * @brief Returns the url
     *
     *	@author Peter Grasch
     */
    const KUrl getURL() const { return this->url; }

    ~PlaceCommand() {}

};
#endif
