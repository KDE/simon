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

/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class MSAABackend : public ATBackend
{
		Q_OBJECT
	public:
		MSAABackend ( QObject* parent );
		void startMonitoring();
		void stopMonitoring();

		~MSAABackend();

};

#endif
