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

#include <commandpluginbase/commandconfiguration.h>
#include "ui_calculatorconfigurationdlg.h"
#include <KSharedConfig>
#include <QPointer>

class CalculatorConfiguration : public CommandConfiguration
{
	Q_OBJECT
	
	private:
		Ui::CalculatorConfigurationDlg ui;
		static QPointer<CalculatorConfiguration> instance;
 
	public slots:
		virtual void save();
		virtual void load();
		virtual void defaults();

	private slots:
		void slotChanged();
	
	public:
		enum OutputModeSelection {
			AlwaysAsk=1,
			UseDefault=2,
			AskButDefaultAfterTimeout=3
		};

		enum OutputMode {
			Result=1,
			CalculationAndResult=2,
			FormattedResult=3,
			FormattedCalculationAndResult=4,
			FormattedMoneyResult=5,
			FormattedMoneyCalculationAndResult=6
		};


		static CalculatorConfiguration *getInstance(QWidget *parent=0, const QVariantList &args = QVariantList()) {
			if (!instance) instance = new CalculatorConfiguration(parent, args);
			return instance;
		}
		CalculatorConfiguration(QWidget *parent=0, const QVariantList &args = QVariantList());
		~CalculatorConfiguration();

		
		int askTimeout();
		OutputModeSelection outputModeSelection();
		OutputMode outputMode();
};

#endif
