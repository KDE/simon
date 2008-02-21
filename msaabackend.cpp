//
// C++ Implementation: msaabackend
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "msaabackend.h"

MSAABackend::MSAABackend(QObject* parent)
 : ATBackend(parent)
{
	
}

/**
 * \brief Starts the monitoring of the MSAA Bus
 * \author 
 * 
 * Call this method to start monitoring the MSAA Bus for running applications.
 * Once we find something on this bus (i.e. the user selected a msaa-aware application window with his mouse) we create a
 * tree-like structure (described in detail in the ATBackend class) that represents the window and its widgets.
 * 
 * This is then emitted with the objectFound(...) signal (also documented in the ATBackend class
 */
void MSAABackend::startMonitoring()
{
	//TODO: Implement monitoring
}


void MSAABackend::stopMonitoring()
{
	//TODO: Implement stopping
}


MSAABackend::~MSAABackend()
{
	
}


