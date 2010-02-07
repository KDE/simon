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

#ifndef SIMON_CALCULATORCOMMANDMANAGER_H_69F8482CCD434513BBC89310A2B529BE
#define SIMON_CALCULATORCOMMANDMANAGER_H_69F8482CCD434513BBC89310A2B529BE

#include <simonscenarios/commandmanager.h>
#include <simonscenarios/commandlauncher.h>
#include <simonactions/greedyreceiver.h>
#include <QVariantList>
#include <QList>
#include <QStack>
#include "ui_calculatorwidget.h"
#include "token.h"

class QDialog;
class CommandListWidget;

/**
 *	@class CalculatorCommandManager
 *	@brief Manager for the calculator commands
 *
 *	@version 0.1
 *	@date 20.05.2008
 *	@author Peter Grasch
 */
class CalculatorCommandManager : public CommandManager, public GreedyReceiver {
Q_OBJECT
private:
	enum NumberType {
		Default=1,
		Money=2
	};

	Ui::CalculatorDlg ui;
	QDialog *widget;
	CommandListWidget *commandListWidget;
	static QStringList numberIdentifiers;
	QList<Token *> * parseString(QString calc);
	QList<Token *> * toPostfix(QList<Token *> *calcList);
	double calculate(QList<Token *>* postList);

	double currentResult;
	bool resultCurrentlyDisplayed;

	void sendOperator(const QString operatorStr);
	void sendBracket(const QString bracketStr);
	void sendNumber(const QString bracketStr);
	void resetInput();
	QString toString(double in);

	QString formatCalculation(CalculatorCommandManager::NumberType type);
	QString formatInput(CalculatorCommandManager::NumberType type);
	QString formatOutput(CalculatorCommandManager::NumberType type);

	bool installInterfaceCommands();
	
private slots:
	void deregister();
	void ok();
	void back();
	void cancel();
	void clear();
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
	void sendPlus();
	void sendMinus();
	void sendMultiply();
	void sendDivide();
	void sendBracketOpen();
	void sendBracketClose();
	void sendEquals();
        void sendPercent();
	void writeoutRequestReceived(int index);

	void printCancel();
	void printResult() { writeoutRequestReceived(1); }
	void printCalculationAndResult() { writeoutRequestReceived(2); }
	void printFormattedResult() { writeoutRequestReceived(3); }
	void printFormattedCalculationAndResult() { writeoutRequestReceived(4); }
	void printFormattedMoneyResult() { writeoutRequestReceived(5); }
	void printFormattedMoneyCalculationAndResult() { writeoutRequestReceived(6); }

public slots:
	void activate();

public:
	const QString preferredTrigger() const;
	const KIcon icon() const;
	const QString iconSrc() const;
	const QString name() const;
	void setFont(const QFont& font);

	bool deSerializeConfig(const QDomElement& elem);

	//CommandList* getCommands() const { return NULL; }

//	QList<CommandLauncher*> launchers() const;


    /**
    * @brief Constructor
    * 
    *	@author Peter Grasch
    */
    CalculatorCommandManager(QObject* parent, const QVariantList& args);

    
    ~CalculatorCommandManager();

};

#endif
