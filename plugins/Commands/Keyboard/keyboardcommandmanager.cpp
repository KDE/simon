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

K_PLUGIN_FACTORY( KeyboardCommandPluginFactory, 
			registerPlugin< KeyboardCommandManager >(); 
		)

K_EXPORT_PLUGIN( KeyboardCommandPluginFactory("simonkeyboardcommand") )

QStringList KeyboardCommandManager::numberIdentifiers;

KeyboardCommandManager::KeyboardCommandManager(QObject* parent, const QVariantList& args) : CommandManager((Scenario*) parent, args),
	GreedyReceiver(this),
	keyboardWidget(new QWidget(0, Qt::Dialog|Qt::WindowStaysOnTopHint)),
	activateAction(new KAction(this)),
	keyboardSet(NULL)
{
	setContainer = new KeyboardSetContainer();

	setFont(ActionManager::getInstance()->pluginBaseFont());

	keyboardWidget->setWindowIcon(KIcon("input-keyboard"));
        ui.setupUi(keyboardWidget);
	keyboardWidget->hide();

	activateAction->setText(i18n("Activate Keyboard"));
	activateAction->setIcon(KIcon("input-keyboard"));
	connect(activateAction, SIGNAL(triggered(bool)),
		this, SLOT(activate()));
	guiActions<<activateAction;
}

void KeyboardCommandManager::setFont(const QFont& font)
{
	keyboardWidget->setFont(font);
}

void KeyboardCommandManager::untoggleShift()
{
	ui.pbShift->setChecked(false); //untoggle shift button
}

void KeyboardCommandManager::activate()
{
	QDesktopWidget* tmp = QApplication::desktop();

	QPoint pos =getKeyboardConfiguration()->keyboardPosition();
	QSize size = getKeyboardConfiguration()->keyboardSize();

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
	getKeyboardConfiguration()->saveKeyboardGeometry(keyboardWidget->pos(),
							  keyboardWidget->size());
	stopGreedy();
}

const QString KeyboardCommandManager::iconSrc() const
{
	return "input-keyboard";
}


const QString KeyboardCommandManager::name() const
{
	return i18n("Keyboard");
}


void KeyboardCommandManager::processRequest(int number)
{
	ui.leNumber->setText(ui.leNumber->text()+QString::number(number));
}

bool KeyboardCommandManager::greedyTrigger(const QString& inputText)
{
	if (trigger(inputText))
		return true;


	bool caseSensitivityBool = getKeyboardConfiguration()->caseSensitive();

	//no widgets?
	if (switchToTab(inputText, caseSensitivityBool))
		return true;

	QString currentTabName = getCurrentTabName();
	if (currentTabName.isNull())
		return false;

	bool buttonFound = keyboardSet->triggerButton(currentTabName, inputText, caseSensitivityBool);
	if (buttonFound)
		untoggleShift();
	return buttonFound;
}

bool KeyboardCommandManager::switchToTab(const QString& tabName, bool caseSensitivity)
{
	if (ui.twTabs->currentIndex() == -1) return false;

	Qt::CaseSensitivity caseS = (caseSensitivity ? Qt::CaseSensitive : Qt::CaseInsensitive);

	QStringList tabNames = keyboardSet->getAvailableTabs();
	for (int i=0; i < tabNames.count(); i++) {
		if (QString::compare(tabNames[i], tabName, caseS) == 0) {
			ui.twTabs->setCurrentIndex(i);
			return true;
		}
	}
	return false;
}

QString KeyboardCommandManager::getCurrentTabName()
{
	int currentIndex = ui.twTabs->currentIndex();
	if (currentIndex == -1) return QString();

	QStringList tabNames = keyboardSet->getAvailableTabs();
	return tabNames[currentIndex];
}

void KeyboardCommandManager::rebuildGui()
{
	keyboardSet = getKeyboardConfiguration()->getStoredKeyboardSet();
	if (!keyboardSet) return;

	keyboardWidget->setWindowTitle(keyboardSet->getSetName());
	
	QString currentTab = getCurrentTabName();
	// clear tab
	while(ui.twTabs->count() > 0) {
		QWidget *w = ui.twTabs->widget(0);
		ui.twTabs->removeTab(0);

		if (!w)
			continue;

		QLayout *lay = w->layout();
		//remove all the buttons so they won't be deleted when deleting the widget
		if (lay) {
			while (lay->count() > 0) {
				QLayoutItem *item = lay->takeAt(0);
				QWidget *itemWidget = item->widget();
				if (itemWidget)  itemWidget->setParent(0);
				delete item; //leaves the widget intact)
			}
			lay->deleteLater();
		}
		w->deleteLater();
	}

	QStringList tabs = keyboardSet->getAvailableTabs();
	foreach (const QString& tabName, tabs) {
		QWidget *w = new QWidget(ui.twTabs);
		FlowLayout *flow = new FlowLayout(w);

		QList<KeyboardButton*> buttons = keyboardSet->getTabButtons(tabName);
		foreach (KeyboardButton* button, buttons) {
			flow->addWidget(button);
			button->show();
		}
		w->setLayout(flow);

		ui.twTabs->addTab(w, tabName);
	}
	if (!currentTab.isNull())
		switchToTab(currentTab, getKeyboardConfiguration()->caseSensitive());

	//characters
	ui.gbNumPad->setVisible(getKeyboardConfiguration()->showNumpad());
}

const QString KeyboardCommandManager::preferredTrigger() const
{
	return "";
}


KeyboardConfiguration* KeyboardCommandManager::getKeyboardConfiguration()
{
	return static_cast<KeyboardConfiguration*>(getConfigurationPage());
}

void KeyboardCommandManager::selectNumber()
{
	if (ui.leNumber->text().isEmpty()) return;

	if (greedyTrigger(ui.leNumber->text()))
		ui.leNumber->clear();
}

void KeyboardCommandManager::writeOutNumber()
{
	if (ui.leNumber->text().isEmpty()) return;

	EventHandler::getInstance()->sendWord(ui.leNumber->text());
	ui.leNumber->clear();
	untoggleShift();
}

void KeyboardCommandManager::numberBackSpace()
{
	QString text = ui.leNumber->text();
	if (text.isEmpty()) return;

	text = text.left(text.count()-1);
	ui.leNumber->setText(text);
}

void KeyboardCommandManager::shift(bool down)
{
	kDebug() << "Shift";
	if (down)
		EventHandler::getInstance()->setModifier((int) Qt::SHIFT, true /*once*/);
	else 
		EventHandler::getInstance()->unsetModifier((int) Qt::SHIFT);

}
void KeyboardCommandManager::returnPressed()
{
	kDebug() << "Return";
	EventHandler::getInstance()->sendWord("\n");
	untoggleShift();
}

void KeyboardCommandManager::capsLock(bool down)
{
	kDebug() << "CapsLock";
	untoggleShift();
	if (down)
		EventHandler::getInstance()->setModifier((int) Qt::SHIFT);
	else
		EventHandler::getInstance()->unsetModifier((int) Qt::SHIFT);

}

void KeyboardCommandManager::control(bool down)
{
	if (down)
		EventHandler::getInstance()->setModifier((int) Qt::CTRL);
	else 
		EventHandler::getInstance()->unsetModifier((int) Qt::CTRL);
}

void KeyboardCommandManager::alt(bool down)
{
	if (down)
		EventHandler::getInstance()->setModifier((int) Qt::ALT);
	else 
		EventHandler::getInstance()->unsetModifier((int) Qt::ALT);
}

void KeyboardCommandManager::altGr(bool down)
{
	if (down)
		EventHandler::getInstance()->setModifier((int) Qt::Key_AltGr);
	else 
		EventHandler::getInstance()->unsetModifier((int) Qt::Key_AltGr);
}

void KeyboardCommandManager::super(bool down)
{
	if (down)
		EventHandler::getInstance()->setModifier((int) Qt::META);
	else 
		EventHandler::getInstance()->unsetModifier((int) Qt::META);
}

void KeyboardCommandManager::backSpace()
{
	EventHandler::getInstance()->sendShortcut(QKeySequence("Backspace"));
}


bool KeyboardCommandManager::deSerializeConfig(const QDomElement& elem)
{
	if (numberIdentifiers.isEmpty())
		numberIdentifiers << i18n("Zero") << i18n("One") << i18n("Two") 
			<< i18n("Three") << i18n("Four") << i18n("Five") <<
			i18n("Six") << i18n("Seven") << i18n("Eight") << i18n("Nine");

	//Connect to Slots
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
	connect(ui.pbCapsLock, SIGNAL(toggled(bool)), this, SLOT(capsLock(bool)));
	connect(ui.pbShift, SIGNAL(toggled(bool)), this, SLOT(shift(bool)));
	connect(ui.pbControl, SIGNAL(toggled(bool)), this, SLOT(control(bool)));
	connect(ui.pbBackspace, SIGNAL(clicked()), this, SLOT(backSpace()));
	connect(ui.pbReturn, SIGNAL(clicked()), this, SLOT(returnPressed()));

	connect(ui.pbAlt, SIGNAL(toggled(bool)), this, SLOT(alt(bool)));
	connect(ui.pbAltGr, SIGNAL(toggled(bool)), this, SLOT(altGr(bool)));
	connect(ui.pbSuper, SIGNAL(toggled(bool)), this, SLOT(super(bool)));

	if (!config) config->deleteLater();
	config = new KeyboardConfiguration(this, parentScenario);
	config->deSerialize(elem);



	bool succ = true;
	succ &= installInterfaceCommand(this, "activate", i18n("Keyboard"), iconSrc(),
			i18n("Starts the keyboard"), true /* announce */, true /* show icon */,
			SimonCommand::DefaultState /* consider this command when in this state */, 
			SimonCommand::GreedyState, /* if executed switch to this state */
			QString() /* take default visible id from action name */,
			"startKeyboard" /* id */);
	
	
	succ &= installInterfaceCommand(ui.pbOk, "click", i18n("Ok"), "dialog-ok",
			i18n("Hides the keyboard"), false, true, SimonCommand::GreedyState,
			SimonCommand::DefaultState);
	succ &= installInterfaceCommand(ui.pbShift, "click", i18n("Shift"), iconSrc(),
			i18n("Shift key"), false, false, SimonCommand::GreedyState,
			SimonCommand::GreedyState);
	succ &= installInterfaceCommand(ui.pbCapsLock, "click", i18n("Caps lock"), iconSrc(),
			i18n("Caps lock key"), false, false, SimonCommand::GreedyState,
			SimonCommand::GreedyState);
	succ &= installInterfaceCommand(ui.pbControl, "click", i18n("Control"), iconSrc(),
			i18n("Control"), false, false, SimonCommand::GreedyState,
			SimonCommand::GreedyState);
	succ &= installInterfaceCommand(ui.pbBackspace, "click", i18n("Backspace"), iconSrc(),
			i18n("Backspace"), false, false, SimonCommand::GreedyState,
			SimonCommand::GreedyState);
	succ &= installInterfaceCommand(ui.pbReturn, "click", i18n("Return"), iconSrc(),
			i18n("Return"), false, false, SimonCommand::GreedyState,
			SimonCommand::GreedyState);
	succ &= installInterfaceCommand(ui.pbAlt, "click", i18n("Alt"), iconSrc(),
			i18n("Alt"), false, false, SimonCommand::GreedyState,
			SimonCommand::GreedyState);
	succ &= installInterfaceCommand(ui.pbAltGr, "click", i18n("AltGr"), iconSrc(),
			i18n("AltGr"), false, false, SimonCommand::GreedyState,
			SimonCommand::GreedyState);
	succ &= installInterfaceCommand(ui.pbSuper, "click", i18n("Super"), iconSrc(),
			i18n("Super"), false, false, SimonCommand::GreedyState,
			SimonCommand::GreedyState);


	//input number
	succ &= installInterfaceCommand(ui.pb0, "click", i18n("Zero"), iconSrc(),
			i18n("Clicks 0"), false, false, SimonCommand::GreedyState, 
			SimonCommand::GreedyState, "0");
	succ &= installInterfaceCommand(ui.pb1, "click", i18n("One"), iconSrc(),
			i18n("Clicks 1"), false, false, SimonCommand::GreedyState, 
			SimonCommand::GreedyState, "1");
	succ &= installInterfaceCommand(ui.pb2, "click", i18n("Two"), iconSrc(),
			i18n("Clicks 2"), false, false, SimonCommand::GreedyState, 
			SimonCommand::GreedyState, "2");
	succ &= installInterfaceCommand(ui.pb3, "click", i18n("Three"), iconSrc(),
			i18n("Clicks 3"), false, false, SimonCommand::GreedyState, 
			SimonCommand::GreedyState, "3");
	succ &= installInterfaceCommand(ui.pb4, "click", i18n("Four"), iconSrc(),
			i18n("Clicks 4"), false, false, SimonCommand::GreedyState, 
			SimonCommand::GreedyState, "4");
	succ &= installInterfaceCommand(ui.pb5, "click", i18n("Five"), iconSrc(),
			i18n("Clicks 5"), false, false, SimonCommand::GreedyState, 
			SimonCommand::GreedyState, "5");
	succ &= installInterfaceCommand(ui.pb6, "click", i18n("Six"), iconSrc(),
			i18n("Clicks 6"), false, false, SimonCommand::GreedyState, 
			SimonCommand::GreedyState, "6");
	succ &= installInterfaceCommand(ui.pb7, "click", i18n("Seven"), iconSrc(),
			i18n("Clicks 7"), false, false, SimonCommand::GreedyState, 
			SimonCommand::GreedyState, "7");
	succ &= installInterfaceCommand(ui.pb8, "click", i18n("Eight"), iconSrc(),
			i18n("Clicks 8"), false, false, SimonCommand::GreedyState, 
			SimonCommand::GreedyState, "8");
	succ &= installInterfaceCommand(ui.pb9, "click", i18n("Nine"), iconSrc(),
			i18n("Clicks 9"), false, false, SimonCommand::GreedyState, 
			SimonCommand::GreedyState, "9");
	succ &= installInterfaceCommand(ui.pbDecimalSeparator, "click", i18nc("Decimal separator (voice trigger)", "Point"), iconSrc(),
			i18n("Decimal separator"), false, false, SimonCommand::GreedyState, 
			SimonCommand::GreedyState, KGlobal::locale()->decimalSymbol());

	succ &= installInterfaceCommand(ui.pbSelectNumber, "click", i18n("Select number"), iconSrc(),
			i18n("Select number"), false, false, SimonCommand::GreedyState, 
			SimonCommand::GreedyState);
	succ &= installInterfaceCommand(ui.pbWriteOutNumber, "click", i18n("Write number"), iconSrc(),
			i18n("Write number"), false, false, SimonCommand::GreedyState, 
			SimonCommand::GreedyState);
	succ &= installInterfaceCommand(ui.pbNumberBackspace, "click", i18n("Number backspace"), iconSrc(),
			i18n("Number backspace"), false, false, SimonCommand::GreedyState, 
			SimonCommand::GreedyState);

	return true;
}


KeyboardCommandManager::~KeyboardCommandManager()
{
	keyboardWidget->deleteLater();
	activateAction->deleteLater();
	delete setContainer;
}

