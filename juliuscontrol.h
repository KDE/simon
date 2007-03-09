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
#include <QTcpServer>
#include <QTcpSocket>
#include <QByteArray>
#include <QMessageBox>

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
	QTcpServer *server;  //!< The listening server
	QTcpSocket *socket; //!< If a client connects to the server this holds the connecting socket
signals:
	void wordRecognised(QString word);
	void connected();
	void disconnected();
public slots:
	void recognised();
	void getConnection();
	void connectionLost();

public:
	JuliusControl(quint16 port=4444);

    ~JuliusControl();

};

#endif
