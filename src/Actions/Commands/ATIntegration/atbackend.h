//
// C++ Interface: atbackend
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef ATBACKEND_H
#define ATBACKEND_H

#include <QObject>

class ATObject;

/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class ATBackend : public QObject
{
Q_OBJECT

signals:
	/**
	 * \brief Emits the found object in a tree-like structure
	 *
	 * Once we found something useful on the AT-Bus we are using, we emit it using this signal.
	 * It is defined as follows:
	 * 	* QList of widgets
	 * 	* Each widget has a parent which builds sort of a "tree" of widgets
	 * 	* The highest widget has a pseudo element as his parent, which is also the first in the list;
	 * 	  It represents the application itself (because there could be more than one "main window" (-> e.g. Dolphin)
	 * 	  Its trigger is the applicationname
	 *
	 * @param name 
	 * @param widgets 
	 */
	void objectFound(ATObject* widgets);
	void objectDeleted(ATObject *widget);
	void objectFocused(ATObject *widget);
public:
	ATBackend(QObject* parent) : QObject(parent) {}
	virtual void startMonitoring()=0;
	virtual void stopMonitoring()=0;

    virtual ~ATBackend() {}

};

#endif
