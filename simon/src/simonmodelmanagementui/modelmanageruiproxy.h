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


#ifndef MODELMANAGERUIPROXY_H
#define MODELMANAGERUIPROXY_H

#include <speechmodelmanagement/modelmanager.h>
#include "simonmodelmanagementui_export.h"

/**
 *	@class ModelManagementUiProxy
 *	@brief Provides a "proxy" for the underlying modelmanager-concept-class which adds ui elements
 *
 *	@version 0.1
 *	@date 18.11.2008
 *	@author Peter Grasch
*/

class SIMONMODELMANAGEMENTUI_EXPORT ModelManagerUiProxy : public ModelManager
{
	Q_OBJECT

signals:
	void recompileModel();

private slots:
	void slotModelChanged();

// private:
// 	bool connectedToServer;
// 	bool askForSync;

public:
	ModelManagerUiProxy(QObject *parent=0);
	~ModelManagerUiProxy();
	
	
	void sampleNotAvailable(const QString&);
	void wordUndefined(const QString&);
	void classUndefined(const QString&);
 	void phonemeUndefined(const QString&);
	void displayCompilationProtocol(const QString& protocol);

	
// 	void setConnectionStatus(bool connected)
// 	{
// 		connectedToServer = connected;
// 	}
// 	void setAskForSync(bool ask)
// 	{
// 		askForSync = ask;
// 	}
};

#endif

