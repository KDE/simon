/*
 *   Copyright (C) 2008 Peter Grasch <grasch@simon-calculatorens.org>
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
#include "calculatorcommandmanager.h"
#include "calculatorconfiguration.h"
#include <eventsimulation/eventhandler.h>
#include <simonactions/actionmanager.h>
#include <unistd.h>
#include <QDesktopWidget>
#include <QDialog>
#include <KLocalizedString>

K_PLUGIN_FACTORY( CalculatorCommandPluginFactory, 
			registerPlugin< CalculatorCommandManager >(); 
		)
        
K_EXPORT_PLUGIN( CalculatorCommandPluginFactory("simoncalculatorcommand") )

QStringList CalculatorCommandManager::numberIdentifiers;

CalculatorCommandManager::CalculatorCommandManager(QObject *parent, const QVariantList& args) :CommandManager(parent, args)  
{
	widget = new QDialog(0, Qt::Dialog|Qt::WindowStaysOnTopHint);
	widget->setWindowIcon(KIcon("accessories-calculator"));
	connect(widget, SIGNAL(rejected()), this, SLOT(deregister()));
	ui.setupUi(widget);
	ui.pbOk->setIcon(KIcon("dialog-ok-apply"));
	ui.pbCancel->setIcon(KIcon("dialog-cancel"));
	widget->hide();

	if (numberIdentifiers.isEmpty())
		numberIdentifiers << i18n("Zero") << i18n("One") << i18n("Two") 
			<< i18n("Three") << i18n("Four") << i18n("Five") <<
			i18n("Six") << i18n("Seven") << i18n("Eight") << i18n("Nine");

	connect(widget, SIGNAL(finished(int)), this, SLOT(deregister()));
	connect(ui.pbCancel, SIGNAL(clicked()), this, SLOT(cancel()));
	connect(ui.pbBack, SIGNAL(clicked()), this, SLOT(back()));
	connect(ui.pbOk, SIGNAL(clicked()), this, SLOT(ok()));
	connect(ui.pbComma, SIGNAL(clicked()), this, SLOT(sendComma()));
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
	connect(ui.pbPlus, SIGNAL(clicked()), this, SLOT(sendPlus()));
}

void CalculatorCommandManager::deregister()
{
	ActionManager::getInstance()->deRegisterPrompt(this, "executeSelection");
}

const KIcon CalculatorCommandManager::icon() const
{
	return KIcon("accessories-calculator");
}


const QString CalculatorCommandManager::name() const
{
	return i18n("Calculator");
}


void CalculatorCommandManager::sendComma()
{
	ui.leNumber->setText(ui.leNumber->text()+i18nc("Decimal point", "."));
}

void CalculatorCommandManager::sendPlus()
{
	ui.leNumber->setText(ui.leNumber->text()+"+");
}

void CalculatorCommandManager::back()
{
	QString text = ui.leNumber->text();
	if (text.count() == 0) return;

	text = text.left(text.count()-1);
	ui.leNumber->setText(text);
}


void CalculatorCommandManager::cancel()
{
	widget->reject();
}

void CalculatorCommandManager::processRequest(int number)
{
	kDebug() << "Digit: " << number;

	ui.leNumber->setText(ui.leNumber->text()+QString::number(number));
}

void CalculatorCommandManager::ok()
{
	widget->accept();
	usleep(300000);
	EventHandler::getInstance()->sendWord(ui.leNumber->text());
}

bool CalculatorCommandManager::executeSelection(QString inputText)
{
	if (inputText.toUpper() == i18n("Cancel").toUpper())
	{
		ui.pbCancel->animateClick();
		return true;
	}
	if (inputText.toUpper() == i18n("Back").toUpper())
	{
		ui.pbBack->animateClick();
		return true;
	}
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
	if(inputText.toUpper() == i18n("Plus").toUpper())
	{
		ui.pbPlus->animateClick();
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

	switch (index)
	{
		case 0:
			ui.pb0->animateClick();
			break;
		case 1:
			ui.pb1->animateClick();
			break;
		case 2:
			ui.pb2->animateClick();
			break;
		case 3:
			ui.pb3->animateClick();
			break;
		case 4:
			ui.pb4->animateClick();
			break;
		case 5:
			ui.pb5->animateClick();
			break;
		case 6:
			ui.pb6->animateClick();
			break;
		case 7:
			ui.pb7->animateClick();
			break;
		case 8:
			ui.pb8->animateClick();
			break;
		case 9:
			ui.pb9->animateClick();
			break;
	}
	return true;
}


#include <KMessageBox>
bool CalculatorCommandManager::trigger(const QString& triggerName)
{
	if (triggerName != CalculatorConfiguration::getInstance()->trigger()){
		kDebug() << triggerName << "Returning";
		return false;
	}
	
	ui.leNumber->clear();
	QDesktopWidget* tmp = QApplication::desktop();
	int x,y;
	x=(tmp->width()/2) - (widget->width()/2);
	y=(tmp->height()/2)-(widget->height()/2);
	widget->move(x, y);
	widget->show();
	ActionManager::getInstance()->registerPrompt(this, "executeSelection");

	return true;
}


CommandConfiguration* CalculatorCommandManager::getConfigurationPage()
{
	return CalculatorConfiguration::getInstance();
}

bool CalculatorCommandManager::load()
{
	return true;
}

bool CalculatorCommandManager::save()
{
	return true;
}

CalculatorCommandManager::~CalculatorCommandManager()
{
	widget->deleteLater();
}
