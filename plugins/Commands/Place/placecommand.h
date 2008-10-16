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


#ifndef PLACECOMMAND_H
#define PLACECOMMAND_H


#include <commandpluginbase/command.h>
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
class PlaceCommand : public Command{
Q_OBJECT
private:
	KUrl url;

protected:
	const QMap<QString,QVariant> getValueMapPrivate() const;
	bool triggerPrivate();

public:
	static const QString staticCategoryText();
	static const KIcon staticCategoryIcon();

	const KIcon getCategoryIcon() const;
	const QString getCategoryText() const;
	
    /**
    * @brief Constructor
    * 
    *	@author Peter Grasch
    */
    PlaceCommand(const QString& name, const QString& iconSrc, const KUrl& url) : Command(name, iconSrc)
    {
        this->url = url;
    }

	void change(const QString& newName, const QString& newIconSrc, const KUrl& newUrl) { 
		this->url = newUrl;
		Command::change(newName, newIconSrc);
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
