/*
    skypewindow.cpp - Kopete Skype Protocol

    Copyright (c) 2009 by Pali Rohár <pali.rohar@gmail.com>

    *************************************************************************
    *                                                                       *
    * This library is free software; you can redistribute it and/or         *
    * modify it under the terms of the GNU General Public                   *
    * License as published by the Free Software Foundation; either          *
    * version 2 of the License, or (at your option) any later version.      *
    *                                                                       *
    *************************************************************************
*/

#include <QString>
#include <QRegExp>
#include <QHash>
#include <QX11Info>
#include <QList>
#include <QTimer>
#include <QEventLoop>
#include <QWaitCondition>
#include <QMutex>

#include <KWindowSystem>
#include <KWindowInfo>
#include <NET>
#include <KDebug>

#include <X11/Xlib.h>
#include <X11/Xatom.h>

#include "skypewindow.h"

class SkypeWindowPrivate
{
	public:
		SkypeWindowPrivate() {
			pid = 0;
			searchWindowsFoundWId = 0;
			searchWindows = false;
			
		}
		Q_PID pid;
		WId searchWindowsFoundWId;
		QString searchWindowsUser;
		bool searchWindows;
		QHash <const QString, WId> hiddenWindows;
		QHash <WId, WId> webcamStreams;
};

SkypeWindow::SkypeWindow(Q_PID pid) {
	kDebug(SKYPE_DEBUG_GLOBAL) << pid;
	d = new SkypeWindowPrivate;
	d->pid = pid;
	connect( KWindowSystem::self(), SIGNAL(windowAdded(WId)), this, SLOT(windowAdded(WId)) );
}

SkypeWindow::~SkypeWindow() {
	kDebug(SKYPE_DEBUG_GLOBAL);
	disconnect( KWindowSystem::self(), SIGNAL(windowAdded(WId)), this, SLOT(windowAdded(WId)) );
	//TODO: revert back all webcam streams and delete or show hidden dialogs if it is needed
	if ( ! d->hiddenWindows.isEmpty() ) {
		kDebug(SKYPE_DEBUG_GLOBAL) << "We have" << d->hiddenWindows.size() << "hidden dialogs";
	}
	delete d;
}

bool SkypeWindow::isCallDialog(const QString &user, WId wid) {
	kDebug(SKYPE_DEBUG_GLOBAL) << user << wid;

	Atom atom = XInternAtom(QX11Info::display(), "_NET_WM_PID", True); //get atom properties for process id
	Atom actual_type; int actual_format; unsigned long nitems, bytes_after; //dont use
	unsigned char *prop; //array of return value of pid
	int status = XGetWindowProperty(QX11Info::display(), wid, atom, 0, 1024, False, AnyPropertyType, &actual_type, &actual_format, &nitems, &bytes_after, &prop); //try get process id from window id

	Q_PID pid = 0;
	if ( status == 0 ) {
		if ( prop ) { //if all ok, we have process id in prop
			pid = prop[1] * 256;
			pid += prop[0];
		}
		XFree(prop); //free memory
	}

	if ( d->pid == 0 || d->pid == pid ) { //check if process id of window id is same as in constructor
		QString userExp = user;
		if ( user.startsWith("+") ) { //check if it is phone number, it may start with "+"
			userExp.insert(0, "\\");
		}
		//english regular exp for skype call dialog
		QString regExp1 = QString::fromUtf8("^Call with %1$").arg(userExp);
		QString regExp2 = QString::fromUtf8("^[0-9]{2}:[0-9]{2} \\| Call with %1$").arg(userExp);
		QString regExp3 = QString::fromUtf8("^Call Finished \\| Call with %1$").arg(userExp);
		QString regExpSingleConference = QString::fromUtf8("^Conference Call - 1 participants$");
		QString windowName = KWindowSystem::windowInfo(wid, NET::WMName | NET::WMVisibleName).name(); //get window title from window id

		if ( QRegExp(regExpSingleConference).exactMatch(windowName) ) { //oups, sometime get skype conference call with one user and after 100ms skype fix, so try again get window title name
			int count = 10;
			while ( count >= 0 && QRegExp(regExpSingleConference).exactMatch(windowName) ) {
				kDebug(SKYPE_DEBUG_GLOBAL) << "Found Conference Call, waiting if it skype change";
				QWaitCondition sleep; QMutex mutex; sleep.wait(&mutex, 50);
				windowName = KWindowSystem::windowInfo(wid, NET::WMName | NET::WMVisibleName).name();
				--count;
			}
		}

		if ( QRegExp(regExp1).exactMatch(windowName) || QRegExp(regExp2).exactMatch(windowName) || QRegExp(regExp3).exactMatch(windowName) ) { //check if pass to english dialog title with user name
			kDebug(SKYPE_DEBUG_GLOBAL) << "It is skype dialog";
			return true;
		} else {
			kDebug(SKYPE_DEBUG_GLOBAL) << "pid" << pid << "windowName" << windowName << "isnt skype call dialog for name" << user;
		}
	} else {
		kDebug(SKYPE_DEBUG_GLOBAL) <<  "pid" << pid << "isnt skype call dialog for name" << user;
	}


	return false;
}

void SkypeWindow::windowAdded(WId wid) {
	kDebug(SKYPE_DEBUG_GLOBAL) << wid;
	if ( d->searchWindows ) {
		if ( ! isCallDialog(d->searchWindowsUser, wid) )
			return;
		d->searchWindowsFoundWId = wid;
		emit(foundCallDialog());
	}
	if ( ! d->hiddenWindows.key(wid, QString()).isEmpty() ) {
		kDebug(SKYPE_DEBUG_GLOBAL) << "Skype call dialog apper again, hide it id" << wid;
		XUnmapWindow(QX11Info::display(), wid); //hide it again
	}
}

WId SkypeWindow::getCallDialogWId(const QString &user) {
	kDebug(SKYPE_DEBUG_GLOBAL) << user;
	WId wid = d->hiddenWindows.value(user, 0); //first check if it not hidded
	if ( wid != 0 && isCallDialog(user, wid) ) //check if this this window still exists
		return wid;
	else
		d->hiddenWindows.remove(user); //if no delete it from list
	QList<WId>::ConstIterator it;
	for ( it = KWindowSystem::windows().begin(); it != KWindowSystem::windows().end(); ++it ) { //get all active windows id
		if ( isCallDialog(user, *it) ) {
			kDebug(SKYPE_DEBUG_GLOBAL) << "Found skype call dialog WId" << *it;
			return *it;
		}
	}
	d->searchWindowsUser = user;
	d->searchWindowsFoundWId = 0;
	d->searchWindows = true;
	QEventLoop * loop = new QEventLoop;
	connect( this, SIGNAL(foundCallDialog()), loop, SLOT(quit()) );
	QTimer::singleShot( 1000, loop, SLOT(quit()) );
	loop->exec();
	disconnect( this, SIGNAL(foundCallDialog()), loop, SLOT(quit()) );
	delete loop;
	wid = d->searchWindowsFoundWId;
	d->searchWindowsUser.clear();
	d->searchWindowsFoundWId = 0;
	d->searchWindows = false;

	if ( wid != 0 ) {
		kDebug(SKYPE_DEBUG_GLOBAL) << "Found skype call dialog WId" << wid;
		return wid;
	}
	return 0; //We dont find it
}

void SkypeWindow::hideCallDialog(const QString &user) {
	kDebug(SKYPE_DEBUG_GLOBAL) << user;
	WId wid = getCallDialogWId(user);
	if ( wid == 0 ) {
		kDebug(SKYPE_DEBUG_GLOBAL) << "Cant find WId of skype call dialog";
		return;
	}
	kDebug(SKYPE_DEBUG_GLOBAL) << "Hide skype call dialog id" << wid;
	XUnmapWindow(QX11Info::display(), wid); //window wid passed test for reg exp, maybe it is skype call dialog, hide it now
	d->hiddenWindows.insert(user, wid);
}

void SkypeWindow::showCallDialog(const QString &user) {
	kDebug(SKYPE_DEBUG_GLOBAL) << user;
	WId wid = d->hiddenWindows.value(user, 0);
	if ( wid == 0 ) {
		kDebug(SKYPE_DEBUG_GLOBAL) << "Cant find WId of skype call dialog, maybe now doesnt exist";
		return;
	}
	kDebug(SKYPE_DEBUG_GLOBAL) << "Show skype call dialog WId" << wid;
	XMapWindow(QX11Info::display(), wid); //map this window
	KWindowSystem::activateWindow(wid); //active this window in kde
	d->hiddenWindows.remove(user);
}

void SkypeWindow::deleteCallDialog(const QString &user) {
	kDebug(SKYPE_DEBUG_GLOBAL) << user;
	WId wid = d->hiddenWindows.value(user, 0);
	if ( wid == 0 ) {
		kDebug(SKYPE_DEBUG_GLOBAL) << "Cant find WId of skype call dialog, maybe is now deleted";
		return;
	}
	kDebug(SKYPE_DEBUG_GLOBAL) << "Delete skype call dialog id" << wid;
	XDestroyWindow(QX11Info::display(), wid); //permanently destroy this window and all subwindows
	d->hiddenWindows.remove(user);
}

bool SkypeWindow::isWebcamWidget(WId wid) {
	kDebug(SKYPE_DEBUG_GLOBAL) << wid;
	XWindowAttributes window_attributes; //window attributes
	int status = XGetWindowAttributes(QX11Info::display(), wid, &window_attributes); //get attributes from window wid
	kDebug(SKYPE_DEBUG_GLOBAL) << "Attributes: width =" << window_attributes.width << "height =" << window_attributes.height << "status =" << status;
	if ( status != 0 && window_attributes.width == 320 && window_attributes.height == 240 ) { //if all is ok and width is 320 and height is 240 it may be webcam window
		kDebug(SKYPE_DEBUG_GLOBAL) << "It is webcam widget";
		return true;
	}
	return false;
}

WId SkypeWindow::getWebcamWidgetWId(WId actualWId) {
	kDebug(SKYPE_DEBUG_GLOBAL) << actualWId;
	if ( isWebcamWidget(actualWId) ) //first check if it is webcam widget
		return actualWId;
	Window root, parent; //dont use
	Window * children; //array of all children windows
	unsigned int nchildren; //count children windows
	int status = XQueryTree(QX11Info::display(), actualWId, &root, &parent, &children, &nchildren); //get all children windows
	if ( status == 0 ) {
		kDebug(SKYPE_DEBUG_GLOBAL) << "Cant get children windows";
		return 0;
	}
	for ( unsigned int i=0; i<nchildren; ++i ) { //browse all child windows and try find webcam widget
		WId newWId = getWebcamWidgetWId(children[i]); //try get windows id of child i
		if ( newWId != 0 ) {
			XFree(children); //free memory
			return newWId;
		}
	}
	XFree(children); //free memory
	return 0;
}

void SkypeWindow::moveWebcamWidget(const QString &user, WId otherWId, int x, int y) {
	kDebug(SKYPE_DEBUG_GLOBAL) << user << otherWId << x << y;
	WId callDialogWId = getCallDialogWId(user);
	if ( callDialogWId == 0 ) {
		kDebug(SKYPE_DEBUG_GLOBAL) << "Cant find WId of skype call dialog";
		return;
	}
	WId webcamWidgetWId = getWebcamWidgetWId(callDialogWId);
	if ( webcamWidgetWId == 0 ) {
		kDebug(SKYPE_DEBUG_GLOBAL) << "Cant find WId of skype webcam widget, maybe isnt incomming webcam stream";
		return;
	}
	//get parent of webcam stream widget
	Window root, parent;
	Window * children;
	unsigned int nchildren;
	int status = XQueryTree(QX11Info::display(), webcamWidgetWId, &root, &parent, &children, &nchildren);
	if ( status == 0 ) {
		kDebug(SKYPE_DEBUG_GLOBAL) << "Cant find parent of skype webcam widget";
		return;
	}
	XFree(children);
	d->webcamStreams.insert(webcamWidgetWId, parent);
	XReparentWindow(QX11Info::display(), webcamWidgetWId, otherWId, x, y); //move webcam widget to other window at position x, y
	XMapWindow(QX11Info::display(), webcamWidgetWId); //map this window to show webcam stream
	//TODO: disable right mouse click events in window webcamWidgetWId
}

void SkypeWindow::revertWebcamWidget(const QString &user) {
	kDebug(SKYPE_DEBUG_GLOBAL) << user;
	WId callDialogWId = getCallDialogWId(user);
	if ( callDialogWId == 0 ) {
		kDebug(SKYPE_DEBUG_GLOBAL) << "Cant find WId of skype call dialog";
		return;
	}
	WId webcamWidgetWId = getWebcamWidgetWId(callDialogWId);
	if ( callDialogWId == 0 ) {
		kDebug(SKYPE_DEBUG_GLOBAL) << "Cant find WId of skype webcam widget, maybe isnt incomming webcam stream";
		return;
	}
	WId parentWId = d->webcamStreams.value(webcamWidgetWId, 0);
	if ( parentWId == 0 ) {
		kDebug(SKYPE_DEBUG_GLOBAL) << "Cant find parent of skype webcam widget";
		return;
	}
	d->webcamStreams.remove(webcamWidgetWId);
	XReparentWindow(QX11Info::display(), webcamWidgetWId, parentWId, 6+1+3+2, 6+1+3+24); //Fix correct position, it is +2+24? (6+1+3 is frame size of skype client call dialog, +2+24 is position of webcam stream)
	XUnmapWindow(QX11Info::display(), webcamWidgetWId);
}

#include "skypewindow.moc"
