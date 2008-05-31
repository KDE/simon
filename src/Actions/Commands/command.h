//
// C++ Interface: command
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef COMMAND_H
#define COMMAND_H
#include <QList>
#include <QObject>
#include <QString>
#include <QIcon>
#include <QMap>

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


class Command : public QObject{

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

public:
	static const QString staticCategoryText() {return "";}
	static const QIcon staticCategoryIcon() {return QIcon();}

	virtual const QIcon getCategoryIcon() const=0;
	virtual const QString getCategoryText() const=0;

	virtual bool trigger();

	virtual const QMap<QString,QVariant> getValueMap() const;


	virtual void remove() { emit removed(); deleteLater(); }
	virtual void change(QString newName, QString newIconSrc) { 
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
    Command(QString name, QString iconSrc)
    {
        this->triggerName = name;
        this->iconSrc = iconSrc;
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
   const QIcon getIcon() const {
	QIcon thisIcon = QIcon(iconSrc);
	if (!thisIcon.isNull())
		return thisIcon;
	else return getCategoryIcon();
   }
    
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
