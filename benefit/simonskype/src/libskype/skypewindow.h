/*
    skypewindow.h - Kopete Skype Protocol

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

#ifndef SKYPEWINDOW_H
#define SKYPEWINDOW_H

#include <QObject>
#include <QWidgetList> //for WId
#include <QProcess> //for Q_PID

class SkypeWindowPrivate;

/**
 * @author Pali Rohár
 * This class contains methods for manipulating with linux skype client
 * !!! Only work if ENGLISH language is setup in skype client and kwin is running !!!
 */
class SkypeWindow : public QObject
{
	Q_OBJECT
	public:
		/**
		 * Constructor
		 * @param pid process id of skype
		 * If you use 0, all functions will check all processes
		 */
		SkypeWindow(Q_PID pid = 0);
		/**
		 * Destructor
		 **/
		~SkypeWindow();
		/**
		 * This will try hide call dialog created by linux skype client
		 * @param user audio/video call session with this skype user name
		 */
		void hideCallDialog(const QString &user);
		/**
		 * This show hidden call dialog
		 * @param user audio/video call session with this skype user name
		 */
		void showCallDialog(const QString &user);
		/**
		 * This permanently delete call dialog
		 * @param user audio/video call session with this skype user name
		 */
		void deleteCallDialog(const QString &user);
		/**
		 * This will try move webcam stream widget from linux skype client to other specified window at position x, y
		 * @param user audio/video call session with this skype user name
		 * @param otherWId Oter window Id for move
		 * @param x, y position
		 */
		void moveWebcamWidget(const QString &user, WId otherWId, int x = 0, int y = 0);
		/**
		 * This revert webcam stream widget back to skype call dialog
		 * @param user audio/video call session with this skype user name
		 */
		void revertWebcamWidget(const QString &user);
	private:
		/**
		 * This check if window is skype call dialog
		 * @param user audio/video call session with this skype user name
		 * @param wid window id for check
		 * @return true if it is
		 */
		bool isCallDialog(const QString &user, WId wid);
		/**
		 * This get window id of call dialog from linux skype client
		 * @param user audio/video call session with this skype user name
		 * @return window id of call dialog
		 */
		WId getCallDialogWId(const QString &user);
		/**
		 * This check if window is skype webcam stream widget
		 * @param wid window id for check
		 * @return true if it is
		 */
		bool isWebcamWidget(WId wid);
		/**
		 * This will try find webcam stream widget
		 * It browse all children windows of actualWId
		 * @param actualWId window id for start
		 * @return webcam stream widget
		 */
		WId getWebcamWidgetWId(WId actualWId);
		///d pointer
		SkypeWindowPrivate * d;
	private slots:
		///Called when new window is added to kwin manager
		void windowAdded(WId wid);
	signals:
		///Emited when we found skype call dialog
		void foundCallDialog();
};

#endif
