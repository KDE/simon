/*  This file is part of the KDE project
    Copyright (C) 2005 Michal Vaner <michal.vaner@kdemail.net>
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
#include "skype.h"
#include "skypeconnection.h"
//#include "skypeaccount.h"

#include <kdebug.h>
#include <qstring.h>
#include <klocale.h>
#include <kmessagebox.h>
#include <knotification.h>
#include <qtimer.h>
#include <QDateTime>

#define PROTOCOL_MAX 8
#define PROTOCOL_MIN 6
#define TEST_QUIT if (!d->connection.connected()) return;

///This one indicates, weather the Skype is connected (does not mean weather it is marked as online, just if it has connection to the site)
typedef enum {
	csOffline,
	csConnecting,
	csPausing,
	csOnline,
	csLoggedOut
} connectionStatus;

///This describes what the user is marked as. If online here and not connected to skype site, he is probably offline
typedef enum {
	usUnknown,
	usOffline,
	usOnline,
	usSkypeMe,
	usAway,
	usNA,
	usDND,
	usInvisible
} userStatus;

class SkypePrivate {
	public:
		///The connection
		SkypeConnection connection;
		///The queue
		QStringList messageQueue;
		///How do we start skype?
		int launchType;
		///What is our name?
		QString appName;
		///Should the skypeconnection start skype automatically if it is not running?
		bool start;
		///Is the skype connected?
		connectionStatus connStatus;
		///What is the online status for the user?
		userStatus onlineStatus;
		///This contains last search request to know, what we were searching for
		QString searchFor;
		///Is the hitch-mode enabled?
		bool hitch;
		///Is the mark read messages mode enabled?
		bool mark;
		///The skype account this connection belongs to
		//SkypeAccount &account;
		///Should we show the message that Skype died? It if off when going offline, this removes that onnoying message when logging off and skype finishes first.
		bool showDeadMessage;
		///Do we automatically scan for unread messages on login?
		bool scanForUnread;
		///Constructor
		SkypePrivate(/*SkypeAccount &_account*/) /*: account(_account)*/ {};//initialize all that needs it
		///List of known calls, so they are not showed twice
		QStringList knownCalls;
		///Are the pings enabled?
		bool pings;
		///Pinging timer
		QTimer *pingTimer;
		///What bus is used now?
		int bus;
		///The launch timeout (after that no connection -> unsuccessfull -> error)
		int launchTimeout;
		///By what command is skype started?
		QString skypeCommand;
		///Do we wait before connecting?
		int waitBeforeConnect;
		///List of alredy received messages (IDs)
		QStringList recvMessages;
		///Skype Groups with contacts <id, contact>
		QMultiHash <int, QString> groupsContacts;
		///Skype Groups names <name, id>
		QMultiHash <QString, int> groupsNames;
		///Timer to fix & load groups
		QTimer *fixGroupTimer;
};

Skype::Skype(/*SkypeAccount &account*/) : QObject(), quitting(false) {
	kDebug(SKYPE_DEBUG_GLOBAL);

	d = new SkypePrivate(/*account*/);//create the d-pointer

	//initial values
	d->connStatus = csOffline;
	d->onlineStatus = usOffline;
	d->searchFor = "";
	d->pings = false;
	d->pingTimer = new QTimer;
	d->fixGroupTimer = new QTimer;

	connect(&d->connection, SIGNAL(connectionClosed(int)), this, SLOT(closed(int)));//tell me if you close/lose the connection
	connect(&d->connection, SIGNAL(connectionDone(int, int)), this, SLOT(connectionDone(int, int)));//Do something whe he finishes connecting
	connect(&d->connection, SIGNAL(error(const QString&)), this, SLOT(error(const QString&)));//Listen for errors
	connect(&d->connection, SIGNAL(received(const QString&)), this, SLOT(skypeMessage(const QString&)));//Take all incoming messages
	connect(d->pingTimer, SIGNAL(timeout()), this, SLOT(ping()));
	connect(d->fixGroupTimer, SIGNAL(timeout()), this, SLOT(fixGroups()));//fix & load groups to memory
}


Skype::~Skype() {
	kDebug(SKYPE_DEBUG_GLOBAL);

  quitting = true;

  disconnect(&d->connection);
	//if (d->connection.connected())
		//d->connection << QString("SET USERSTATUS OFFLINE");

	d->pingTimer->stop();
	d->fixGroupTimer->stop();
	d->pingTimer->deleteLater();
	d->fixGroupTimer->deleteLater();

	delete d;//release the memory
}

void Skype::setOnline() {
	kDebug(SKYPE_DEBUG_GLOBAL);
	d->showDeadMessage = true;

	if ((d->onlineStatus == usOnline) && (d->connStatus == csOnline) && (d->connection.connected()))
		return;//Already online

	queueSkypeMessage("SET USERSTATUS ONLINE", true);//just send the message
}

void Skype::setUserProfileRichMoodText(const QString &message) {
	kDebug(SKYPE_DEBUG_GLOBAL);

	queueSkypeMessage(QString("SET PROFILE RICH_MOOD_TEXT %1").arg(message), false);
}

void Skype::setOffline() {
	kDebug(SKYPE_DEBUG_GLOBAL);
	d->showDeadMessage = false;

	d->connection << QString("SET USERSTATUS OFFLINE");//this one special, do not connect to skype because of that
	d->connection % QString("SET SILENT_MODE OFF");//try turn on (to default) all gui skype notification - this is not supported by skype yet, once it will works
	d->connection.disconnectSkype();
}

void Skype::setAway() {
	kDebug(SKYPE_DEBUG_GLOBAL);
	d->showDeadMessage = true;

	queueSkypeMessage("SET USERSTATUS AWAY", true);
}

void Skype::setNotAvailable() {
	kDebug(SKYPE_DEBUG_GLOBAL);
	d->showDeadMessage = true;

	queueSkypeMessage("SET USERSTATUS NA", true);
}

void Skype::setDND() {
	kDebug(SKYPE_DEBUG_GLOBAL);
	d->showDeadMessage = true;

	queueSkypeMessage("SET USERSTATUS DND", true);
}

void Skype::setInvisible() {
	kDebug(SKYPE_DEBUG_GLOBAL);
	d->showDeadMessage = true;

	queueSkypeMessage("SET USERSTATUS INVISIBLE", true);
}

void Skype::setSkypeMe() {
	kDebug(SKYPE_DEBUG_GLOBAL);
	d->showDeadMessage = true;

	queueSkypeMessage("SET USERSTATUS SKYPEME", true);
}

void Skype::queueSkypeMessage(const QString &message, bool deleteQueue) {
	kDebug(SKYPE_DEBUG_GLOBAL);

	if (d->connection.connected()) {//we are connected, so just send it
		d->connection << message;//just send it
	} else {
		emit statusConnecting();//Started connecting to skype
		if (deleteQueue)
			d->messageQueue.clear();//delete all old messages
		d->messageQueue << message;//add the new one
		d->connection.connectSkype((d->start) ? d->skypeCommand : "", "simon_skype", PROTOCOL_MAX, d->bus, d->launchTimeout, d->waitBeforeConnect);//try to connect
	}
}

void Skype::setValues(int launchType, const QString &appName) {
	kDebug(SKYPE_DEBUG_GLOBAL);

	d->appName = appName;
	if (d->appName.isEmpty()) //The defaut one?
		d->appName = "Kopete";
	d->launchType = launchType;
	switch (launchType) {
		case 0: //start the skype if it is needed
			d->start = true;//just set autostart
			break;
		case 1: //do not start
			d->start = false;//do not start
			break;
	}
}

void Skype::closed(int) {
	kDebug(SKYPE_DEBUG_GLOBAL);
  if (quitting) return;

	emit wentOffline();//No longer connected
	d->messageQueue.clear();//no messages will wait, it was lost
	d->pingTimer->stop();
	d->fixGroupTimer->stop();
}

void Skype::connectionDone(int error, int protocolVer) {
	kDebug(SKYPE_DEBUG_GLOBAL);

	if (d->pings) {
		d->pingTimer->start(1000);
	}

	if (error == seSuccess) {//It worked
		if (protocolVer < PROTOCOL_MIN) {//The protocol is too old, it is not useable
			this->error(i18n("This version of Skype is too old, consider upgrading"));
			connectionDone(seUnknown, 0);//So act like there was an error
			return;//and it is all fo now
		}

		d->connection % QString("MINIMIZE");//try hide Skype to system tray
		d->connection % QString("SET SILENT_MODE ON");//try turn off all gui skype notification - this is not supported by skype yet, once it will works
		///TODO: Set silent mode on, when is turned off

		while (d->messageQueue.size()) {//It isn't empty yet?
			QStringList::iterator it = d->messageQueue.begin();//take the first one
			d->connection << (*it);//send the message
			//d->messageQueue.remove(it);//remove this one
			d->messageQueue.removeFirst();
		}
		emit updateAllContacts();//let all contacts update their information
		fixGroups();//fix & load groups immediately
		search("FRIENDS");//search for friends - to add them all
		TEST_QUIT;//if it failed, do not continue
		d->connection.send("GET USERSTATUS");
		TEST_QUIT;
		d->connection.send("GET CONNSTATUS");
		d->fixGroupTimer->start(60000);//fix & load groups to memory every minutes
	} else {
		closed(crLost);//OK, this is wrong, justclose the connection/atempt and delete the queue
	}
}

void Skype::error(const QString &message) {
	kDebug(SKYPE_DEBUG_GLOBAL);

	disconnect(&d->connection, SIGNAL(error(const QString&)), this, SLOT(error(const QString&)));//One arror at a time is enough, stop flooding the user

	if (d->showDeadMessage)//just skip the error message if we are going offline, none ever cares.
		KNotification::event(KNotification::Error, i18n("Skype protocol"), message);//Show the message

	connect(&d->connection, SIGNAL(error(const QString&)), this, SLOT(error(const QString&)));//Continue showing more errors in future
}

void Skype::skypeMessage(const QString &message) {
	kDebug(SKYPE_DEBUG_GLOBAL);

	QString messageType = message.section(' ', 0, 0).trimmed().toUpper();//get the first part of the message
	if (messageType == "CONNSTATUS") {//the connection status
		QString value = message.section(' ', 1, 1).trimmed().toUpper();//get the second part of the message
		if (value == "OFFLINE")
			d->connStatus = csOffline;
		else if (value == "CONNECTING")
			d->connStatus = csConnecting;
		else if (value == "PAUSING")
			d->connStatus = csPausing;
		else if (value == "ONLINE")
			d->connStatus = csOnline;
		else if (value == "LOGGEDOUT")
			d->connStatus = csLoggedOut;

		resetStatus();//set new status
	} else if (messageType == "VOICEMAIL") {//the connection status
		QString value = message.section(' ', 1, 1).trimmed().toUpper();//get the second part of the message
    value = value.left(value.indexOf(" "));
    emit voiceMailActive(value.toInt());
	} else if (messageType == "USERSTATUS") {//Status of this user
		QString value = message.section(' ', 1, 1).trimmed().toUpper();//get the second part
		if (value == "UNKNOWN")
			d->onlineStatus = usUnknown;
		else if (value == "OFFLINE")
			d->onlineStatus = usOffline;
		else if (value == "ONLINE")
			d->onlineStatus = usOnline;
		else if (value == "SKYPEME")
			d->onlineStatus = usSkypeMe;
		else if (value == "AWAY")
			d->onlineStatus = usAway;
		else if (value == "NA")
			d->onlineStatus = usNA;
		else if (value == "DND")
			d->onlineStatus = usDND;
		else if (value == "INVISIBLE")
			d->onlineStatus = usInvisible;

		resetStatus();
	} else if (messageType == "USERS") {//some user info
		QString theRest = message.section(' ', 1).trimmed();//take the rest
		if (d->searchFor == "FRIENDS") {//it was initial search for al users
			QStringList names = theRest.split(", ");//divide it into names by comas
			kDebug(SKYPE_DEBUG_GLOBAL) << "Names: " << names;//write what you have done with that
			for (QStringList::iterator it = names.begin(); it != names.end(); ++it) {//run trough the names
				QString name = (*it).trimmed();//get the name only
				if (name.isEmpty())
					continue;//just skip the empty names
				int groupID = getContactGroupID(name);
				if ( groupID != -1 )
					kDebug(SKYPE_DEBUG_GLOBAL) << "Found group for user" << name << groupID << ":" << getGroupName(groupID);
				else
					kDebug(SKYPE_DEBUG_GLOBAL) << "Not found group for user" << name;
				emit newUser(name, groupID);//add the user to list
			}
			if (d->scanForUnread)
				search("MISSEDMESSAGES");
		}
	} else if (messageType == "USER") {//This is for some contact
		const QString &contactId = message.section(' ', 1, 1);//take the second part, it is the user name
		const QString &type = message.section(' ', 2, 2).trimmed().toUpper();//get what it is
		if ((type == "FULLNAME") || (type == "DISPLAYNAME") || (type == "SEX") ||
			(type == "PHONE_HOME") || (type == "PHONE_OFFICE") ||
			(type == "PHONE_MOBILE") ||
			(type == "ONLINESTATUS") || (type == "BUDDYSTATUS") || (type == "HOMEPAGE")) {
			const QString &info = message.section(' ', 2);//and the rest is just the message for that contact
			emit contactInfo(contactId, info);//and let the contact know
		} else if ( type == "ISBLOCKED" || type == "ISAUTHORIZED" ) {
			/// TODO: Implement status ISBLOCKED and ISAUTHORIZED
			kDebug(SKYPE_DEBUG_GLOBAL) << "Status ISBLOCKED and ISAUTHORIZED is not implemented for contact, ignored";
		} else if ( type == "ABOUT" ) {
			/// TODO: Implement info ABOUT
			kDebug(SKYPE_DEBUG_GLOBAL) << "Info ABOUT is not implemented for contact, ignored";
		} else if ( type == "RECEIVEDAUTHREQUEST") {
			if ( getAuthor(contactId) != Skype::Author ) {// Skype send all authorize request min 2x, filter if contact is authorized
				const QString &info = message.section(' ', 3);
				emit receivedAuth(contactId, info);
				kDebug(SKYPE_DEBUG_GLOBAL) << "Received auth request from" << contactId;
			}
		} else {
			kDebug(SKYPE_DEBUG_GLOBAL) << "Unknown message for contact, ignored";
		}
	} else if (messageType == "CHATMESSAGE") {//something with message, maebe incoming/sent
		QString messageId = message.section(' ', 1, 1).trimmed();//get the second part of message - it is the message ID
		QString type = message.section(' ', 2, 2).trimmed().toUpper();//This part significates what about the message are we talking about (status, body, etc..)
		QString chatMessageType = (d->connection % QString("GET CHATMESSAGE %1 TYPE").arg(messageId)).section(' ', 3, 3).toUpper();
		if (chatMessageType == "ADDEDMEMBERS") {
			QString status = message.section(' ', 3, 3).trimmed().toUpper();
			if (d->recvMessages.indexOf(messageId) != d->recvMessages.lastIndexOf(QRegExp(messageId)))
				return;
			d->recvMessages << messageId;
			const QString &users = (d->connection % QString("GET CHATMESSAGE %1 USERS").arg(messageId)).section(' ', 3).trimmed();
			QStringList splitUsers = users.split(' ');
			const QString &chatId = (d->connection % QString("GET CHATMESSAGE %1 CHATNAME").arg(messageId)).section(' ', 3, 3).trimmed();
			for (QStringList::iterator it = splitUsers.begin(); it != splitUsers.end(); ++it) {
				if ((*it).toUpper() == getMyself().toUpper())
					continue;
				emit joinUser(chatId, *it);
			}
			return;
		} else if (chatMessageType == "LEFT") {
			QString status = message.section(' ', 3, 3).trimmed().toUpper();
			if (d->recvMessages.indexOf(messageId) != d->recvMessages.lastIndexOf(QRegExp(messageId)))
				return;
			d->recvMessages << messageId;
			const QString &chatId = (d->connection % QString("GET CHATMESSAGE %1 CHATNAME").arg(messageId)).section(' ', 3, 3).trimmed();
			const QString &chatType = (d->connection % QString("GET CHAT %1 STATUS").arg(chatId)).section(' ', 3, 3).trimmed().toUpper();
			if ((chatType == "DIALOG") || (chatType == "LEGACY_DIALOG"))
				return;
			const QString &user = (d->connection % QString("GET CHATMESSAGE %1 FROM_HANDLE").arg(messageId)).section(' ', 3, 3).trimmed();
			const QString &reason = (d->connection % QString("GET CHATMESSAGE %1 LEAVEREASON").arg(messageId)).section(' ', 3, 3).trimmed().toUpper();
			QString showReason = i18n("Unknown");
			if (reason == "USER_NOT_FOUND") {
				showReason = i18n("User not found");
			} else if (reason == "USER_INCAPABLE") {
				showReason = i18n("Does not have multi-user chat capability");
			} else if ((reason == "ADDER_MUST_BE_FRIEND") || ("ADDER_MUST_BE_AUTHORIZED")) {
				showReason = i18n("Chat denied");
			} else if (reason == "UNSUBSCRIBE") {
				showReason = "";
			}
			if (user.toUpper() == getMyself().toUpper())
				return;
			emit leftUser(chatId, user, showReason);
			return;
		}
		if (type == "STATUS") {//OK, status of some message has changed, check what is it
			QString value = message.section(' ', 3, 3).trimmed().toUpper();//get the last part, what status it is
			if (value == "RECEIVED") {//OK, received new message, possibly read it
				if (chatMessageType == "SAID") {//OK, it is some IM
					hitchHike(messageId);//receive the message
				}
			} else if (value == "SENDING") {
				if ((d->connection % QString("GET CHATMESSAGE %1 TYPE").arg(messageId)).section(' ', 3, 3).trimmed().toUpper() == "SAID") {
					emit gotMessageId(messageId);
				}
			} else if (value == "SENT") {//Sending out some message, that means it is a new one
				if ((d->connection % QString("GET CHATMESSAGE %1 TYPE").arg(messageId)).section(' ', 3, 3).trimmed().toUpper() == "SAID")//it is some message I'm interested in
					emit gotMessageId(messageId);//Someone may be interested in its ID
				if (d->recvMessages.indexOf(messageId) != d->recvMessages.lastIndexOf(QRegExp(messageId)))
					return;//we already got this one
				d->recvMessages << messageId;
				const QString &chat = (d->connection % QString("GET CHATMESSAGE %1 CHATNAME").arg(messageId)).section(' ', 3, 3).trimmed();
				const QString &body = (d->connection % QString("GET CHATMESSAGE %1 BODY").arg(messageId)).section(' ', 3);
				if (!body.isEmpty())//sometimes skype shows empty messages, just ignore them
					emit outgoingMessage(messageId, body, chat);
			}
		} else if ( type == "EDITED_TIMESTAMP" ) {//timestamp of message was edited
			///TODO: Implement this
			kDebug(SKYPE_DEBUG_GLOBAL) << "Timestamp of message" << messageId << "was edited, this is not implemented now";
		} else if ( type == "EDITED_BY" ) {//this message was edited
			///TODO: Implement this
			QString editedBy = message.section(' ', 3, 3).trimmed();
			kDebug(SKYPE_DEBUG_GLOBAL) << "Message" << messageId << "was edited by" << editedBy << ", this is not implemented now";
		} else if ( type == "BODY" ) { //This message was edited and has new body
			///TODO: Implement this
			QString newBody = message.section(' ', 3, 3).trimmed();
			kDebug(SKYPE_DEBUG_GLOBAL) << "Message" << messageId << "was edited to" << newBody << ", this is not implemented now";
		}
	} else if (messageType == "CHATMESSAGES") {
		if (d->searchFor == "MISSEDMESSAGES") {//Theese are messages we did not read yet
			QStringList messages = message.section(' ', 1).split(',');//get the meassage IDs
			for (QStringList::iterator it = messages.begin(); it != messages.end(); ++it) {
				QString Id = (*it).trimmed();
				if (Id.isEmpty())
					continue;
				if ( (d->connection % QString("GET CHATMESSAGE %1 FROM_HANDLE").arg(Id)).section(' ', 3, 3).trimmed().toUpper() == getMyself().toUpper() )
					skypeMessage(QString("CHATMESSAGE %1 STATUS SENT").arg(Id));//this message is from current user, so it is outcomming
				else
					skypeMessage(QString("CHATMESSAGE %1 STATUS RECEIVED").arg(Id));//simulate incoming message notification
			}
		}
	} else if (messageType == "CALL") {
		const QString &callId = message.section(' ', 1, 1).trimmed();
		if (message.section(' ', 2, 2).trimmed().toUpper() == "CONF_ID") {
			if (d->knownCalls.indexOf(callId) == -1) {//new call
				d->knownCalls << callId;
				const QString &userId = (d->connection % QString("GET CALL %1 PARTNER_HANDLE").arg(callId)).section(' ', 3, 3).trimmed();
				emit newCall(callId, userId);
			}
			const QString &confId = message.section(' ', 3, 3).trimmed().toUpper();
			if (confId != "0") {//It is an conference
				emit groupCall(callId, confId);
			}
		}
		if (message.section(' ', 2, 2).trimmed().toUpper() == "STATUS") {
      if (message.contains("VM_SENT")) 
        emit voiceMessageSent();
			if (d->knownCalls.indexOf(callId) == -1) {//new call
				d->knownCalls << callId;
        const QString &userId = (d->connection % QString("GET CALL %1 PARTNER_HANDLE").arg(callId)).section(' ', 3, 3).trimmed();
        if (message.contains("ROUTING")) 
          kDebug() << "Skipping as this is an outgoing call...";
        else
          emit newCall(callId, userId);
			}
			const QString &status = message.section(' ', 3, 3).toUpper();
			if (status == "FAILED") {
				int reason = (d->connection % QString("GET CALL %1 FAILUREREASON").arg(callId)).section(' ', 3, 3).trimmed().toInt();
				QString errorText = i18n("Unknown error");
				switch (reason) {
					case 1:
						errorText = i18n("Misc error");
						break;
					case 2:
						errorText = i18n("User or phone number does not exist");
						break;
					case 3:
						errorText = i18n("User is offline");
						break;
					case 4:
						errorText = i18n("No proxy found");
						break;
					case 5:
						errorText = i18n("Session terminated");
						break;
					case 6:
						errorText = i18n("No common codec found");
						break;
					case 7:
						errorText = i18n("Sound I/O error");
						break;
					case 8:
						errorText = i18n("Problem with remote sound device");
						break;
					case 9:
						errorText = i18n("Call blocked by recipient");
						break;
					case 10:
						errorText = i18n("Recipient not a friend");
						break;
					case 11:
						errorText = i18n("User not authorized by recipient");
						break;
					case 12:
						errorText = i18n("Sound recording error");
						break;
				}
				emit callError(callId, errorText);
			}
			emit callStatus(callId, status);
		} else if ( message.section(' ', 2, 2).trimmed().toUpper() == "VIDEO_RECEIVE_STATUS" ) {
			const QString &status = message.section(' ', 3, 3).trimmed().toUpper();
			if ( status == "RUNNING" ) {
				kDebug(SKYPE_DEBUG_GLOBAL) << "Start receiving video";
				emit startReceivingVideo(callId);
			} else if ( status == "STOPPING" ) {
				kDebug(SKYPE_DEBUG_GLOBAL) << "Stop receiving video";
				emit stopReceivingVideo(callId);
			}
		}
	} else if (messageType == "CURRENTUSERHANDLE") {
		QString user = message.section(' ', 1, 1).trimmed();
		QString name = (d->connection % QString("GET USER %1 DISPLAYNAME").arg(user)).section(' ', 3).trimmed();
		if (name.isEmpty())
			name = (d->connection % QString("GET USER %1 FULLNAME").arg(user)).section(' ', 3).trimmed();
		if (name.isEmpty())
			name = user;
		emit setMyselfName(name);
	} else if (messageType == "PONG") {
		// Do nothing
	} else {
		kDebug(SKYPE_DEBUG_GLOBAL) << "Unknown message" << message << "of type" << messageType;
	}
}

void Skype::getContactBuddy(const QString &contact) {
	kDebug(SKYPE_DEBUG_GLOBAL);

	d->connection << QString("GET USER %1 BUDDYSTATUS").arg(contact);//just make a message asking for the buddystatus of user and send it
}

void Skype::resetStatus() {
	kDebug(SKYPE_DEBUG_GLOBAL);

	switch (d->connStatus) {
		case csOffline:
		case csLoggedOut:
			emit wentOffline();//Do not care what is the user marked as, this is more importatnt
			return;
		case csConnecting:
			//if (d->onlineStatus == usOffline)//not connecting, user wants to be offline
			//	break;
			emit statusConnecting();//still connecting, wait a minute
			return;
		default://just remove the compile-time warning about not handled value
			break;
	}

	switch (d->onlineStatus) {
		case usUnknown:
			emit statusConnecting();
			break;
		case usOffline:
			//emit wentOffline(); //We got onlineStatus usOffline too, when connStatus is csConnecting
			break;
		case usOnline:
			emit wentOnline();
			break;
		case usSkypeMe:
			emit wentSkypeMe();
			break;
		case usAway:
			emit wentAway();
			break;
		case usNA:
			emit wentNotAvailable();
			break;
		case usDND:
			emit wentDND();
			break;
		case usInvisible:
			emit wentInvisible();
			break;
	}
}

void Skype::search(const QString &what) {
	kDebug(SKYPE_DEBUG_GLOBAL);

	d->searchFor = what.section(' ', 0, 0).trimmed().toUpper();
	d->connection << QString("SEARCH %1").arg(what.toUpper());//search for that
}

void Skype::getContactInfo(const QString &contact) {
	kDebug(SKYPE_DEBUG_GLOBAL);

	d->connection << QString("GET USER %1 FULLNAME").arg(contact)//ask for full name
	<< QString("GET USER %1 SEX").arg(contact)//ask for sex
	<< QString("GET USER %1 DISPLAYNAME").arg(contact)
	<< QString("GET USER %1 PHONE_HOME").arg(contact)
	<< QString("GET USER %1 PHONE_OFFICE").arg(contact)
	<< QString("GET USER %1 PHONE_MOBILE").arg(contact)
	<< QString("GET USER %1 ONLINESTATUS").arg(contact)
	<< QString("GET USER %1 HOMEPAGE").arg(contact)
	<< QString("GET USER %1 BUDDYSTATUS").arg(contact);//and the rest of info
}

bool Skype::canComunicate() {
	return d->connection.connected();
}

void Skype::setHitchMode(bool value) {
	d->hitch = value;
}

void Skype::setMarkMode(bool value) {
	d->mark = value;
}

void Skype::hitchHike(const QString &messageId) {
	kDebug(SKYPE_DEBUG_GLOBAL) << "Message: " << messageId;

	const QString &chat = (d->connection % QString("GET CHATMESSAGE %1 CHATNAME").arg(messageId)).section(' ', 3, 3).trimmed();

	const QString &chatType = (d->connection % QString("GET CHAT %1 STATUS").arg(chat)).section(' ', 3, 3).trimmed().toUpper();

	/*bool ok = false;
	uint utime = (d->connection % QString("GET CHATMESSAGE %1 TIMESTAMP").arg(messageId)).section(' ', 3, 3).trimmed().toUInt(&ok);
	if ( ! ok )
		utime = QDateTime::currentDateTime().toTime_t();

	const QDateTime &timeStamp = QDateTime::fromTime_t(utime);

  */
	if ((chatType == "LEGACY_DIALOG") || (chatType == "DIALOG")) {

		/*const QString &user =*/ (d->connection % QString("GET CHATMESSAGE %1 FROM_HANDLE").arg(messageId)).section(' ', 3, 3).trimmed();//ask skype for a sender of that message and filter out the blouat around (like CHATMESSAGE 123...)

		//if ((d->hitch) || (d->account.userHasChat(user))) {//it can be read eather if the hitchhiking non-chat messages is enabled or if the user already has opened a chat
			//emit receivedIM(user, (d->connection % QString("GET CHATMESSAGE %1 BODY").arg(messageId)).section(' ', 3), messageId, timeStamp);//ask skype for the body and filter out the bload, we want only the text and make everyone aware that we received a message
			//if (d->mark) //We should mark it as read
				//d->connection << QString("SET CHATMESSAGE %1 SEEN").arg(messageId);//OK, just tell skype it is read
		//}
	} else {
		//if ((d->hitch) || (d->account.chatExists(chat))) {
			//const QString &user = (d->connection % QString("GET CHATMESSAGE %1 FROM_HANDLE").arg(messageId)).section(' ', 3, 3).trimmed();
			//emit receivedMultiIM(chat, (d->connection % QString("GET CHATMESSAGE %1 BODY").arg(messageId)).section(' ', 3), messageId, user, timeStamp);
			//if (d->mark)
				//d->connection << QString("SET CHATMESSAGE %1 SEEN").arg(messageId);
		//}
	}
}

QString Skype::send(const QString &user, const QString &message) {
	kDebug(SKYPE_DEBUG_GLOBAL);

	QString resp = d->connection % (QString("MESSAGE %1 %2").arg(user).arg(message));//just ask skype to send it

	QString messageType = resp.section(' ', 0, 0).trimmed().toUpper();
	if (messageType == "CHATMESSAGE") {
		QString messageId = resp.section(' ', 1, 1).trimmed();
		return messageId;
	}
	return QString();
}

void Skype::editMessage(int messageId, const QString &newMessage) {
	d->connection << QString("CHATMESSAGE %1 BODY %2").arg(messageId).arg(newMessage);
}

void Skype::setScanForUnread(bool value) {
	d->scanForUnread = value;
}

void Skype::makeCall(const QString &userId) {
	kDebug(SKYPE_DEBUG_GLOBAL);

	d->connection << QString("CALL %1").arg(userId);
}

void Skype::acceptCall(const QString &callId) {
	kDebug(SKYPE_DEBUG_GLOBAL);

	d->connection << QString("SET CALL %1 STATUS INPROGRESS").arg(callId);
}

void Skype::hangUp(const QString &callId) {
	kDebug(SKYPE_DEBUG_GLOBAL);

	d->connection << QString("SET CALL %1 STATUS FINISHED").arg(callId);
}

void Skype::toggleHoldCall(const QString &callId) {
	kDebug(SKYPE_DEBUG_GLOBAL);

	const QString &status = (d->connection % QString("GET CALL %1 STATUS").arg(callId)).section(' ', 3, 3).trimmed().toUpper();
	if ((status == "ONHOLD") || (status == "LOCALHOLD"))
		d->connection << QString("SET CALL %1 STATUS INPROGRESS").arg(callId);
	else
		d->connection << QString("SET CALL %1 STATUS ONHOLD").arg(callId);
}

bool Skype::isCallIncoming(const QString &callId) {
	const QString &type = (d->connection % QString("GET CALL %1 TYPE").arg(callId)).section(' ', 3, 3).trimmed().toUpper();
	return ((type == "INCOMING_P2P") || (type == "INCOMING_PSTN"));
}

void Skype::getSkypeOut() {
	const QString &curr = (d->connection % QString("GET PROFILE PSTN_BALANCE_CURRENCY")).section(' ', 2, 2).trimmed().toUpper();
	if (curr.isEmpty()) {
		emit skypeOutInfo(0, "");
	} else {
		int value = (d->connection % QString("GET PROFILE PSTN_BALANCE")).section(' ', 2, 2).trimmed().toInt();
		emit skypeOutInfo(value, curr);
	}
}

void Skype::enablePings(bool enabled) {
	kDebug(SKYPE_DEBUG_GLOBAL);

	d->pings = enabled;

	if (!enabled) {
		d->pingTimer->stop();
		return;
	}

	if (d->connStatus != csOffline) {
		d->pingTimer->start(1000);
	}
}

void Skype::ping() {
	d->connection << QString("PING");
}

void Skype::setBus(int bus) {
	d->bus = bus;
}

void Skype::setLaunchTimeout(int seconds) {
	d->launchTimeout = seconds;
}

void Skype::setSkypeCommand(const QString &command) {
	d->skypeCommand = command;
}

void Skype::setWaitConnect(int value) {
	d->waitBeforeConnect = value;
}

QString Skype::sendToChat(const QString &chat, const QString &message) {
	kDebug(SKYPE_DEBUG_GLOBAL);

	QString resp = d->connection % (QString("CHATMESSAGE %1 %2").arg(chat).arg(message));


	QString messageType = resp.section(' ', 0, 0).trimmed().toUpper();
	if (messageType == "CHATMESSAGE") {
		QString messageId = resp.section(' ', 1, 1).trimmed();
		return messageId;
	}
	return QString();
}

void Skype::getTopic(const QString &chat) {
	kDebug(SKYPE_DEBUG_GLOBAL);

	emit setTopic(chat, (d->connection % QString("GET CHAT %1 FRIENDLYNAME").arg(chat)).section(' ', 3).trimmed());
}

QString Skype::getMessageChat(const QString &message) {
	kDebug(SKYPE_DEBUG_GLOBAL);

	return (d->connection % QString("GET CHATMESSAGE %1 CHATNAME").arg(message)).section(' ', 3, 3).trimmed();
}

QStringList Skype::getChatUsers(const QString &chat) {
	kDebug(SKYPE_DEBUG_GLOBAL);

	const QString &me = getMyself();
	const QString &rawUsers = (d->connection % QString("GET CHAT %1 MEMBERS").arg(chat)).section(' ', 3).trimmed();
	const QStringList &users = rawUsers.split(' ');
	QStringList readyUsers;
	for (QStringList::const_iterator it = users.begin(); it != users.end(); ++it) {
		const QString &user = (*it).trimmed();
		if (user.toUpper() != me.toUpper())
			readyUsers.append(user);
	}

	return readyUsers;
}

QString Skype::getMyself() {
	return (d->connection % QString("GET CURRENTUSERHANDLE")).section(' ', 1, 1).trimmed();
}

void Skype::inviteUser(const QString &chatId, const QString &userId) {
	kDebug(SKYPE_DEBUG_GLOBAL) << chatId << userId;

	if (d->connection.protocolVer() <= 4) {
		KMessageBox::error(0L, i18n("This version of Skype does not support adding users to chat."), i18n("Skype Protocol"));
		return;
	}

	d->connection << QString("ALTER CHAT %1 ADDMEMBERS %2").arg(chatId).arg(userId);
}

QString Skype::createChat(const QString &users) {
	kDebug(SKYPE_DEBUG_GLOBAL);

	const QString &chatDesc = d->connection % QString("CHAT CREATE %1").arg(users);
	kDebug(SKYPE_DEBUG_GLOBAL) << "New chat ID: " << chatDesc.section(' ', 1, 1);
	return chatDesc.section(' ', 1, 1);
}

void Skype::leaveChat(const QString &chatId) {
	kDebug(SKYPE_DEBUG_GLOBAL);

	d->connection << QString("ALTER CHAT %1 LEAVE").arg(chatId);
}

void Skype::removeContact(const QString &contactId) {
	kDebug(SKYPE_DEBUG_GLOBAL);

	d->connection << QString("SET USER %1 BUDDYSTATUS 1").arg(contactId);
}

void Skype::addContact(const QString &contactId) {
	kDebug(SKYPE_DEBUG_GLOBAL);

	d->connection % QString("SET USER %1 BUDDYSTATUS 2").arg(contactId);//do NOT parse this so the contact won't be created automatically
}

void Skype::setAuthor(const QString &contactId, AuthorType author) {
	kDebug(SKYPE_DEBUG_GLOBAL);

	switch (author) {
		case Author:
			d->connection << QString("SET USER %1 ISBLOCKED FALSE").arg(contactId);
			d->connection << QString("SET USER %1 ISAUTHORIZED TRUE").arg(contactId);
			break;
		case Deny:
			d->connection << QString("SET USER %1 ISBLOCKED FALSE").arg(contactId);
			d->connection << QString("SET USER %1 ISAUTHORIZED FALSE").arg(contactId);
			break;
		case Block:
			d->connection << QString("SET USER %1 ISBLOCKED TRUE").arg(contactId);
			break;
	}
}

Skype::AuthorType Skype::getAuthor(const QString &contactId) {
	kDebug(SKYPE_DEBUG_GLOBAL);
	if ((d->connection % QString("GET USER %1 ISBLOCKED").arg(contactId)).section(' ', 3, 3).trimmed().toUpper() == "TRUE")
		return Block;
	else if ((d->connection % QString("GET USER %1 ISAUTHORIZED").arg(contactId)).section(' ', 3, 3).trimmed().toUpper() == "TRUE")
		return Author;
	else
		return Deny;
}

bool Skype::ableConference() {
	kDebug(SKYPE_DEBUG_GLOBAL);
	return false;
}

void Skype::fixGroups(bool loadOnly) {
	kDebug(SKYPE_DEBUG_GLOBAL);

	d->groupsContacts.clear();//remove all contacts and groups in memory
	d->groupsNames.clear();//remove all groups names

	//Fill d->groupsNames
	{
		QStringList groups = QString(d->connection % "SEARCH GROUPS CUSTOM").section(' ', 1).trimmed().split(", ");//get all ids of group

		for ( QStringList::iterator it = groups.begin(); it != groups.end(); ++it ){
			if ( ! (*it).trimmed().isEmpty() ){
				int groupID = (*it).trimmed().toInt();
				QString groupName = (d->connection % QString("GET GROUP %1 DISPLAYNAME").arg(groupID)).section(' ', 3).trimmed();
				kDebug(SKYPE_DEBUG_GLOBAL) << "Adding to memory group" << groupID << ":" << groupName;
				d->groupsNames.insert(groupName, groupID);
			}
		}
	}

	//Fill d->groupsContacts
	//Remove empty groups
	if ( d->groupsNames.count() > 0 ) {
		QList <int> groups = d->groupsNames.values();
		for ( QList <int>::iterator group = groups.begin(); group != groups.end(); ++group ) {
			QStringList groupusers = QString(d->connection % QString("GET GROUP %1 USERS").arg(*group)).section(' ', 3).trimmed().split(", ");//get all user in group (*group)
			if ( ( groupusers.count() == 0 || groupusers.first().trimmed().isEmpty() ) && ! loadOnly ) {//if group is empty, delete it
				kDebug(SKYPE_DEBUG_GLOBAL) << QString("Group %1 is empty, delete it").arg(*group);
				deleteGroup(*group);
			} else {
				kDebug(SKYPE_DEBUG_GLOBAL) << QString("Group %1 has users:").arg(*group) << groupusers;
				for ( QStringList::iterator user = groupusers.begin(); user != groupusers.end(); ++user ) {//search for all users in group (*group)
					if ( ! (*user).trimmed().isEmpty() ){//if username isnt empty add it to group
						kDebug(SKYPE_DEBUG_GLOBAL) << "Adding user " << (*user).trimmed() << "to memory group" << *group << ":" << d->groupsNames.key(*group);
						d->groupsContacts.insert( *group, (*user).trimmed() );//add user (*user) to group (*group)
					}
				}
			}
		}
	}

	//Check if user is only in one group
	if ( ! loadOnly ){
		QStringList users = d->groupsContacts.values();//get all users from all groups
		for ( QStringList::iterator user = users.begin(); user != users.end(); ++user ) {
			if ( ! (*user).isEmpty() ) {
				QList <int> groups = d->groupsContacts.keys(*user);//get groups for user (*user)
				kDebug(SKYPE_DEBUG_GLOBAL) << QString("User %1 is in memory groups:").arg(*user) << groups;
				if ( groups.count() > 1 ) {//if user is in more then one group, remove it from all groups except first
					for ( QList <int>::iterator group = groups.begin()+1; group != groups.end(); ++group ){
						kDebug(SKYPE_DEBUG_GLOBAL) << QString("User %1 is in more then one memory groups, removing from memory group %2").arg(*user).arg(*group);
						removeFromGroup(*user, *group);
					}
				}
			}
		}
	}

	//Check if groups doesnt have same names
	if ( ! loadOnly ) {
		QStringList groupsNames = d->groupsNames.keys();//get all groups names, if group has more ides then one, add to List this group only one
		for ( QStringList::iterator group = groupsNames.begin(); group != groupsNames.end(); ++group ){
			QList <int> groupIdes = d->groupsNames.values(*group);
			if ( groupIdes.count() > 1 ){
				kDebug(SKYPE_DEBUG_GLOBAL) << "Group" << *group << "has more ides then one:" << groupIdes << ", so merge all these groups to" << groupIdes.last();
				for ( QList <int>::iterator groupID = groupIdes.begin(); groupID != groupIdes.end() - 1; ++groupID ) {
					QStringList users = d->groupsContacts.values(*groupID);
					for ( QStringList::iterator user = users.begin(); user != users.end(); ++user ) {
						removeFromGroup((*user), (*groupID));
						addToGroup((*user), groupIdes.last());
					}
					deleteGroup(*groupID);
				}
				groupsNames.removeAll(*group);
			}
		}
	}

}

int Skype::getContactGroupID(const QString &name) {
	kDebug(SKYPE_DEBUG_GLOBAL) << name;
	return d->groupsContacts.key(name, -1); //get group id from d->groupsContacts
}

void Skype::stopVoiceMail(int id) {
	kDebug(SKYPE_DEBUG_GLOBAL) << id;
	d->connection << QString("ALTER VOICEMAIL %1 STOPRECORDING").arg(id);
}

void Skype::removeFromGroup(const QString &name, int groupID) {
	kDebug(SKYPE_DEBUG_GLOBAL) << name << groupID;
	d->connection << QString("ALTER GROUP %1 REMOVEUSER %2").arg(groupID).arg(name);
	d->groupsContacts.remove(groupID, name);
}

void Skype::addToGroup(const QString &name, int groupID) {
	kDebug(SKYPE_DEBUG_GLOBAL) << name << groupID;
	d->connection << QString("ALTER GROUP %1 ADDUSER %2").arg(groupID).arg(name);
	d->groupsContacts.insert(groupID, name);
}

void Skype::createGroup(const QString &name) {
	kDebug(SKYPE_DEBUG_GLOBAL) << name;
	d->connection << QString("CREATE GROUP %1").arg(name);
	fixGroups(true); ///TODO: Find better way to get group id and create memory group too without fixGroups()
}

void Skype::deleteGroup(int groupID) {
	kDebug(SKYPE_DEBUG_GLOBAL) << groupID << ":" << d->groupsNames.key(groupID);
	d->connection << QString("DELETE GROUP %1").arg(groupID);
	d->groupsNames.remove(d->groupsNames.key(groupID), groupID);
	d->groupsContacts.remove(groupID);
}

void Skype::renameGroup(int groupID, const QString &newName) {
	kDebug(SKYPE_DEBUG_GLOBAL) << groupID;
	d->connection << QString("SET GROUP %1 DISPLAYNAME %2").arg(groupID).arg(newName);
	d->groupsNames.remove(d->groupsNames.key(groupID));
	d->groupsNames.insert(newName, groupID);
}

int Skype::getGroupID(const QString &groupname) {
	kDebug(SKYPE_DEBUG_GLOBAL) << groupname;
	return d->groupsNames.value(groupname, -1); //get group id from d->groupsNames
}

QString Skype::getGroupName(int groupID) {
	kDebug(SKYPE_DEBUG_GLOBAL) << groupID;

	if (groupID == -1) //If groupID is empty return empty name
		return QString();

	return d->groupsNames.key(groupID, QString()); //get group name from d->groupsNames
}

QString Skype::getContactDisplayName(const QString &user) {
	kDebug(SKYPE_DEBUG_GLOBAL) << user;
	return (d->connection % QString("GET USER %1 DISPLAYNAME").arg(user)).section(' ', 3).trimmed();
}

void Skype::setContactDisplayName(const QString &user, const QString &name) {
	kDebug(SKYPE_DEBUG_GLOBAL);
	d->connection % QString("SET USER %1 DISPLAYNAME %2").arg(user).arg(name);
}

bool Skype::openFileTransfer(const QString &user, const QString &url) {
	kDebug(SKYPE_DEBUG_GLOBAL) << user << url;
	if ( (d->connection % QString("OPEN FILETRANSFER %1 IN %2").arg(user).arg(url)).trimmed() == "OK" )
		return true;
	else
		return false;
}

QStringList Skype::searchUsers(const QString &string) {
	kDebug(SKYPE_DEBUG_GLOBAL) << string;
	return (d->connection % QString("SEARCH USERS %1").arg(string)).section(' ', 1).trimmed().split(' ');
}

QStringList Skype::searchActiveCalls()
{
	return (d->connection % QString("SEARCH ACTIVECALLS")).section(' ', 1).trimmed().split(' ');
}


bool Skype::supportVideo(const QString &user) {
	kDebug(SKYPE_DEBUG_GLOBAL) << user;
	if ( (d->connection % QString("GET USER %1 IS_VIDEO_CAPABLE").arg(user)).section(' ', 3).trimmed().toUpper() == "TRUE" )
		return true;
	else
		return false;
}

void Skype::startSendingVideo(const QString &callId) {
	kDebug(SKYPE_DEBUG_GLOBAL) << callId;
	d->connection << QString("ALTER CALL %1 START_VIDEO_SEND").arg(callId);
}

void Skype::stopSendingVideo(const QString &callId) {
	kDebug(SKYPE_DEBUG_GLOBAL) << callId;
	d->connection << QString("ALTER CALL %1 STOP_VIDEO_SEND").arg(callId);
}

#include "skype.moc"
