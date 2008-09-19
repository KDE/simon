//
// C++ Interface: msaabackend
//
// Description:
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef MSAABACKEND_H
#define MSAABACKEND_H

#include "atbackend.h"

#include "ato.h"
#include "atobject.h"
#include <QStringList>

/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class MSAABackend : public ATBackend
{
		Q_OBJECT
    
    private:
        QStringList windowNameList;
    
	public:
		MSAABackend ( QObject* parent );
		void startMonitoring();
		void stopMonitoring();
    
        QStringList getWindowNames();
		QString getWindowClassName(QString windowName);
        QString getForegroundWindowName();  //HWND GetForegroundWindow(VOID);
    	ATOLocation getLocation(QString windowName);

		~MSAABackend();

};

#endif
