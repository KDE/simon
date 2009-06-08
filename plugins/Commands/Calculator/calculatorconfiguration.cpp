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
					"calculatorrc");

	QObject::connect(ui.leTrigger, SIGNAL(textChanged(QString)), this, SLOT(slotChanged()));
}

QString CalculatorConfiguration::trigger()
{
	KConfigGroup cg(config, "");
	return cg.readEntry("Trigger", i18n("Calculator"));
}


void CalculatorConfiguration::save()
{
	Q_ASSERT(config);
	
	KConfigGroup cg(config, "");
	cg.writeEntry("Trigger", ui.leTrigger->text());

	cg.sync();
	
	emit changed(false);
}

void CalculatorConfiguration::destroy()
{
	deleteLater();
	instance=0;
}
 
void CalculatorConfiguration::load()
{
	Q_ASSERT(config);

	KConfigGroup cg(config, "");
	ui.leTrigger->setText(cg.readEntry("Trigger", i18n("Calculator")));

	cg.sync();
	
	emit changed(false);
}
 
void CalculatorConfiguration::defaults()
{
	ui.leTrigger->setText(i18n("Calculator"));
	save();
}

CalculatorConfiguration::~CalculatorConfiguration()
{
	
}
