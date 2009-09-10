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
#include "keyboardcommandmanager.h"
#include "keyboardconfiguration.h"
#include "keyboardset.h"
#include "keyboardsetcontainer.h"
#include "flowlayout.h"
#include <eventsimulation/eventhandler.h>
#include <simonactions/actionmanager.h>
#include <unistd.h>
#include <QDesktopWidget>
#include <QDialog>
#include <QPoint>
#include <QSize>
#include <KLocalizedString>
#include <KAction>
#include <KActionCollection>

K_PLUGIN_FACTORY( KeyboardCommandPluginFactory, 
			registerPlugin< KeyboardCommandManager >(); 
		)

K_EXPORT_PLUGIN( KeyboardCommandPluginFactory("simonkeyboardcommand") )

QStringList KeyboardCommandManager::numberIdentifiers;

KeyboardCommandManager::KeyboardCommandManager(QObject* parent, const QVariantList& args) : CommandManager(parent, args),
	keyboardWidget(new QWidget(0, Qt::Dialog|Qt::WindowStaysOnTopHint)),
	activateAction(new KAction(this)),
	keyboardSet(NULL)
{
	setContainer = new KeyboardSetContainer();

	keyboardWidget->setWindowIcon(KIcon("input-keyboard"));
        ui.setupUi(keyboardWidget);
	keyboardWidget->hide();

	activateAction->setText(i18n("Activate Keyboard"));
	activateAction->setIcon(KIcon("input-keyboard"));
	connect(activateAction, SIGNAL(triggered(bool)),
		this, SLOT(activate()));
	guiActions<<activateAction;
}

void KeyboardCommandManager::activate()
{
	QDesktopWidget* tmp = QApplication::desktop();

	QPoint pos =KeyboardConfiguration::getInstance(setContainer)->keyboardPosition();
	QSize size = KeyboardConfiguration::getInstance(setContainer)->keyboardSize();

	if (!pos.isNull() && !size.isNull()) {
		keyboardWidget->move(pos);
		keyboardWidget->resize(size);
	} else {
		int x,y;
		x=(tmp->width()/2) - (keyboardWidget->width()/2);
		y=((tmp->height()*3)/4)-(keyboardWidget->height()/2);
		keyboardWidget->move(x, y);
	}

	keyboardWidget->show();
	startGreedy();
}

void KeyboardCommandManager::deregister()
{
	kDebug() << "deregister begin";
	KeyboardConfiguration::getInstance(setContainer)->saveKeyboardGeometry(keyboardWidget->pos(),
										keyboardWidget->size());
	stopGreedy();
	kDebug() << "deregister end";
}

const KIcon KeyboardCommandManager::icon() const
{
	return KIcon("input-keyboard");
}


const QString KeyboardCommandManager::name() const
{
	return i18n("Keyboard");
}



void KeyboardCommandManager::cancel()
{
	keyboardWidget->hide();
}

void KeyboardCommandManager::processRequest(int number)
{
	ui.leNumber->setText(ui.leNumber->text()+QString::number(number));
}

void KeyboardCommandManager::ok()
{
	keyboardWidget->hide();
	//usleep(300000);
	//EventHandler::getInstance()->sendWord(ui.leNumber->text());
}

bool KeyboardCommandManager::greedyTrigger(const QString& inputText)
{
	kDebug() << "Processing " << inputText;

	bool caseSensitivityBool = KeyboardConfiguration::getInstance(setContainer)->caseSensitive();
	Qt::CaseSensitivity caseSensitivity = caseSensitivityBool ? Qt::CaseSensitive : Qt::CaseInsensitive;
        if (QString::compare(inputText, ui.pbOk->text(), caseSensitivity)==0) {
		ui.pbOk->animateClick();
		return true;
	}

	kDebug() << "About to process special keys " << inputText;

	//special keys
        if (ui.pbShift->isVisible() && QString::compare(inputText, ui.pbShift->text(), caseSensitivity)==0) {
		ui.pbShift->animateClick();
		return true;
	}
        if (ui.pbCapsLock->isVisible() && QString::compare(inputText, ui.pbCapsLock->text(), caseSensitivity)==0) {
		ui.pbCapsLock->animateClick();
		return true;
	}
        if (ui.pbControl->isVisible() && QString::compare(inputText, ui.pbControl->text(), caseSensitivity)==0) {
		ui.pbControl->animateClick();
		return true;
	}
        if (ui.pbBackspace->isVisible() && QString::compare(inputText, ui.pbBackspace->text(), caseSensitivity)==0) {
		ui.pbBackspace->animateClick();
		return true;
	}

	kDebug() << "About to process numpad " << inputText;

	//numpad?
	if (KeyboardConfiguration::getInstance(setContainer)->showNumpad()) {
		if (QString::compare(inputText, ui.pbSelectNumber->text(), caseSensitivity)==0) {
			ui.pbDecimalSeparator->animateClick();
			return true;
		}
		if (QString::compare(inputText, ui.pbDecimalSeparator->text(), caseSensitivity)==0) {
			ui.pbDecimalSeparator->animateClick();
			return true;
		}

		bool ok=false;
		int index = inputText.toInt(&ok);
		if (!ok)
		{
			while ((index < numberIdentifiers.count()) && (numberIdentifiers.at(index).toUpper() != inputText.toUpper()))
				index++;

			if (index < numberIdentifiers.count()) ok = true;
		}

		if (ok) {
			//recognized >index<
			switch (index) {
				case 0: send0(); break;
				case 1: send1(); break;
				case 2: send2(); break;
				case 3: send3(); break;
				case 4: send4(); break;
				case 5: send5(); break;
				case 6: send6(); break;
				case 7: send7(); break;
				case 8: send8(); break;
				case 9: send9(); break;

			}
		}
	}

	//no widgets?
	if (ui.twTabs->currentIndex() == -1) return false;

	QStringList tabNames = keyboardSet->getAvailableTabs();
	for (int i=0; i < tabNames.count(); i++) {
		if (tabNames[i].toUpper() == inputText.toUpper()) {
			ui.twTabs->setCurrentIndex(i);
			return true;
		}
	}
	
	kDebug() << "Available tabs: " << tabNames << " of set " << keyboardSet->getSetName();
	kDebug() << "Get tab: " << ui.twTabs->currentIndex();
	return keyboardSet->triggerButton(tabNames[ui.twTabs->currentIndex()], inputText, 
			caseSensitivityBool);
}


void KeyboardCommandManager::rebuildGui()
{
	keyboardSet = KeyboardConfiguration::getInstance(setContainer)->getStoredKeyboardSet();
	if (!keyboardSet) return;

	keyboardWidget->setWindowTitle(keyboardSet->getSetName());
	
	// clear tab
	while(ui.twTabs->count() > 0) {
		QWidget *w = ui.twTabs->widget(0);
		ui.twTabs->removeTab(0);
		QLayout *lay = w->layout();
		//remove all the buttons so they won't be deleted when deleting the widget
		if (lay) {
			while (lay->count() > 0) {
				QLayoutItem *item = lay->takeAt(0);
				QWidget *itemWidget = item->widget();
				if (itemWidget)  itemWidget->setParent(0);
				delete item; //leaves the widget intact)
			}
			delete lay;
		}
		w->deleteLater();
	}

	QStringList tabs = keyboardSet->getAvailableTabs();
	foreach (const QString& tabName, tabs) {
		QWidget *w = new QWidget(ui.twTabs);
		FlowLayout *flow = new FlowLayout(w);
		//QHBoxLayout *flow = new QHBoxLayout(w);

		QList<KeyboardButton*> buttons = keyboardSet->getTabButtons(tabName);
		foreach (KeyboardButton* button, buttons) {
			flow->addWidget(button);
			button->show();
		}
		w->setLayout(flow);

		ui.twTabs->addTab(w, tabName);
	}

	//special keys
	ui.pbControl->setVisible(KeyboardConfiguration::getInstance(setContainer)->control());
	ui.pbControl->setText(KeyboardConfiguration::getInstance(setContainer)->controlTrigger());
	ui.pbShift->setVisible(KeyboardConfiguration::getInstance(setContainer)->shift());
	ui.pbShift->setText(KeyboardConfiguration::getInstance(setContainer)->shiftTrigger());
	ui.pbCapsLock->setVisible(KeyboardConfiguration::getInstance(setContainer)->capsLock());
	ui.pbCapsLock->setText(KeyboardConfiguration::getInstance(setContainer)->capsLockTrigger());
	ui.pbBackspace->setVisible(KeyboardConfiguration::getInstance(setContainer)->backspace());
	ui.pbBackspace->setText(KeyboardConfiguration::getInstance(setContainer)->backspaceTrigger());

	//characters
	if (KeyboardConfiguration::getInstance(setContainer)->showNumpad()) {
		kDebug() << "Showing numpad...";
		ui.gbNumPad->show();
		ui.pbNumberBackspace->setText(KeyboardConfiguration::getInstance(setContainer)->numberBackspaceTrigger());
		ui.pbSelectNumber->setVisible(KeyboardConfiguration::getInstance(setContainer)->enableNumberBasedSelection());
		ui.pbSelectNumber->setText(KeyboardConfiguration::getInstance(setContainer)->numberBasedSelectionTrigger());
		ui.pbWriteOutNumber->setVisible(KeyboardConfiguration::getInstance(setContainer)->enableNumberWriteOut());
		ui.pbWriteOutNumber->setText(KeyboardConfiguration::getInstance(setContainer)->numberWriteOutTrigger());
	} else
		ui.gbNumPad->hide();
}

const QString KeyboardCommandManager::preferredTrigger() const
{
	return i18n("Keyboard");
}

bool KeyboardCommandManager::trigger(const QString& triggerName)
{
	if (!triggerName.isEmpty())
		return false;

	activate();
	return true;
}


CommandConfiguration* KeyboardCommandManager::getConfigurationPage()
{
	disconnect(KeyboardConfiguration::getInstance(setContainer), SIGNAL(currentSetChanged()), this, SLOT(rebuildGui()));
	connect(KeyboardConfiguration::getInstance(setContainer), SIGNAL(currentSetChanged()), this, SLOT(rebuildGui()));
	return KeyboardConfiguration::getInstance(setContainer);
}


void KeyboardCommandManager::selectNumber()
{
	QStringList tabNames = keyboardSet->getAvailableTabs();
	if (keyboardSet->triggerButton(tabNames[ui.twTabs->currentIndex()], ui.leNumber->text(), 
			false /*case sensitivity does not matter with numbers so lets make it faster*/))
		ui.leNumber->clear();
}


void KeyboardCommandManager::writeOutNumber()
{
	EventHandler::getInstance()->sendWord(ui.leNumber->text());
	ui.leNumber->clear();
}

void KeyboardCommandManager::numberBackSpace()
{
	QString text = ui.leNumber->text();
	if (text.isEmpty()) return;

	text = text.left(text.count()-1);
	ui.leNumber->setText(text);
}

void KeyboardCommandManager::shift()
{
	kDebug() << "Shift";

}

void KeyboardCommandManager::capsLock()
{
	kDebug() << "CapsLock";

}

void KeyboardCommandManager::control()
{
	kDebug() << "Control";

}

void KeyboardCommandManager::backSpace()
{
	kDebug() << "BackSpace";

}


bool KeyboardCommandManager::load()
{

	if (numberIdentifiers.isEmpty())
		numberIdentifiers << i18n("Zero") << i18n("One") << i18n("Two") 
			<< i18n("Three") << i18n("Four") << i18n("Five") <<
			i18n("Six") << i18n("Seven") << i18n("Eight") << i18n("Nine");

	//Connect to Slots
	connect(KeyboardConfiguration::getInstance(setContainer), SIGNAL(currentSetChanged()), this, SLOT(rebuildGui()));
	connect(ui.pbOk, SIGNAL(clicked()), keyboardWidget, SLOT(hide()));
	connect(ui.pbOk, SIGNAL(clicked()), this, SLOT(deregister()));
	connect(ui.pb0, SIGNAL(clicked()), this, SLOT(send0()));
	connect(ui.pb1, SIGNAL(clicked()), this, SLOT(send1()));
	connect(ui.pb2, SIGNAL(clicked()), this, SLOT(send2()));
	connect(ui.pb3, SIGNAL(clicked()), this, SLOT(send3()));
	connect(ui.pb4, SIGNAL(clicked()), this, SLOT(send4()));
	connect(ui.pb5, SIGNAL(clicked()), this, SLOT(send5()));
	connect(ui.pb6, SIGNAL(clicked()), this, SLOT(send6()));
	connect(ui.pb7, SIGNAL(clicked()), this, SLOT(send7()));
	connect(ui.pb8, SIGNAL(clicked()), this, SLOT(send8()));
	connect(ui.pb9, SIGNAL(clicked()), this, SLOT(send9()));
	connect(ui.pbSelectNumber, SIGNAL(clicked()), this, SLOT(selectNumber()));
	connect(ui.pbWriteOutNumber, SIGNAL(clicked()), this, SLOT(writeOutNumber()));
	connect(ui.pbNumberBackspace, SIGNAL(clicked()), this, SLOT(numberBackSpace()));
	connect(ui.pbCapsLock, SIGNAL(clicked()), this, SLOT(capsLock()));
	connect(ui.pbShift, SIGNAL(clicked()), this, SLOT(shift()));
	connect(ui.pbControl, SIGNAL(clicked()), this, SLOT(control()));
	connect(ui.pbBackspace, SIGNAL(clicked()), this, SLOT(backSpace()));
	rebuildGui();
	return true;
}

bool KeyboardCommandManager::save()
{
	return true;
}

KeyboardCommandManager::~KeyboardCommandManager()
{
	kDebug() << "DELETING COMMAND MANAGER";
	keyboardWidget->deleteLater();
	activateAction->deleteLater();
	delete setContainer;
}
