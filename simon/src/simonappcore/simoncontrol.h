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
		ConnectedDeactivating=3,
		ConnectedDeactivatedNotReady=4,
		ConnectedDeactivatedReady=5,
		ConnectedPaused=6,
		ConnectedActivating=7,
		ConnectedResuming=8,
		ConnectedActivated=9
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
	bool startMinimized();
	QString adminPassword();
	
signals:
	void guiAction(const QString& action);
	void systemStatusChanged(SimonControl::SystemStatus);
// 	void statusInfo(const QString&);
// 	void statusError(const QString&);
// 	void progressInfo(int now, int max=-1);

	void simondSystemError(const QString &err);
	void synchronisationError(const QString &err);
	void recognitionError(const QString &err);
	void compilationError(const QString &err);

	
public slots:
	void connectToServer();
	void disconnectFromServer();
	void connectedToServer();
	void disconnectedFromServer();
	void wordRecognised(const RecognitionResultList& recognitionResults);
	void abortConnecting();

	void compileModel();

private slots:
	void slotConnectionError(const QString& err);
	void slotSimondSystemError(const QString &err);
	void slotSynchronisationError(const QString &err);
	void slotRecognitionError(const QString &err);
	void slotCompilationError(const QString &err, const QString& protocol);

	void slotSimondSystemWarning(const QString& warning);
	void slotSynchronisationWarning(const QString& warning);
	void slotRecognitionWarning(const QString& warning);
	void slotCompilationWarning(const QString& warning);

	void loggedIn();
	void recognitionStatusChanged(RecognitionControl::RecognitionStatus status);

private:
	SimonControl::SystemStatus status;
	
	RecognitionControl *recognitionControl; //!< Julius Backend
	
	

};

#endif
