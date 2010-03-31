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

#ifndef SIMON_CALCULATORCONFIGURATION_H_2E45638A02CF487A8B39BFA49677F4F8
#define SIMON_CALCULATORCONFIGURATION_H_2E45638A02CF487A8B39BFA49677F4F8

#include <simonscenarios/commandconfiguration.h>
#include "ui_calculatorconfigurationdlg.h"
#include <KSharedConfig>
#include <QPointer>

class CalculatorConfiguration : public CommandConfiguration
{
	Q_OBJECT
	
	private:
		Ui::CalculatorConfigurationDlg ui;
 
	public slots:
		virtual bool deSerialize(const QDomElement&);
		virtual QDomElement serialize(QDomDocument *doc);
		virtual void defaults();

	public:
		enum OutputModeSelection {
			AlwaysAsk=1,
			UseDefault=2,
			AskButDefaultAfterTimeout=3
		};

		enum OutputMode {
			Result=1,
			Calculation=2,
			CalculationAndResult=3,
			FormattedResult=4,
			FormattedCalculationAndResult=5,
			FormattedMoneyResult=6,
			FormattedMoneyCalculationAndResult=7
		};

		enum ControlMode {
			FullCalculator=1,
			OnlyNumberInput=2,
			OnlyOutput=3
		};



		CalculatorConfiguration(Scenario *parent, const QVariantList &args = QVariantList());
		~CalculatorConfiguration();

		
		int askTimeout();
		ControlMode controlMode();
		OutputModeSelection outputModeSelection();
		OutputMode outputMode();
};

#endif
