/*  This file is part of the KDE project
    Copyright (C) 2005 Kopete Developers <kopete-devel@kde.org>
    Copyright (C) 2008-2009 Pali Rohár <pali.rohar@gmail.com>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License version 2 as published by the Free Software Foundation.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.

*/
#ifndef SKYPECONNECTION_H
#define SKYPECONNECTION_H

#include <qobject.h>

typedef enum {
	///The connection was successful
	seSuccess = 0,
	///No running DBUS found
	seNoDBus,
	///No running skype found and launching disabled or did not worked
	seNoSkype,
	///User did not accept this app
	seAuthorization,
	///Some other error
	seUnknown,
	///It was canceled (by disconnectSkype)
	seCanceled
} skypeConnectionError;

///This describes why was the connection closed
typedef enum {
	///It was closed by this application
	crLocalClosed,
	///It was closed by skype (reserverd for future versions of protocol, does not work yet)
	crRemoteClosed,
	///The connection was lost, skype does not respond for the ping command or messages can not be sent
	crLost
} skypeCloseReason;

class SkypeConnectionPrivate;
class QDBusConnection;

/**
 * This class is classs wrapping DBUS so it can be used easilly to connect to skype, disconnect send and receive messages from it.
 * @author Kopete Developers
 * @author Pali Rohár
 */
class SkypeConnection : public QObject
{
	Q_OBJECT
	Q_CLASSINFO("D-Bus Interface", "com.Skype.API.Client")
	private:
		///The D-pointer for internal things
		SkypeConnectionPrivate *d;
	private slots:
		///This one takes care of incoming messages if they have some sence for the connection (protocol, pings and so on)
		void parseMessage(const QString &message);
		///Starts logging into skype
		void startLogOn();
		///Another interval try to connect to just started Skype
		void tryConnect();
	public slots:
		/**
		 * Connects to skype
		 * After connection (both successful or unsuccessful) connectionDone is emitted
		 * @see connectionDone
		 * @param start By what command start Skype if it is not running (empty string means nothing is started)
		 * @param appName tells as what application it should authorise itself (this will user see on the "do you want to allow" dialog box)
		 * @param protocolVer Maximal protocol version that this app manages
		 * @param bus 0 - session bus, 1 - system bus
		 * @param launchTimeout How long max. should wait to tell that launching skype did not work
		 * @param waitBeforeConnect Do we need to wait a while after skype starts?
		 * @param name login user name
		 * @param pass login password
		 */
		void connectSkype(const QString &start, const QString &appName, int protocolVer, int bus, int launchTimeout, int waitBeforeConnect, const QString &name = QString(), const QString &pass = QString());
		/**
		 * Disconnects from skype
		 * @see connectionClosed
		 */
		void disconnectSkype(skypeCloseReason reason = crLocalClosed);
		/**
		 * Sends a message to skype. You must be connected before !
		 * @param message Contains the message to send
		 */
		void send(const QString &message);
		///This one listens for incoming messages
		void Notify(const QString &message);
	public:
		/**
		 * Constructor. Creates UNCONECTED connection (sounds oddly ?)
		 */
		SkypeConnection();
		/**
		 * Destructor
		 */
		~SkypeConnection();
		/**
		 * This enables/disables pings to skype and sets interval of the pings and timeout
		 * @param enable Enable or not the pings? If this is false, no pings are done and the rest of parameters has no effect
		 * @param interval Interval in witch pings should be sent, in miliseconds
		 * @param timeout When the ping should be considered unanwsered? (should be shorter than interval), in miliseconds
		 */
		void setPing(bool enable, int interval, int timeout);
		/**
		 * @return Are the pings enabled?
		 */
		bool getPing() const;
		/**
		 * @return What is the interval of pings? (ms)
		 */
		int getPingInterval() const;
		/**
		 * @return What is the timeout od pings? (ms)
		 */
		int getPingTimeout() const;
		/**
		 * @return Are we connected to the skype?
		 */
		bool connected() const;
		/**
		 * @return What is the protocol version?
		 */
		int protocolVer() const;
		/**
		 * This operator makes it possible to just send messages by writing connection << SomeString << anotherString. They are sent as separate objects;
		 * @param message What will be sent
		 */
		SkypeConnection &operator<<(const QString &message);
		/**
		 * This operator sends a message to skype and returns the response from it. Note that this one blocks.
		 * @param message What should be sent to skype
		 * @return The response from skype
		 */
		QString operator%(const QString &message);
		/**
		 * @return Skype state;
		 */
	signals:
		/**
		 * This signal is emitted when an attempt to connect to skype application is done. It is done in both cases, success or not.
		 * @param error Indicates error code. seSuccess means there was no error and the connection was successful.
		 * @param protocolVer Protocol version used by this connection. Is less or equal to the version set in connect
		 * @see connect
		 */
		void connectionDone(int error, int protocolVer);
		/**
		 * This signal is emitted when the connection is closed due to error or because it was disconnetcted
		 * @param reason Describes why it was closed (you can typecast it to skypeCloseReason if you are interested, or just use the numeric values)
		 */
		void connectionClosed(int reason);
		/**
		 * This slot is emitted when something is coming from skype.
		 * It contains pongs as well (responses to ping) and if you do not care about them, you should ignore them.
		 * @param message The message that arrived
		 */
		void received(const QString &message);
		/**
		 * This is emitted when some error occurs
		 * @param message Describes the error
		 */
		void error(const QString &message);
		/**
		 * This is provided for debugging so you can see what you have sent to skype
		 * @param message The message that was sent to skype
		 */
		void sent(const QString &message);
};

#endif
