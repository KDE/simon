//
// C++ Interface: place
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 20078
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef PLACECOMMAND_H
#define PLACECOMMAND_H


#include "../command.h"
#include <QUrl>


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
	QUrl url;

protected:
	const QMap<QString,QVariant> getValueMapPrivate() const;
	bool triggerPrivate();

public:
	static const QString staticCategoryText();
	static const QIcon staticCategoryIcon();

	const QIcon getCategoryIcon() const;
	const QString getCategoryText() const;
	
    /**
    * @brief Constructor
    * 
    *	@author Peter Grasch
    */
    PlaceCommand(QString name, QString iconSrc, QUrl url) : Command(name, iconSrc)
    {
        this->url = url;
    }

	void change(QString newName, QString newIconSrc, QUrl newUrl) { 
		this->url = newUrl;
		Command::change(newName, newIconSrc);
	}

    /**
    * @brief Returns the url
    * 
    *	@author Peter Grasch
    */
    const QUrl getURL() const { return this->url; }
    
    
    ~PlaceCommand() {}

};

#endif
