/*
 *   Copyright (C) 2009 Peter Grasch <grasch@simon-listens.org>
 *   Copyright (C) 2009 Dominik Neumeister <neudob06@edvhtl.at>
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
#include <KDebug>

K_PLUGIN_FACTORY( CalculatorCommandPluginFactory, 
			registerPlugin< CalculatorCommandManager >(); 
		)
        
K_EXPORT_PLUGIN( CalculatorCommandPluginFactory("simoncalculatorcommand") )

QStringList CalculatorCommandManager::numberIdentifiers;

CalculatorCommandManager::CalculatorCommandManager(QObject* parent, const QVariantList& args) : CommandManager(parent, args),
	widget(new QDialog(0, Qt::Dialog|Qt::WindowStaysOnTopHint)),
	commandListWidget(new CommandListWidget()),
	currentResult(0),
	resultCurrentlyDisplayed(true)
{
	KAction *activateAction = new KAction(this);
	activateAction->setText(i18n("Activate Calculator"));
	activateAction->setIcon(KIcon("accessories-calculator"));
	connect(activateAction, SIGNAL(triggered(bool)),
		this, SLOT(activate()));
	guiActions<<activateAction;

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
//	connect(ui.pbBracketOpen, SIGNAL(clicked()), this, SLOT(sendBracketOpen()));
//	connect(ui.pbBracketClose, SIGNAL(clicked()), this, SLOT(sendBracketClose()));
	connect(ui.pbEquals, SIGNAL(clicked()), this, SLOT(sendEquals()));
        connect(ui.pbPercent, SIGNAL(clicked()), this, SLOT(sendPercent()));

	//TODO: implement brackets
	ui.pbBracketClose->hide();
	ui.pbBracketOpen->hide();
//	ui.pbPercent->hide();

	commandListWidget->init(QStringList() << "go-next" << "go-next" << "go-next" << "go-next" <<
			"go-next" << "go-next", 
			QStringList() << i18n("Result") << 
			i18n("Calculation & result") << 
			i18n("Formatted result") <<
			i18n("Formatted calculation and result") <<
			i18n("Format result as money") << 
			i18n("Format calculation and result as money"), 0); //Add Elements for the list
	connect(commandListWidget, SIGNAL(runRequest(int)), this, SLOT(writeoutRequestReceived(int)));
}

const QString CalculatorCommandManager::preferredTrigger() const
{
	return i18n("Calculator");
}

void CalculatorCommandManager::writeoutRequestReceived(int index)
{
	kDebug() << "write request received";
	commandListWidget->hide();
	commandListWidget->abortTimeoutSelection();
	
	QString output;

	switch (index) {
		case CalculatorConfiguration::Result:
			if (resultCurrentlyDisplayed)
				output = toString(currentResult);
			break;
		case CalculatorConfiguration::CalculationAndResult:
			output = ui.leNumber->text();
			break;
		case CalculatorConfiguration::FormattedResult:
			output = formatOutput(CalculatorCommandManager::Default);
			break;
		case CalculatorConfiguration::FormattedCalculationAndResult:
			output = formatCalculation(CalculatorCommandManager::Default);
			break;
		case CalculatorConfiguration::FormattedMoneyResult:
			output = formatOutput(CalculatorCommandManager::Money);
			break;
		case CalculatorConfiguration::FormattedMoneyCalculationAndResult:
			output = formatCalculation(CalculatorCommandManager::Money);
			break;
	}

	widget->accept();
	usleep(300000);
	EventHandler::getInstance()->sendWord(output);
}


QString CalculatorCommandManager::formatCalculation(CalculatorCommandManager::NumberType type)
{
	QString output = formatInput(type);

	if (resultCurrentlyDisplayed)
		output += "="+formatOutput(type);

	return output;
}


QString CalculatorCommandManager::formatInput(CalculatorCommandManager::NumberType type)
{
	QString input = ui.leNumber->text();
	if (input.contains("="))
		input = input.left(input.indexOf("="));
	QList<Token*> *parsedString = parseString(input);
	if (!parsedString) {
		SimonInfo::showMessage(i18n("Error in output"), 3000);
		return QString();
	}

	QString output;

	for (int i=0; i < parsedString->count(); i++) {
		Token *t = parsedString->at(i);

		//format number / operator
		switch (t->getType()) {
			case -1: output += ",";
				break;
			case 0:
				 
				switch (type) {
					case CalculatorCommandManager::Default:
						 output += KGlobal::locale()->formatNumber(t->getNumber());
						 break;
					case CalculatorCommandManager::Money:
						 if ((i+1 < parsedString->count()) && (parsedString->at(i+1)->getType() == 3))
							 //percentage coming up so don't format it as money
							 output += KGlobal::locale()->formatNumber(t->getNumber());
						 else 
							 output += KGlobal::locale()->formatMoney(t->getNumber());
						 break;
				}
				break;
			case 1:
			case 2:
			case 3:
				output += t->getArOperator();
				break;
		}
	}
	return output;

}

QString CalculatorCommandManager::formatOutput(CalculatorCommandManager::NumberType type)
{
	if (!resultCurrentlyDisplayed) return QString();

	QString output;

	switch (type) {
		case CalculatorCommandManager::Default:
			output = KGlobal::locale()->formatNumber(currentResult);
			break;
		case CalculatorCommandManager::Money:
			output = KGlobal::locale()->formatMoney(currentResult);
			break;
	}
	return output;
}


void CalculatorCommandManager::deregister()
{
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

void CalculatorCommandManager::sendOperator(const QString operatorStr)
{
	if (resultCurrentlyDisplayed) {
		ui.leNumber->setText(toString(currentResult));
		resultCurrentlyDisplayed = false;
	}

	ui.leNumber->setText(ui.leNumber->text()+operatorStr);
}

void CalculatorCommandManager::sendBracket(const QString bracketStr)
{
	if (resultCurrentlyDisplayed) 
		resetInput();

	ui.leNumber->setText(ui.leNumber->text()+bracketStr);
}

void CalculatorCommandManager::sendNumber(const QString bracketStr)
{
	if (resultCurrentlyDisplayed) 
		resetInput();

	ui.leNumber->setText(ui.leNumber->text()+bracketStr);
}

void CalculatorCommandManager::sendComma()
{
/*	if (resultCurrentlyDisplayed) {
		clear();
		resultCurrentlyDisplayed = false;
	}

	ui.leNumber->setText(ui.leNumber->text()+KGlobal::locale()->decimalSymbol());*/
	sendOperator(KGlobal::locale()->decimalSymbol());
}

void CalculatorCommandManager::sendPlus()
{
	sendOperator("+");
}

void CalculatorCommandManager::sendMinus()
{
	sendOperator("-");
}

void CalculatorCommandManager::sendMultiply()
{
	sendOperator("*");
}

void CalculatorCommandManager::sendDivide()
{
	sendOperator("/");
}

void CalculatorCommandManager::sendBracketOpen()
{
	sendBracket("(");
}

void CalculatorCommandManager::sendBracketClose()
{
	sendBracket(")");
}

void CalculatorCommandManager::sendPercent()
{
	sendOperator("%");
}


void CalculatorCommandManager::resetInput()
{
	ui.leNumber->clear();
	resultCurrentlyDisplayed=false;
}

void CalculatorCommandManager::sendEquals()
{
	QString input = ui.leNumber->text();
	if (input.contains("="))
		input = input.left(input.indexOf("="));
	QList<Token*> *parsedInput = parseString(input);
	if(parsedInput!=NULL)
	{
		kDebug() << "parsed input";
	    foreach (Token* t, *parsedInput) {
		    kDebug() << t->getType() << t->getNumber() << t->getArOperator();
	    }
	    kDebug() << "end parsed input";
	    QList<Token*> *postfixedInput =  toPostfix(parsedInput);
		kDebug() << "postfixed input";
	    foreach (Token* t, *postfixedInput) {
		    kDebug() << t->getType() << t->getNumber() << t->getArOperator();
	    }
	    kDebug() << "end postfixed input";

	    currentResult = calculate(postfixedInput);
	    //ui.leNumber->setText(QString("%1").arg(output,0,'f',4));
	    ui.leNumber->setText(ui.leNumber->text()+"="+toString(currentResult));
	    resultCurrentlyDisplayed = true;
	}
	else
		resetInput();
}

QString CalculatorCommandManager::toString(double in)
{
	QString out = QString::number(in);
	out.replace(".", KGlobal::locale()->decimalSymbol());
	return out;
}

QList<Token *> * CalculatorCommandManager::parseString(QString calc)
{
	QList<Token *> *list=new QList<Token *>();
	//status: Explains the status from the parser. 0=start, 1=number, 2=comma, 3=arithmetic operator, 4=commanumber, 5=percent, -1=fail
	int status=0;
	double number=0.0;
	bool isFloat=false;
	float decimalMultiplier =  10.0f;
	QString decimal = KGlobal::locale()->decimalSymbol();

	for(int i=0;i<calc.size();i++)
	{
	    kDebug() << calc.at(i) << status;
	    if(calc.at(i)>=48 && calc.at(i)<=57) // digit
	    {
		switch(status)
		{
		    case -1: clear();
			     kDebug() << "Error in 1";
			     SimonInfo::showMessage(i18n("Not a legal expression!"), 3000, new KIcon("accessories-calculator"));
			     break;
		    case 2: number=number+(calc.at(i).digitValue()/decimalMultiplier);
			    decimalMultiplier*=10;
			    status=4;
			    break;
		    case 4: number=number+(calc.at(i).digitValue()/decimalMultiplier);
			    decimalMultiplier*=10;
			    break;
		    case 5:
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
	    else if(calc.at(i)=='(' && (status==3 || status==5 || status==0))
	    {
		list->append(new Token('(', -1));
		status=3;
	    }
	    else if(calc.at(i)==')' && ((status==1)||status==5 || (status==4)))
	    {
		list->append(new Token(')', -1));
	    }
	    else if((status==1) || (status==4) || (status == 5))
	    {
		if((i+1)!=calc.size())
		{
		    if(calc.at(i).toAscii() == decimal.at(0).toAscii())
		    {
			if(!isFloat)
			 {
			  status=2;
			  isFloat=true;
			  decimalMultiplier = 10.0f;
			 }
			else
			  status=-1;
		    }

		    else
		    {
			switch(calc.at(i).toAscii())
			{
			
			    case '+': if (status != 5) list->append(new Token(number));
				      list->append(new Token('+', 1));
				      isFloat=false;
				      status=3;
				      break;
			    case '-': if (status != 5) list->append(new Token(number));
				      list->append(new Token('-',1));
				      isFloat=false;
				      status=3;
				      break;
			    case '*': if (status != 5) list->append(new Token(number));
				      list->append(new Token('*',2));
				      isFloat=false;
				      status=3;
				      break;
			    case '/': if (status != 5) list->append(new Token(number));
				      list->append(new Token('/',2));
				      isFloat=false;
				      status=3;
				      break;
//                            case '%': //list->append(new Token(list->at(list->size()-1)->getNumber()/100*number));
 //                                     break;
			}
		    }
		}
		if (calc.at(i).toAscii() == '%') {
		      list->append(new Token(number));
		      list->append(new Token('%', 3));
		      isFloat=false;
		      status=5;
		} else {
			if (i+1 == calc.size()) {
			    status=-1;
			    clear();
			     kDebug() << "Error in 2";
			    SimonInfo::showMessage(i18n("Not a legal expression!"), 3000, new KIcon("accessories-calculator"));
			}
		}
	    }
	    else
	    {
		status=-1;
		clear();
	     kDebug() << "Error in 3";
		SimonInfo::showMessage(i18n("Not a legal expression!"), 3000, new KIcon("accessories-calculator"));
	    }
	}
	
	if (status != 5)
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
	    //brackets
	if(calcList->at(i)->getType()==-1)
	{
	    if(calcList->at(i)->getArOperator()=='(')
	    {
		arOperatoren->push(calcList->at(i));
	    }
	    if(calcList->at(i)->getArOperator()==')')
	    {
		while(!arOperatoren->isEmpty())
		{
		    if(arOperatoren->top()->getType()!=-1)
		    {
			list->append(arOperatoren->pop());
		    }
		    else
			break;
		}
	    }
	}
	    //number
	else if(calcList->at(i)->getType()==0)
	{
	    list->append((*calcList)[i]);
	}

	    //+, -
	else if(calcList->at(i)->getType()==1)
	{
	    while(!arOperatoren->isEmpty())
	    {
		list->append(arOperatoren->pop());
	    }
	    arOperatoren->push(calcList->at(i));
	}

	    //*, /
	else if(calcList->at(i)->getType()==2)
	{
	    if(!arOperatoren->isEmpty() && arOperatoren->top()->getType()==2) //if there are more then 2 types, exchange the if with a while-loop
	    {
		list->append(arOperatoren->pop());
	    }
	    arOperatoren->push(calcList->at(i));
	}
	else if (calcList->at(i)->getType() == 3) {
		list->append(calcList->at(i));
	}
	else
	{
		kWarning() << "Error in function: toPostfix()";
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
		case '%':  {
			double op1;
			op1 = calc.pop()->getNumber()/100.0f;
		        calc.push(new Token(op1));
			kDebug() << "Pushing percenticed number: " << op1;
			break;
			}
	    }
	}

    }

    delete postList;

    double result = calc.pop()->getNumber();
    kDebug() << "result";
    return result;
}

void CalculatorCommandManager::back()
{
	QString text = ui.leNumber->text();
	if (text.count() == 0) return;

	text = text.left(text.count()-1);

	if (text.count() == 0) clear();
	else ui.leNumber->setText(text);
}

void CalculatorCommandManager::clear()
{
	ui.leNumber->setText("0");
	resultCurrentlyDisplayed = true;
	currentResult=0;
}


void CalculatorCommandManager::cancel()
{
	widget->reject();
}

void CalculatorCommandManager::processRequest(int number)
{
	if (resultCurrentlyDisplayed) 
		resetInput();

	ui.leNumber->setText(ui.leNumber->text()+QString::number(number));
}

void CalculatorCommandManager::ok()
{
	CalculatorConfiguration::OutputModeSelection modeSelection= CalculatorConfiguration::getInstance()->outputModeSelection();
	CalculatorConfiguration::OutputMode mode = CalculatorConfiguration::getInstance()->outputMode();
	kDebug() << modeSelection << mode;

	switch (modeSelection) {
		case CalculatorConfiguration::AlwaysAsk:
			commandListWidget->show();
			break;
		case CalculatorConfiguration::UseDefault:
			writeoutRequestReceived((int) mode);
			break;
		case CalculatorConfiguration::AskButDefaultAfterTimeout:
			commandListWidget->show();
			commandListWidget->selectAfterTimeout((int) mode, CalculatorConfiguration::getInstance()->askTimeout());
			break;
	}
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
		if (commandListWidget->isVisible()) {
			commandListWidget->hide();
			commandListWidget->abortTimeoutSelection();
		} else {
			ui.pbCancel->animateClick();
		}
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
        if(inputText.toUpper() == ui.pbBack->text().remove("&").toUpper()) {
                ui.pbBack->animateClick();
                return true;
        }
        if(inputText.toUpper() == ui.pbClear->text().remove("&").toUpper()) {
                ui.pbClear->animateClick();
                return true;
        }

	return true;
}


bool CalculatorCommandManager::trigger(const QString& triggerName)
{
	if (!triggerName.isEmpty()){
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

void CalculatorCommandManager::activate()
{
	clear();
	QDesktopWidget* tmp = QApplication::desktop();
	int x,y;
	x=(tmp->width()/2)-(widget->width()/2);
	y=(tmp->height()/2)-(widget->height()/2);
	widget->move(x, y);
	widget->show();
	startGreedy();
}

CalculatorCommandManager::~CalculatorCommandManager()
{
	widget->deleteLater();
}

