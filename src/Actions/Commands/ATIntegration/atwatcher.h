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


/// \warning Was wenn von einer untest�tzten Applikation zu einer nicht unterst�tzten umgewechselt wird? was wird emmitted?
class ATWatcher : public QObject
{
		Q_OBJECT
	private:
		ATBackend* backend;
		ATObject *focusedApplication; //!< holds a pointer to the currently selected app.
		ATObject *focusedWindow; //!< holds a pointer to the currently opened window
		QList<ATObject*> applications;
		static ATWatcher *instance;
		
	private slots:
		void addObject(ATObject *newObject);
		void deleteObject(ATObject *oldObject);
		void translateFocusToWindow(ATObject* selectedObject);
	public slots:
		bool trigger(const QString &word);

	protected:
		ATWatcher ( QObject* parent=0 );

	public:
		void applySettings();
		static ATWatcher* getInstance() {
			if (!instance) instance = new ATWatcher();
			return instance;
		}

		~ATWatcher();

};

#endif
