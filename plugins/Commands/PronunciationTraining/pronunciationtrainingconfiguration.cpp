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

#include "pronunciationtrainingconfiguration.h"
#include "pronunciationtrainingcommandmanager.h"
#include <speechmodelmanagement/grammarmanager.h>
#include <QVariantList>
#include <kgenericfactory.h>
#include <KAboutData>
#include <KMessageBox>

K_PLUGIN_FACTORY_DECLARATION(PronunciationTrainingPluginFactory)

QPointer<PronunciationTrainingConfiguration> PronunciationTrainingConfiguration::instance;


PronunciationTrainingConfiguration::PronunciationTrainingConfiguration(QWidget *parent, const QVariantList &args)
		: CommandConfiguration("pronunciationtraining", ki18n( "Pronunciation Training" ),
				      "0.1", ki18n("Pronunciation Training"),
				      "games-config-board",
				      PronunciationTrainingPluginFactory::componentData(),
				      parent)
{
	Q_UNUSED(args);
	ui.setupUi(this);
	
	config = KSharedConfig::openConfig(PronunciationTrainingPluginFactory::componentData(),
					"pronunciationtrainingrc");

	ui.cbTerminal->clear();
	ui.cbTerminal->addItems(GrammarManager::getInstance()->getTerminals());

	QObject::connect(ui.leTrigger, SIGNAL(textChanged(QString)), this, SLOT(slotChanged()));
	QObject::connect(ui.cbTerminal, SIGNAL(currentIndexChanged(int)), this, SLOT(slotChanged()));
	
// 	if (instance) instance->deleteLater();
// 	instance = this;
}

QString PronunciationTrainingConfiguration::trigger()
{
	KConfigGroup cg(config, "");
	return cg.readEntry("Trigger", i18n("Start Training"));
}

void PronunciationTrainingConfiguration::save()
{
	Q_ASSERT(config);
	
	KConfigGroup cg(config, "");
	cg.writeEntry("Trigger", ui.leTrigger->text());
	cg.writeEntry("Terminal", ui.cbTerminal->currentText());

	cg.sync();
	
	emit changed(false);
}

void PronunciationTrainingConfiguration::destroy()
{
	deleteLater();
	instance=0;
}
 
void PronunciationTrainingConfiguration::load()
{
	Q_ASSERT(config);

	KConfigGroup cg(config, "");
	ui.leTrigger->setText(cg.readEntry("Trigger", i18n("Start Training")));
	ui.cbTerminal->setCurrentIndex(ui.cbTerminal->findText(cg.readEntry("Terminal")));

	cg.sync();
	
	emit changed(false);
}
 
void PronunciationTrainingConfiguration::defaults()
{
	ui.leTrigger->setText(i18n("Start Training"));
 
	save();
}

PronunciationTrainingConfiguration::~PronunciationTrainingConfiguration()
{
	
}
