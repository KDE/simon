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

#define STATIC_CREATE_INSTANCE_H(x) \
	static x* createInstance(const QDomElement& element);

#define STATIC_CREATE_INSTANCE_C(x) \
	x* x::createInstance(const QDomElement& element) \
	{ \
		x *command = new x(); \
		if (!command->deSerialize(element)) \
		{ \
			delete command; \
			return NULL; \
		} \
		return command; \
	} 
		

#include "simonmodelmanagement_export.h"

#include <QList>
#include <QString>
#include <KIcon>
#include <QMap>
class QDomElement;
class QDomDocument;


/**
 * @class Command
 * @brief This class represents one command with all its attributes
 *
 * Extend this base class if you want your command manager to have subcommands.
 *
 * If your commands should be user configurable, you also have to extend 
 * CreateCommandWidget.
 *
 * \sa CreateCommandWidget
 */
class Command;

/**
 *	@typedef CommandList
 *	QList of Commands
 */
typedef QList<Command*> CommandList;

#include "commandmanager.h"

class CommandManager;


class MODELMANAGEMENT_EXPORT  Command {

private:
	/// The name of the command - this is used to call the command and should be unique
	/// in the parent CommandManager
	QString triggerName; 

	/// Icon name of the command
	QString iconSrc; 

	/// Detailed, user visible description of the command
	QString description;

	/// If this is true, the base class will provide graphical feedback when the command is triggered (passive popup)
	bool announce;

	/// The parent CommandManager
	CommandManager *m_parent;

protected:
	/// The command is bound to this state.
	/// \sa matches()
	int boundState;

	/// Whenever the command is executed, the commandmanager should switch into this state
	int switchToState;

	/// Empty private constructor 
    	Command() {}

	virtual bool triggerPrivate(int* state)=0;
	virtual const QMap<QString,QVariant> getValueMapPrivate() const=0;

	virtual QDomElement serializePrivate(QDomDocument *doc, QDomElement& commandElem)=0;
	virtual bool deSerializePrivate(const QDomElement& commandElem)=0;

public:
	void setParent(CommandManager* man) { m_parent = man; }
	CommandManager* parent() { return m_parent; }

	static const QString staticCategoryText() {return "";}
	static const KIcon staticCategoryIcon() {return KIcon();}

	virtual const KIcon getCategoryIcon() const=0;
	virtual const QString getCategoryText() const=0;

	virtual bool trigger(int* state);

	virtual bool matches(int commandManagerState, const QString& trigger);

	virtual const QMap<QString,QVariant> getValueMap() const;


	
    /**
    * @brief Constructor
    * 
    *	@author Peter Grasch
    *	@param trigger 
    *	@param icon
    */
    Command(const QString& name, const QString& icon, const QString& description_, int boundState_ = SimonCommand::DefaultState,
		    int newState_ = SimonCommand::DefaultState, bool announce_ = true)
        : triggerName(name),
        iconSrc(icon),
	description(description_),
	announce(announce_),
	boundState(boundState_),
	switchToState(newState_)
    {
    }

    /**
    * @brief Returns the name
    * 
    *	@author Peter Grasch
    *	@return Returns the name of the command (member)
    */
    QString getTrigger() const { return this->triggerName; }

    int getBoundState() const { return this->boundState; }
    int getTargetState() const { return this->switchToState; }

    bool getAnnounce() const { return announce; }
    
    QString getDescription() const { return this->description; }
    
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
   bool deSerialize(const QDomElement& commandElem);

    /**
    * @brief Returns the Icon of this command.
    *
    * @author Peter Grasch
    * @return The icon
    */
   const QString getIconSrc() const {
	return iconSrc;
   }
    
   virtual ~Command() {}
};

#endif
