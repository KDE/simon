//
// C++ Interface: juliuscontrol
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef JULIUSCONTROL_H
#define JULIUSCONTROL_H

#include <QObject>
#include "simoninfo.h"

class QTcpSocket;

/**
 *	@class JuliusControl
 *	@brief Provides the methods and emits the needed signals to work with Julius
 *
 *	@version 0.1
 *	@date 23.01.2006
 *	@author Peter Grasch
 */
class JuliusControl : public QObject {
	Q_OBJECT
private:
	QTcpSocket *socket; //!< QTcpSocket for communicating with the juliusd-socket
signals:
	void wordRecognised(QString word);
	void connected();
	void disconnected();
	void error(QString error);
public slots:
	void recognised();
	void connectTo( QString server="127.0.0.1", quint16 port=4444 );
	void connectionLost();
	void connectedTo();
	bool isConnected();
	void disconnect();

public:
	JuliusControl();

    ~JuliusControl();

};

#endif
