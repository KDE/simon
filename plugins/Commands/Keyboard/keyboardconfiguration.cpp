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

#include "keyboardconfiguration.h"
#include "keyboardcommandmanager.h"
#include <QVariantList>
#include <kgenericfactory.h>
#include <KAboutData>
#include <KMessageBox>

K_PLUGIN_FACTORY_DECLARATION(KeyboardCommandPluginFactory)

QPointer<KeyboardConfiguration> KeyboardConfiguration::instance;


KeyboardConfiguration::KeyboardConfiguration(QWidget *parent, const QVariantList &args)
		: CommandConfiguration("keyboard", ki18n( "Keyboard" ),
				      "0.1", ki18n("Input signes with ease"),
				      "accessories-calculator",
				      KeyboardCommandPluginFactory::componentData(),
				      parent)
{
	Q_UNUSED(args);
	ui.setupUi(this);
	
	config = KSharedConfig::openConfig(KeyboardCommandPluginFactory::componentData(),
					"keyboardrc");

	QObject::connect(ui.leTrigger, SIGNAL(textChanged(QString)), this, SLOT(slotChanged()));
        connect(ui.tbAddSet, SIGNAL(clicked()), this, SLOT(addSet()));
        connect(ui.tbDeleteSet, SIGNAL(clicked()), this, SLOT(deleteSet()));
        connect(ui.tbAddTab, SIGNAL(clicked()), this, SLOT(addTab()));
        connect(ui.tbDeleteTab, SIGNAL(clicked()), this, SLOT(deleteTab()));
        connect(ui.pbAddButton, SIGNAL(clicked()), this, SLOT(addButton()));
        connect(ui.pbEditButton, SIGNAL(clicked()), this, SLOT(editButton()));
        connect(ui.pbDeleteButton, SIGNAL(clicked()), this, SLOT(deleteButton()));
}

QString KeyboardConfiguration::trigger()
{
	KConfigGroup cg(config, "");
	return cg.readEntry("Trigger", i18n("Keyboard"));
}

void KeyboardConfiguration::addSet()
{
}
void KeyboardConfiguration::deleteSet()
{
}
void KeyboardConfiguration::addTab()
{
}
void KeyboardConfiguration::deleteTab()
{
}
void KeyboardConfiguration::addButton()
{
}
void KeyboardConfiguration::editButton()
{
}
void KeyboardConfiguration::deleteButton()
{
}

void KeyboardConfiguration::save()
{
	Q_ASSERT(config);
	
	KConfigGroup cg(config, "");
	cg.writeEntry("Trigger", ui.leTrigger->text());

	cg.sync();
	
	emit changed(false);
}

void KeyboardConfiguration::destroy()
{
	deleteLater();
	instance=0;
}
 
void KeyboardConfiguration::load()
{
	Q_ASSERT(config);

	KConfigGroup cg(config, "");
	ui.leTrigger->setText(cg.readEntry("Trigger", i18n("Keyboard")));

	cg.sync();
	
	emit changed(false);
}
 
void KeyboardConfiguration::defaults()
{
	ui.leTrigger->setText(i18n("Keyboard"));
	save();
}

KeyboardConfiguration::~KeyboardConfiguration()
{
	
}
