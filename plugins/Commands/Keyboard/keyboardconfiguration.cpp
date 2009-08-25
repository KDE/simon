/*
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

#include "inputnumberconfiguration.h"
#include "inputnumbercommandmanager.h"
#include <QVariantList>
#include <kgenericfactory.h>
#include <KAboutData>
#include <KMessageBox>

K_PLUGIN_FACTORY_DECLARATION(InputNumberCommandPluginFactory)

QPointer<InputNumberConfiguration> InputNumberConfiguration::instance;


InputNumberConfiguration::InputNumberConfiguration(QWidget *parent, const QVariantList &args)
		: CommandConfiguration("inputnumber", ki18n( "Input Number" ),
				      "0.1", ki18n("Input numbers with ease"),
				      "accessories-calculator",
				      InputNumberCommandPluginFactory::componentData(),
				      parent)
{
	Q_UNUSED(args);
	ui.setupUi(this);
	
	config = KSharedConfig::openConfig(InputNumberCommandPluginFactory::componentData(),
					"inputnumberrc");

	QObject::connect(ui.leTrigger, SIGNAL(textChanged(QString)), this, SLOT(slotChanged()));
        connect(ui.tbAddSet, SIGNAL(clicked()), this, SLOT(addSet()));
        connect(ui.tbDeleteSet, SIGNAL(clicked()), this, SLOT(deleteSet()));
        connect(ui.tbAddTab, SIGNAL(clicked()), this, SLOT(addTab()));
        connect(ui.tbDeleteTab, SIGNAL(clicked()), this, SLOT(deleteTab()));
        connect(ui.pbAddButton, SIGNAL(clicked()), this, SLOT(addButton()));
        connect(ui.pbEditButton, SIGNAL(clicked()), this, SLOT(editButton()));
        connect(ui.pbDeleteButton, SIGNAL(clicked()), this, SLOT(deleteButton()));
}

QString InputNumberConfiguration::trigger()
{
	KConfigGroup cg(config, "");
	return cg.readEntry("Trigger", i18n("Number"));
}


void InputNumberConfiguration::save()
{
	Q_ASSERT(config);
	
	KConfigGroup cg(config, "");
	cg.writeEntry("Trigger", ui.leTrigger->text());

	cg.sync();
	
	emit changed(false);
}

void InputNumberConfiguration::destroy()
{
	deleteLater();
	instance=0;
}
 
void InputNumberConfiguration::load()
{
	Q_ASSERT(config);

	KConfigGroup cg(config, "");
	ui.leTrigger->setText(cg.readEntry("Trigger", i18n("Number")));

	cg.sync();
	
	emit changed(false);
}
 
void InputNumberConfiguration::defaults()
{
	ui.leTrigger->setText(i18n("Number"));
	save();
}

InputNumberConfiguration::~InputNumberConfiguration()
{
	
}
