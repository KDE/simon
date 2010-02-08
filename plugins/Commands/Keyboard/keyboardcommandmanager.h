/*
 *   Copyright (C) 2009 Peter Grasch <grasch@simon-listens.org>
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

#ifndef SIMON_KEYBOARDCOMMANDMANAGER_H_7A7B9100FF5245329569C1B540119C37
#define SIMON_KEYBOARDCOMMANDMANAGER_H_7A7B9100FF5245329569C1B540119C37

#include <simonscenarios/commandmanager.h>
#include <simonscenarios/commandlauncher.h>
#include <simonactions/greedyreceiver.h>
#include <QVariantList>
#include <KXMLGUIClient>
#include "ui_keyboardwidget.h"
class QWidget;
class KAction;
class KeyboardConfiguration;
class KeyboardSet;
class KeyboardSetContainer;

/**
 *	@class KeyboardCommandManager
 *	@brief Manager for the keyboard commands
 *
 *	@version 0.1
 *	@date 20.05.2008
 *	@author Peter Grasch
 */
class KeyboardCommandManager : public CommandManager, public GreedyReceiver {
Q_OBJECT
private:
	Ui::KeyboardDlg ui;
	QWidget *keyboardWidget;
	static QStringList numberIdentifiers;
	KAction *activateAction;

	KeyboardSet *keyboardSet;
	KeyboardSetContainer *setContainer;
	bool switchToTab(const QString& tabName, bool caseSensitivity);
	QString getCurrentTabName();

private slots:
	void selectNumber();
	void writeOutNumber();
	void numberBackSpace();

	void shift(bool down);
	void capsLock(bool down);
	void control(bool down);
	void alt(bool down);
	void altGr(bool down);
	void super(bool down);
	void backSpace();
	void returnPressed();

	void deregister();
	void ok();
	void cancel();
	void processRequest(int number);
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

	KeyboardConfiguration* getKeyboardConfiguration();

	void untoggleShift();

protected:
	bool trigger(const QString& triggerName);

public slots:
	bool greedyTrigger(const QString& inputText);
	void activate();
	void rebuildGui();

public:
	KeyboardSetContainer* getKeyboardSetContainer() { return setContainer;}
	const QString preferredTrigger() const;
	const QString iconSrc() const;
	const QString name() const;
	bool deSerializeConfig(const QDomElement& elem);
	void setFont(const QFont& font);

	QList<CommandLauncher*> launchers() const;
    /**
    * @brief Constructor
    * 
    *	@author Peter Grasch
    */
    KeyboardCommandManager(QObject* parent, const QVariantList& args);

    
    ~KeyboardCommandManager();

};

#endif
