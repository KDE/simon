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
#include <KLocalizedString>
#include <KAction>
#include <KActionCollection>

K_PLUGIN_FACTORY( KeyboardCommandPluginFactory, 
			registerPlugin< KeyboardCommandManager >(); 
		)

K_EXPORT_PLUGIN( KeyboardCommandPluginFactory("simonkeyboardcommand") )

QStringList KeyboardCommandManager::numberIdentifiers;

KeyboardCommandManager::KeyboardCommandManager(QObject* parent, const QVariantList& args) : CommandManager(parent, args),
	keyboardWidget(new QDialog(0, Qt::Dialog|Qt::WindowStaysOnTopHint)),
	activateAction(new KAction(this)),
	keyboardSet(NULL)
{
	setContainer = new KeyboardSetContainer();

	keyboardWidget->setWindowIcon(KIcon("input-keyboard"));
	connect(keyboardWidget, SIGNAL(finished(int)), this, SLOT(deregister()));
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
	int x,y;
	x=(tmp->width()/2) - (keyboardWidget->width()/2);
	y=((tmp->height()*3)/4)-(keyboardWidget->height()/2);
	keyboardWidget->move(x, y);
	keyboardWidget->show();
	startGreedy();
}

void KeyboardCommandManager::deregister()
{
	stopGreedy();
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
	keyboardWidget->reject();
}

void KeyboardCommandManager::processRequest(int number)
{
	kDebug() << "Digit: " << number;

	//ui.leNumber->setText(ui.leNumber->text()+QString::number(number));
}

void KeyboardCommandManager::ok()
{
	keyboardWidget->accept();
	//usleep(300000);
	//EventHandler::getInstance()->sendWord(ui.leNumber->text());
}

bool KeyboardCommandManager::greedyTrigger(const QString& inputText)
{
        if (inputText.toUpper() == ui.pbOk->text().toUpper())
	{
		ui.pbOk->animateClick();
		return true;
	}

        if (inputText.toUpper() == ui.pbShift->text().toUpper())
	{
		ui.pbShift->animateClick();
		return true;
	}

        if (inputText.toUpper() == ui.pbCapsLock->text().toUpper())
	{
		ui.pbCapsLock->animateClick();
		return true;
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

	return keyboardSet->triggerButton(tabNames[ui.twTabs->currentIndex()], inputText, 
			KeyboardConfiguration::getInstance(setContainer)->caseSensitive());

	//setting correct index
/*	bool ok=false;
	int index = inputText.toInt(&ok);
	if (!ok)
	{
		while ((index < numberIdentifiers.count()) && (numberIdentifiers.at(index).toUpper() != inputText.toUpper()))
			index++;

		if (index == numberIdentifiers.count()) return false;
	}

	return true;*/
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

	if (KeyboardConfiguration::getInstance(setContainer)->showNumpad()) {
		kDebug() << "Showing numpad...";

	}
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

bool KeyboardCommandManager::load()
{

	if (numberIdentifiers.isEmpty())
		numberIdentifiers << i18n("Zero") << i18n("One") << i18n("Two") 
			<< i18n("Three") << i18n("Four") << i18n("Five") <<
			i18n("Six") << i18n("Seven") << i18n("Eight") << i18n("Nine");

	//Connect to Slots
	connect(KeyboardConfiguration::getInstance(setContainer), SIGNAL(currentSetChanged()), this, SLOT(rebuildGui()));
	rebuildGui();
	return true;
}

bool KeyboardCommandManager::save()
{
	return true;
}

KeyboardCommandManager::~KeyboardCommandManager()
{
	keyboardWidget->deleteLater();
	activateAction->deleteLater();
	delete setContainer;
}
