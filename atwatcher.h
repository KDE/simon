//
// C++ Interface: atwatcher
//
// Description:
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef ATWATCHER_H
#define ATWATCHER_H

#include <QObject>

class ATBackend;
class ATObject;
/**
	@author Peter Grasch <bedahr@gmx.net>
*/


/// \warning Was wenn von einer untestützten Applikation zu einer nicht unterstützten umgewechselt wird? was wird emmitted?
class ATWatcher : public QObject
{
		Q_OBJECT
	private:
		ATBackend* backend;
		ATObject *focusedApplication; //!< holds a pointer to the currently selected app.
		ATObject *focusedWindow; //!< holds a pointer to the currently opened window
		QList<ATObject*> applications;
		
	private slots:
		void addObject(ATObject *newObject);
		void deleteObject(ATObject *oldObject);
		void translateFocusToWindow(ATObject* selectedObject);
	public slots:
		bool trigger(QString word);
	public:
		ATWatcher ( QObject* parent );

		~ATWatcher();

};

#endif
