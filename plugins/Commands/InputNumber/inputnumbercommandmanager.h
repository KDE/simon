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

#ifndef INPUTNUMBERCOMMANDMANAGER_H
#define INPUTNUMBERCOMMANDMANAGER_H

#include <commandpluginbase/commandmanager.h>
#include <simonactions/greedyreceiver.h>
#include <QVariantList>
#include "ui_inputnumberwidget.h"
class QDialog;
class KAction;

/**
 *	@class InputNumberCommandManager
 *	@brief Manager for the inputnumber commands
 *
 *	@version 0.1
 *	@date 20.05.2008
 *	@author Peter Grasch
 */
class InputNumberCommandManager : public CommandManager, public GreedyReceiver {
Q_OBJECT
private:
	Ui::InputNumberDlg ui;
	QDialog *widget;
	static QStringList numberIdentifiers;
	KAction *activateAction;

private slots:
	void deregister();
	void ok();
	void back();
	void cancel();
	void processRequest(int number);
	void sendComma();
	void send0() { processRequest(0); }
	void send1() { processRequest(1); }
	void send2() { processRequest(2); }
	void send3() { processRequest(3); }
	void send4() { processRequest(4); }
	void send5() { processRequest(5); }
	void send6() { processRequest(6); }
	void send7() { processRequest(7); }
	void send8() { processRequest(8); }
	void send9() { processRequest(9); }

protected:
	bool trigger(const QString& triggerName);

public slots:
	bool greedyTrigger(const QString& inputText);
	void activate();

public:
	const KIcon icon() const;
	bool addCommand(Command *) { return false; }
	const QString name() const;
	bool load();
	bool save();
	CommandConfiguration* getConfigurationPage();

    /**
    * @brief Constructor
    * 
    *	@author Peter Grasch
    */
    InputNumberCommandManager(QObject *parent, const QVariantList& args);

    
    ~InputNumberCommandManager();

};

#endif
