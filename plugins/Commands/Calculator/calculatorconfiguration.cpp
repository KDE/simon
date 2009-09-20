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

#include "calculatorconfiguration.h"
#include "calculatorcommandmanager.h"
#include <QVariantList>
#include <kgenericfactory.h>
#include <KAboutData>
#include <KMessageBox>

K_PLUGIN_FACTORY_DECLARATION(CalculatorCommandPluginFactory)

QPointer<CalculatorConfiguration> CalculatorConfiguration::instance;


CalculatorConfiguration::CalculatorConfiguration(QWidget *parent, const QVariantList &args)
		: CommandConfiguration("calculator", ki18n( "Calculator" ),
				      "0.1", ki18n("Calculate with your voice"),
				      "accessories-calculator",
				      CalculatorCommandPluginFactory::componentData(),
				      parent)
{
	Q_UNUSED(args);
	ui.setupUi(this);
	
	config = KSharedConfig::openConfig(CalculatorCommandPluginFactory::componentData(),
					"simoncalculatorrc");
	connect(ui.rbOutputAsk, SIGNAL(toggled(bool)), this, SLOT(slotChanged()));
	connect(ui.rbOutputDefault, SIGNAL(toggled(bool)), this, SLOT(slotChanged()));
	connect(ui.rbAskAndDefault, SIGNAL(toggled(bool)), this, SLOT(slotChanged()));
	connect(ui.cbDefaultOutputMode, SIGNAL(currentIndexChanged(int)), this, SLOT(slotChanged()));
	connect(ui.sbOutputDefaultTimeout, SIGNAL(valueChanged(double)), this, SLOT(slotChanged()));
	load();
}


void CalculatorConfiguration::slotChanged()
{
	emit changed(true);
}

void CalculatorConfiguration::save()
{
	Q_ASSERT(config);
	
	KConfigGroup cg(config, "");
	cg.writeEntry("OutputModeSelection", (int) outputModeSelection());
	cg.writeEntry("DefaultOutputMode", ((int) outputMode())-1);
	cg.writeEntry("AskTimeout", ui.sbOutputDefaultTimeout->value());
	cg.sync();
	
	emit changed(false);
}

/*void CalculatorConfiguration::destroy()
{
	deleteLater();
	instance=0;
}*/
 
void CalculatorConfiguration::load()
{
	Q_ASSERT(config);

	KConfigGroup cg(config, "");
	CalculatorConfiguration::OutputModeSelection modeSelection = 
			(CalculatorConfiguration::OutputModeSelection) cg.readEntry("OutputModeSelection", 0);
	switch (modeSelection) {
		case CalculatorConfiguration::AlwaysAsk:
			ui.rbOutputAsk->setChecked(true);
			break;
		case CalculatorConfiguration::UseDefault:
			ui.rbOutputDefault->setChecked(true);
			break;
		case CalculatorConfiguration::AskButDefaultAfterTimeout:
			ui.rbAskAndDefault->setChecked(true);
			break;
	}

	ui.cbDefaultOutputMode->setCurrentIndex(cg.readEntry("DefaultOutputMode", 0));
	ui.sbOutputDefaultTimeout->setValue(cg.readEntry("AskTimeout", 12));

	cg.sync();
	
	emit changed(false);
}
 
void CalculatorConfiguration::defaults()
{
	ui.rbOutputAsk->animateClick();
	ui.sbOutputDefaultTimeout->setValue(12);
	ui.cbDefaultOutputMode->setCurrentIndex(0);
	save();
}

/**
 * \return timeout in milliseconds
 */
int CalculatorConfiguration::askTimeout()
{
	double timeoutD = ui.sbOutputDefaultTimeout->value();
	return qRound(timeoutD*1000.0f);
}

CalculatorConfiguration::OutputModeSelection CalculatorConfiguration::outputModeSelection()
{
	if (ui.rbOutputAsk->isChecked())
		return CalculatorConfiguration::AlwaysAsk;
	else if (ui.rbOutputDefault->isChecked())
		return CalculatorConfiguration::UseDefault;
	else 
		return CalculatorConfiguration::AskButDefaultAfterTimeout;
}


CalculatorConfiguration::OutputMode CalculatorConfiguration::outputMode()
{
	return (CalculatorConfiguration::OutputMode) (ui.cbDefaultOutputMode->currentIndex()+1);
}


CalculatorConfiguration::~CalculatorConfiguration()
{
	
}
