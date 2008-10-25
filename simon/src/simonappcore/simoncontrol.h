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


#ifndef SIMONCONTROL_H
#define SIMONCONTROL_H

/**
 *	@class SimonControl
 *	@brief The Main Control Class
 *
 * 	The Main "Control" Class. It handles all the GUI Signals and operates
 * 	accordingly. It is the main point of the simon application and as such
 * 	ties all the individual parts together
 *
 *	@version 0.1
 *	@date 07.01.2006
 *	@author Peter Grasch
*/

#include <QObject>
#include <QStringList>
#include <simonrecognitioncontrol/recognitioncontrol.h>
#include "simonappcore_export.h"

class QSettings;
class QVariant;
class RecognitionControl;
class ActionManager;

class SIMONAPPCORE_EXPORT SimonControl : public QObject {
	Q_OBJECT

public:
	
	enum SystemStatus {
		Disconnected=0,
		Connecting=1,
		ConnectedDeactivated=2,
		ConnectedActivated=3
	};
	
	void connectTo(QString host);
	SimonControl::SystemStatus deactivateSimon();
	SimonControl::SystemStatus activateSimon();

	SimonControl::SystemStatus toggleActivition();
	
	SimonControl::SystemStatus getStatus() const {return status;}
	void setStatus(SimonControl::SystemStatus status);

	SimonControl(QWidget *parent=0);

	~SimonControl();

	bool passwordProtected();
	QString adminPassword();
	bool askBeforeQuit();
	
signals:
	void connected();
	void disconnected();
	void guiAction(const QString& action);
	void systemStatusChanged(SimonControl::SystemStatus);
	void statusInfo(const QString&);
	void statusError(const QString&);
	void progressInfo(int now, int max=-1);
	
public slots:
	void connectToServer();
	void disconnectFromServer();
	void connectedToServer();
	void disconnectedFromServer();
	void wordRecognised(QString word,QString sampa,QString samparaw);
	void abortConnecting();
	void errorConnecting(QString error);

	void compileModel();

private slots:
	void serverError(QString error,bool skippable);
	void serverWarning(QString warning);
	void loggedIn();
	void recognitionStatusChanged(RecognitionControl::RecognitionStatus status);

private:
	SimonControl::SystemStatus status;
	
	RecognitionControl *recognitionControl; //!< Julius Backend
	
	

};

#endif
