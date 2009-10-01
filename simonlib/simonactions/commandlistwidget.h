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

#ifndef SIMON_COMMANDLISTWIDGET_H_055CB08189FE40A4B28221757CDEB148
#define SIMON_COMMANDLISTWIDGET_H_055CB08189FE40A4B28221757CDEB148

#include "actionlib_export.h"
#include <commandpluginbase/command.h>
#include <QList>
#include <QTimer>
#include <QWidget>
#include <KUrl>

class QCloseEvent;
class KPushButton;
class QTableWidget;
class QProgressBar;
class QShowEvent;

/**
 *	@class CommandListWidget
 *	@brief Describes a list command; Consists of several other commands
 *	
 *	@version 0.1
 *	@date 19.05.2008
 *	@author Peter Grasch
 */
class SIMONACTIONS_EXPORT CommandListWidget : public QWidget {
Q_OBJECT

signals:
	void runRequest(int index);
	void canceled();

private:
	QTimer toggleAfterTimeoutTimer;
	QTimer blinkTimer;
	int indexToSelectAfterTimeout;
	void selectAfterTimeoutIndex(bool);

private slots:
	void runCommand();
	void blink();
	void timeoutReached();

protected:
	KPushButton *pbCancel;
	QTableWidget *twCommands;
	QProgressBar *pbAutomaticSelection;
	
	void closeEvent(QCloseEvent *);
	void showEvent(QShowEvent *);

public:
	enum Flag {
		HasBack=0x1,
		HasNext=0x2
	};

	Q_DECLARE_FLAGS(Flags, Flag)
	
	/**
	* @brief Constructor
	* 
	*	@author Peter Grasch
	*/
	CommandListWidget();
	
	void init(const QStringList& iconsrcs, const QStringList commands, Flags flags);

	void runRequestSent();

	void selectAfterTimeout(int index, int timeout /* in ms */);
	void abortTimeoutSelection();

    	virtual ~CommandListWidget();

private:
	Flags currentFlags;
	bool runRequestEmitted;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(CommandListWidget::Flags)
#endif
