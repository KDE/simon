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

KeyboardCommandManager::KeyboardCommandManager(QObject *parent, const QVariantList& args) :CommandManager(parent, args)  
{
	widget = new QDialog(0, Qt::Dialog|Qt::WindowStaysOnTopHint);
	widget->setWindowIcon(KIcon("accessories-calculator"));
	connect(widget, SIGNAL(rejected()), this, SLOT(deregister()));
        ui.setupUi(widget);
	widget->hide();

	setXMLFile("simonkeyboardpluginui.rc");
	activateAction = new KAction(this);
	activateAction->setText(i18n("Activate Number Input"));
	activateAction->setIcon(KIcon("accessories-calculator"));
	connect(activateAction, SIGNAL(triggered(bool)),
		this, SLOT(activate()));
	actionCollection()->addAction("simonkeyboardplugin", activateAction);

	if (numberIdentifiers.isEmpty())
		numberIdentifiers << i18n("Zero") << i18n("One") << i18n("Two") 
			<< i18n("Three") << i18n("Four") << i18n("Five") <<
			i18n("Six") << i18n("Seven") << i18n("Eight") << i18n("Nine");

        //Connect to Slots
}

void KeyboardCommandManager::activate()
{
	ui.leNumber->clear();
	QDesktopWidget* tmp = QApplication::desktop();
	int x,y;
	x=(tmp->width()/2) - (widget->width()/2);
	y=(tmp->height()/2)-(widget->height()/2);
	widget->move(x, y);
	widget->show();
	startGreedy();
}

void KeyboardCommandManager::deregister()
{
	stopGreedy();
}

const KIcon KeyboardCommandManager::icon() const
{
	return KIcon("accessories-calculator");
}


const QString KeyboardCommandManager::name() const
{
	return i18n("Input Number");
}


void KeyboardCommandManager::sendComma()
{
	ui.leNumber->setText(ui.leNumber->text()+i18nc("Decimal point", "."));
}

void KeyboardCommandManager::back()
{
	QString text = ui.leNumber->text();
	if (text.count() == 0) return;

	text = text.left(text.count()-1);
	ui.leNumber->setText(text);
}


void KeyboardCommandManager::cancel()
{
	widget->reject();
}

void KeyboardCommandManager::processRequest(int number)
{
	kDebug() << "Digit: " << number;

	ui.leNumber->setText(ui.leNumber->text()+QString::number(number));
}

void KeyboardCommandManager::ok()
{
	widget->accept();
	usleep(300000);
	EventHandler::getInstance()->sendWord(ui.leNumber->text());
}

bool KeyboardCommandManager::greedyTrigger(const QString& inputText)
{
        if (inputText.toUpper() == i18n("Ok").toUpper())
	{
		ui.pbOk->animateClick();
		return true;
	}
	if (inputText.toUpper() == i18nc("Name of the decimal seperator", "Comma").toUpper())
	{
		ui.pbComma->animateClick();
		return true;
	}


	//setting correct index
	bool ok=false;
	int index = inputText.toInt(&ok);
	if (!ok)
	{
		while ((index < numberIdentifiers.count()) && (numberIdentifiers.at(index).toUpper() != inputText.toUpper()))
			index++;

		if (index == numberIdentifiers.count()) return false;
	}

//	switch (index)
//	{
//		case 0:
//			ui.pb0->animateClick();
//			break;
//		case 1:
//			ui.pb1->animateClick();
//			break;
//		case 2:
//			ui.pb2->animateClick();
//			break;
//		case 3:
//			ui.pb3->animateClick();
//			break;
//		case 4:
//			ui.pb4->animateClick();
//			break;
//		case 5:
//			ui.pb5->animateClick();
//			break;
//		case 6:
//			ui.pb6->animateClick();
//			break;
//		case 7:
//			ui.pb7->animateClick();
//			break;
//		case 8:
//			ui.pb8->animateClick();
//			break;
//			ui.pb9->animateClick();
//			break;
//	}
	return true;
}



bool KeyboardCommandManager::trigger(const QString& triggerName)
{
	if (triggerName != KeyboardConfiguration::getInstance()->trigger()){
		return false;
	}

	activate();
	return true;
}


CommandConfiguration* KeyboardCommandManager::getConfigurationPage()
{
	return KeyboardConfiguration::getInstance();
}

bool KeyboardCommandManager::load()
{
	return true;
}

bool KeyboardCommandManager::save()
{
	return true;
}

KeyboardCommandManager::~KeyboardCommandManager()
{
	widget->deleteLater();
	activateAction->deleteLater();
}
