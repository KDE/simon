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
#include "calculatorcommandmanager.h"
#include "calculatorconfiguration.h"
#include <eventsimulation/eventhandler.h>
#include <simonactions/actionmanager.h>
#include <simoninfo/simoninfo.h>
#include <unistd.h>
#include <QDesktopWidget>
#include <QDialog>
#include <KLocalizedString>
#include <KAction>
#include <stdlib.h>
#include <QList>
#include <simonactions/commandlistwidget.h>
#include <klocale.h>
#include <kglobal.h>

K_PLUGIN_FACTORY( CalculatorCommandPluginFactory, 
			registerPlugin< CalculatorCommandManager >(); 
		)
        
K_EXPORT_PLUGIN( CalculatorCommandPluginFactory("simoncalculatorcommand") )

QStringList CalculatorCommandManager::numberIdentifiers;

CalculatorCommandManager::CalculatorCommandManager(QObject *parent, const QVariantList& args) :CommandManager(parent, args)  
{
	KAction *activateAction = new KAction(this);
	activateAction->setText(i18n("Activate Calculator"));
	activateAction->setIcon(KIcon("accessories-calculator"));
	connect(activateAction, SIGNAL(triggered(bool)),
		this, SLOT(activate()));
	guiActions<<activateAction;


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
	connect(ui.pbClear, SIGNAL(clicked()), this, SLOT(clear()));
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
	connect(ui.pbMinus, SIGNAL(clicked()), this, SLOT(sendMinus()));
	connect(ui.pbMultiply, SIGNAL(clicked()), this, SLOT(sendMultiply()));
	connect(ui.pbDivide, SIGNAL(clicked()), this, SLOT(sendDivide()));
	connect(ui.pbEquals, SIGNAL(clicked()), this, SLOT(sendEquals()));
        connect(ui.pbPercent, SIGNAL(clicked()), this, SLOT(sendPercent()));

	commandListWidget = new CommandListWidget();
	commandListWidget->init(QStringList() << "go-next" << "go-back", QStringList() << "huhu" << "Yeah", 0); //Add Elements for the list
	connect(commandListWidget, SIGNAL(runRequest(int)), this, SLOT(writeoutRequestReceived(int)));
}

void CalculatorCommandManager::writeoutRequestReceived(int index)
{
	commandListWidget->hide();

	//Process Elements
	
}

void CalculatorCommandManager::deregister()
{
//	ActionManager::getInstance()->deRegisterPrompt(this, "executeSelection");
	stopGreedy();
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
	ui.leNumber->setText(ui.leNumber->text()+KGlobal::locale()->decimalSymbol());
}

void CalculatorCommandManager::sendPlus()
{
	ui.leNumber->setText(ui.leNumber->text()+"+");
}

void CalculatorCommandManager::sendMinus()
{
	ui.leNumber->setText(ui.leNumber->text()+"-");
}

void CalculatorCommandManager::sendMultiply()
{
	ui.leNumber->setText(ui.leNumber->text()+"*");
}

void CalculatorCommandManager::sendDivide()
{
	ui.leNumber->setText(ui.leNumber->text()+"/");
}

void CalculatorCommandManager::sendPercent()
{
        ui.leNumber->setText(ui.leNumber->text()+"%");
}

void CalculatorCommandManager::sendEquals()
{
	QList<Token*> *parsedInput = parseString(ui.leNumber->text());
	if(parsedInput!=NULL)
	{
	    QList<Token*> *postfixedInput =  toPostfix(parsedInput);

	    double output = calculate(postfixedInput);
	    //ui.leNumber->setText(QString("%1").arg(output,0,'f',4));
	    ui.leNumber->setText(QString::number(output));
	}
	else
	    ui.leNumber->clear();
}

QList<Token *> * CalculatorCommandManager::parseString(QString calc)
{
	QList<Token *> *list=new QList<Token *>();
	//status: Explains the status from the parser. 0=start, 1=number, 2=comma, 3=arithmetic operator, -1=fail
	int status=0;
	double number=0.0;
	bool isFloat=false;
	QString decimal = KGlobal::locale()->decimalSymbol();

	for(int i=0;i<calc.size();i++)
	{
	    if(calc.at(i)>=48 && calc.at(i)<=57)
	    {
		switch(status)
		{
		    case -1: ui.leNumber->clear();
			     SimonInfo::showMessage(i18n("Not a legal expression!"), 3000, new KIcon("accessories-calculator"));
			     break;
		    case 2: number=number+(calc.at(i).digitValue()/10.0f);
			    status=1;
			    break;
		    case 3: number=calc.at(i).digitValue();
			    status=1;
			    break;
		    default: number=number*10+calc.at(i).digitValue();
			     status=1;
			     break;
		}
		if((i+1)==calc.size())
		{
		    list->append(new Token(number));
		    return list;
		}
	    }
	    else if(status==1)
	    {
		if((i+1)!=calc.size())
		{

		    if(calc.at(i).toAscii() == decimal.at(0).toAscii())
		    {
			if(!isFloat)
			 {
			  status=2;
			  isFloat=true;
			 }
			else
			status=-1;
		    }

		    else
		    {
			switch(calc.at(i).toAscii())
			{
			
			    case '+': list->append(new Token(number));
				      list->append(new Token('+', 1));
				      isFloat=false;
				      status=3;
				      break;
			    case '-': list->append(new Token(number));
				      list->append(new Token('-',1));
				      isFloat=false;
				      status=3;
				      break;
			    case '*': list->append(new Token(number));
				      list->append(new Token('*',2));
				      isFloat=false;
				      status=3;
				      break;
			    case '/': list->append(new Token(number));
				      list->append(new Token('/',2));
				      isFloat=false;
				      status=3;
				      break;
                            case '%': list->append(new Token(list->at(list->size()-1)->getNumber()/100*number));
                                      break;
			}
		    }
		}
		else
		{
		    status=-1;
		    ui.leNumber->clear();
		    SimonInfo::showMessage(i18n("Not a legal expression!"), 3000, new KIcon("accessories-calculator"));
		}
	    }
	    else
	    {
		status=-1;
		ui.leNumber->clear();
		SimonInfo::showMessage(i18n("Not a legal expression!"), 3000, new KIcon("accessories-calculator"));
	    }
	}

	list->append(new Token(number));
	if(status==-1)
		return NULL;

	return list;
}

QList<Token *>* CalculatorCommandManager::toPostfix(QList<Token *> *calcList)
{
    QStack<Token *> *arOperatoren=new QStack<Token *>();
    QList<Token *> *list=new QList<Token *>();

    for(int i=0;i<calcList->size();i++)
    {
	if(calcList->at(i)->getType()==0)
	{
	    list->append((*calcList)[i]);
	}

	else if(calcList->at(i)->getType()==1)
	{
	    while(!arOperatoren->isEmpty())
	    {
		list->append(arOperatoren->pop());
	    }
	    arOperatoren->push(calcList->at(i));
	}

	else if(calcList->at(i)->getType()==2)
	{
	    if(!arOperatoren->isEmpty() && arOperatoren->top()->getType()==2) //if there are more then 2 types, exchange the if with a while-loop
	    {
		list->append(arOperatoren->pop());
	    }
	    arOperatoren->push(calcList->at(i));
	}

	else
	{
	    ui.leNumber->setText(i18n("Error in function: toPostfix()"));
	}
    }

    while(!arOperatoren->isEmpty())
    {
	list->append(arOperatoren->pop());
    }

    delete arOperatoren;
    delete calcList;
    return list;
}

double CalculatorCommandManager::calculate(QList<Token *>* postList)
{
    int i;
    QStack<Token *> calc;
    Token *t;

    for(i=0;i<postList->size();i++)
    {
	t=postList->at(i);

	if(t->getType()==0)
	{
	    calc.push(t);
	}
	else
	{
	    switch(t->getArOperator())
	    {
		case '+':  {
			double op1, op2;
			op1 = calc.pop()->getNumber();
			op2 = calc.pop()->getNumber();
			kDebug() << " Adding: " << op1 << op2 << " = " << (op1 + op2);
			calc.push(new Token(op1+op2));
			 break;
			}
		case '-':  {
			double op1, op2;
			op2 = calc.pop()->getNumber();
			op1 = calc.pop()->getNumber();
			calc.push(new Token(op1-op2));
			break;
			}
		case '*': 
			 calc.push(new Token(calc.pop()->getNumber()*calc.pop()->getNumber()));
			 break;
		case '/':  {
			double op1, op2;
			op2 = calc.pop()->getNumber();
			op1 = calc.pop()->getNumber();
			if (op2 == 0)
			{
			    SimonInfo::showMessage(i18n("Can't divide through 0"), 3000, new KIcon("accessories-calculator"));
			    calc.push(new Token(0));
			} 
			else 
			    calc.push(new Token(op1/op2));
			break;
			}
	    }
	}

    }

    delete postList;
    return calc.pop()->getNumber();
}

void CalculatorCommandManager::back()
{
	QString text = ui.leNumber->text();
	if (text.count() == 0) return;

	text = text.left(text.count()-1);
	ui.leNumber->setText(text);
}

void CalculatorCommandManager::clear()
{
	ui.leNumber->clear();
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
	commandListWidget->show();
	//widget->accept();
	//usleep(300000);
	//EventHandler::getInstance()->sendWord(ui.leNumber->text());
}

bool CalculatorCommandManager::greedyTrigger(const QString& inputText)
{
	//setting correct index
	bool ok=false;
	int index = inputText.toInt(&ok);
	if (!ok){
		while ((index < numberIdentifiers.count()) && (numberIdentifiers.at(index).toUpper() != inputText.toUpper()))
			index++;
	}

	if (index < numberIdentifiers.count()) {
		if (commandListWidget->isVisible()) {
			writeoutRequestReceived(index);
			return true;
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
	if(inputText.toUpper() == i18n("Minus").toUpper())
	{
		ui.pbMinus->animateClick();
		return true;
	}
	if(inputText.toUpper() == i18n("Multiply").toUpper())
	{
		ui.pbMultiply->animateClick();
		return true;
	}
	if(inputText.toUpper() == i18n("Divide").toUpper())
	{
		ui.pbDivide->animateClick();
		return true;
	}
	if(inputText.toUpper() == i18n("Equals").toUpper())
	{
		ui.pbEquals->animateClick();
		return true;
	}
        if(inputText.toUpper() == i18n("Percent").toUpper())
        {
                ui.pbPercent->animateClick();
                return true;
        }

	return true;
}


bool CalculatorCommandManager::trigger(const QString& triggerName)
{
	if (triggerName != CalculatorConfiguration::getInstance()->trigger()){
		kDebug() << triggerName << "Returning";
		return false;
	}
	
	activate();
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

void CalculatorCommandManager::activate()
{
	ui.leNumber->clear();
	QDesktopWidget* tmp = QApplication::desktop();
	int x,y;
	x=(tmp->width()/2)-(widget->width()/2);
	y=(tmp->height()/2)-(widget->height()/2);
	widget->move(x, y);
	widget->show();
	startGreedy();
    

CalculatorCommandManager::~CalculatorCommandManager()
{
	widget->deleteLater();
}
