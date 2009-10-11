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

#ifndef COMMAND_H
#define COMMAND_H

#include "simoncommandpluginbase_export.h"

#include <QList>
#include <QObject>
#include <QString>
#include <KIcon>
#include <QMap>
class QDomElement;
class QDomDocument;


/**
 *	@class Command
 *	@brief Ressamles one command with all the attributes
 *
 *	@version 0.1
 *	@date 23.01.2006
 *	@author Peter Grasch
 */
class Command;

/**
 *	@typedef CommandList
 *	QList of Commands
 */
typedef QList<Command*> CommandList;


class SIMONCOMMANDPLUGINBASE_EXPORT Command : public QObject{

Q_OBJECT

private:
	QString triggerName; //!< The name of the command - this is used to call the command and is unique
	QString iconSrc; //!< The icon of a command.

signals:
	void changed();
	void removed();

protected:
	virtual bool triggerPrivate()=0;
	virtual const QMap<QString,QVariant> getValueMapPrivate() const=0;

	virtual QDomElement serializePrivate(QDomDocument *doc, QDomElement& commandElem)=0;

public:
	static const QString staticCategoryText() {return "";}
	static const KIcon staticCategoryIcon() {return KIcon();}

	virtual const KIcon getCategoryIcon() const=0;
	virtual const QString getCategoryText() const=0;

	virtual bool trigger();

	virtual const QMap<QString,QVariant> getValueMap() const;


	virtual void remove() { emit removed(); deleteLater(); }
	void change(const QString& newName, const QString& newIconSrc) { 
		triggerName = newName;
		iconSrc = newIconSrc;
		emit changed();
	}
	
    /**
    * @brief Constructor
    * 
    *	@author Peter Grasch
    *	@param trigger 
    *	@param icon
    */
    Command(const QString& name, const QString& icon)
        : triggerName(name),
        iconSrc(icon)
    {
    }

    /**
    * @brief Returns the name
    * 
    *	@author Peter Grasch
    *	@return Returns the name of the command (member)
    */
    QString getTrigger() const { return this->triggerName; }
    
    
    /**
    * @brief Returns the Icon of this command.
    *
    * @author Peter Grasch
    * @return The icon
    */
   const KIcon getIcon() const {
	KIcon thisIcon = KIcon(iconSrc);
	if (!thisIcon.isNull())
		return thisIcon;
	else return getCategoryIcon();
   }
 
   QDomElement serialize(QDomDocument *doc);

    /**
    * @brief Returns the Icon of this command.
    *
    * @author Peter Grasch
    * @return The icon
    */
   const QString getIconSrc() const {
	return iconSrc;
   }
    
    ~Command() {}

};

#endif
