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

#include "aiconfiguration.h"
#include "aicommandmanager.h"
#include <QVariantList>
#include <QDir>
#include <kgenericfactory.h>
#include <KAboutData>
#include <KMessageBox>
#include <KStandardDirs>

K_PLUGIN_FACTORY_DECLARATION(AIPluginFactory)

QPointer<AIConfiguration> AIConfiguration::instance;

AIConfiguration::AIConfiguration(QWidget *parent, const QVariantList &args)
		: CommandConfiguration("ai", ki18n( "Künstliche Intelligenz" ),
				      "0.1", ki18n("Sprachgesteuerter Chatbot"),
				      "view-media-artist",
				      AIPluginFactory::componentData(),
				      parent)
{
	Q_UNUSED(args);
	ui.setupUi(this);

	manager = 0;
	
	config = KSharedConfig::openConfig(AIPluginFactory::componentData(),
					"airc");

	QObject::connect(ui.cbAimlSets, SIGNAL(currentIndexChanged(int)), this, SLOT(slotChanged()));
}

void AIConfiguration::setManager(AICommandManager *manager)
{
	this->manager = manager;
}

void AIConfiguration::save()
{
	Q_ASSERT(config);

	QString oldStoredAimlSet = storedAimlSet;
	storedAimlSet = ui.cbAimlSets->currentText();

	if (storedAimlSet != oldStoredAimlSet)
	{
		if (manager) manager->setupParser();

		KConfigGroup cg(config, "");
		cg.writeEntry("Personality", storedAimlSet);
		cg.sync();
	}
	
	emit changed(false);
}

void AIConfiguration::destroy()
{
	deleteLater();
	instance=0;
}
 
void AIConfiguration::load()
{
	Q_ASSERT(config);

	ui.cbAimlSets->clear();
	ui.cbAimlSets->addItems(QDir(KStandardDirs::locate("data", "ai/aimls/")).entryList(QStringList(), QDir::Dirs|QDir::NoDotAndDotDot));

	KConfigGroup cg(config, "");

	ui.cbAimlSets->setCurrentIndex(ui.cbAimlSets->findText(cg.readEntry("Personality", "Alice")));

	storedAimlSet = ui.cbAimlSets->currentText();

	cg.sync();
	
	emit changed(false);
}
 
void AIConfiguration::defaults()
{
	ui.cbAimlSets->setCurrentIndex(ui.cbAimlSets->findText("Alice"));
 
	save();
}

AIConfiguration::~AIConfiguration()
{
	
}
