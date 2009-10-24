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
	kDebug() << "deregister begin";
	getKeyboardConfiguration()->saveKeyboardGeometry(keyboardWidget->pos(),
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

	bool caseSensitivityBool = getKeyboardConfiguration()->caseSensitive();
	Qt::CaseSensitivity caseSensitivity = caseSensitivityBool ? Qt::CaseSensitive : Qt::CaseInsensitive;
        if (QString::compare(inputText, ui.pbOk->text().remove("&"), caseSensitivity)==0) {
		ui.pbOk->animateClick();
		return true;
	}

	kDebug() << "About to process special keys " << inputText;

	//special keys
	kDebug() << ui.pbShift->text().remove("&") << inputText << QString::compare(inputText, ui.pbShift->text().remove("&"), caseSensitivity);
        if (ui.pbShift->isVisible() && (QString::compare(inputText, ui.pbShift->text().remove("&"), caseSensitivity)==0)) {
		ui.pbShift->animateClick();
		return true;
	}
	kDebug()  << ui.pbCapsLock->isVisible() << inputText << ui.pbCapsLock->text().remove("&") << QString::compare(inputText, ui.pbCapsLock->text().remove("&"), caseSensitivity);
        if (ui.pbCapsLock->isVisible() && (QString::compare(inputText, ui.pbCapsLock->text().remove("&"), caseSensitivity)==0)) {
		ui.pbCapsLock->animateClick();
		return true;
	}
        if (ui.pbControl->isVisible() && (QString::compare(inputText, ui.pbControl->text().remove("&"), caseSensitivity)==0)) {
		ui.pbControl->animateClick();
		return true;
	}
        if (ui.pbBackspace->isVisible() && (QString::compare(inputText, ui.pbBackspace->text().remove("&"), caseSensitivity)==0)) {
		ui.pbBackspace->animateClick();
		return true;
	}
        if (ui.pbReturn->isVisible() && (QString::compare(inputText, ui.pbReturn->text().remove("&"), caseSensitivity)==0)) {
		ui.pbReturn->animateClick();
		return true;
	}
        if (ui.pbAlt->isVisible() && (QString::compare(inputText, ui.pbAlt->text().remove("&"), caseSensitivity)==0)) {
		ui.pbAlt->animateClick();
		return true;
	}
        if (ui.pbAltGr->isVisible() && (QString::compare(inputText, ui.pbAltGr->text().remove("&"), caseSensitivity)==0)) {
		ui.pbAltGr->animateClick();
		return true;
	}
        if (ui.pbSuper->isVisible() && (QString::compare(inputText, ui.pbSuper->text().remove("&"), caseSensitivity)==0)) {
		ui.pbSuper->animateClick();
		return true;
	}

	kDebug() << "About to process numpad " << inputText;

	//numpad?
	if (getKeyboardConfiguration()->showNumpad()) {
		if (ui.pbSelectNumber->isVisible() && (QString::compare(inputText, ui.pbSelectNumber->text().remove("&"), caseSensitivity)==0)) {
			ui.pbSelectNumber->animateClick();
			return true;
		}
		if (ui.pbWriteOutNumber->isVisible() && (QString::compare(inputText, ui.pbWriteOutNumber->text().remove("&"), caseSensitivity)==0)) {
			ui.pbWriteOutNumber->animateClick();
			return true;
		}
		if (QString::compare(inputText, ui.pbNumberBackspace->text().remove("&"), caseSensitivity)==0) {
			ui.pbNumberBackspace->animateClick();
			return true;
		}
		if (QString::compare(inputText, ui.pbDecimalSeparator->text().remove("&"), caseSensitivity)==0) {
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
	if (switchToTab(inputText, caseSensitivityBool))
		return true;

	QString currentTabName = getCurrentTabName();
	if (currentTabName.isNull())
		return false;

	return keyboardSet->triggerButton(currentTabName, inputText);
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
	if (!currentTab.isNull())
		switchToTab(currentTab, getKeyboardConfiguration()->caseSensitive());

	//special keys
	ui.pbControl->setVisible(getKeyboardConfiguration()->control());
	ui.pbControl->setText(getKeyboardConfiguration()->controlTrigger());
	ui.pbShift->setVisible(getKeyboardConfiguration()->shift());
	ui.pbShift->setText(getKeyboardConfiguration()->shiftTrigger());
	ui.pbCapsLock->setVisible(getKeyboardConfiguration()->capsLock());
	ui.pbCapsLock->setText(getKeyboardConfiguration()->capsLockTrigger());
	ui.pbBackspace->setVisible(getKeyboardConfiguration()->backspace());
	ui.pbBackspace->setText(getKeyboardConfiguration()->backspaceTrigger());
	ui.pbReturn->setVisible(getKeyboardConfiguration()->returnKey());
	ui.pbReturn->setText(getKeyboardConfiguration()->returnKeyTrigger());

	ui.pbAlt->setVisible(getKeyboardConfiguration()->alt());
	ui.pbAlt->setText(getKeyboardConfiguration()->altTrigger());
	ui.pbAltGr->setVisible(getKeyboardConfiguration()->altGr());
	ui.pbAltGr->setText(getKeyboardConfiguration()->altGrTrigger());
	ui.pbSuper->setVisible(getKeyboardConfiguration()->super());
	ui.pbSuper->setText(getKeyboardConfiguration()->superTrigger());

	//characters
	if (getKeyboardConfiguration()->showNumpad()) {
		kDebug() << "Showing numpad...";
		ui.gbNumPad->show();
		ui.pbNumberBackspace->setText(getKeyboardConfiguration()->numberBackspaceTrigger());
		ui.pbSelectNumber->setVisible(getKeyboardConfiguration()->enableNumberBasedSelection());
		ui.pbSelectNumber->setText(getKeyboardConfiguration()->numberBasedSelectionTrigger());
		ui.pbWriteOutNumber->setVisible(getKeyboardConfiguration()->enableNumberWriteOut());
		ui.pbWriteOutNumber->setText(getKeyboardConfiguration()->numberWriteOutTrigger());
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
	return KeyboardConfiguration::getInstance(this);
}

KeyboardConfiguration* KeyboardCommandManager::getKeyboardConfiguration()
{
	return static_cast<KeyboardConfiguration*>(getConfigurationPage());
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

}

void KeyboardCommandManager::capsLock(bool down)
{
	kDebug() << "CapsLock";
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
	kDebug() << "BackSpace";
	EventHandler::getInstance()->sendShortcut(QKeySequence("Backspace"));
}


bool KeyboardCommandManager::deSerializeConfig(const QDomElement& elem, Scenario *parent)
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
	rebuildGui();
	return true;
}

KeyboardCommandManager::~KeyboardCommandManager()
{
	keyboardWidget->deleteLater();
	activateAction->deleteLater();
	delete setContainer;
}
