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

#include "pronunciationtrainingcommandmanager.h"
#include <simonlogging/logger.h>
#include <KLocalizedString>
#include <KGenericFactory>
#include <KMessageBox>
#include <KAction>
#include <KActionCollection>
#include "pronunciationtrainingconfiguration.h"

K_PLUGIN_FACTORY( PronunciationTrainingPluginFactory, 
			registerPlugin< PronunciationTrainingCommandManager >(); 
			registerPlugin< PronunciationTrainingConfiguration >(); 
		)
        
K_EXPORT_PLUGIN( PronunciationTrainingPluginFactory("simonpronunciationtrainingcommand") )



PronunciationTrainingCommandManager::PronunciationTrainingCommandManager(QObject *parent, const QVariantList& args) : CommandManager(parent, args)
{
	setXMLFile("simonpronunciationtrainingpluginui.rc");
	activateAction = new KAction(this);
	activateAction->setText(i18n("Activate Pronunciation Training"));
	activateAction->setIcon(KIcon("go-right"));
	connect(activateAction, SIGNAL(triggered(bool)),
		this, SLOT(activateTraining()));
	actionCollection()->addAction("simonpronunciationtrainingplugin", activateAction);
}

const QString PronunciationTrainingCommandManager::name() const
{
	return i18n("Pronunciation Training");
}

CommandConfiguration* PronunciationTrainingCommandManager::getConfigurationPage()
{
	return PronunciationTrainingConfiguration::getInstance();
}

void PronunciationTrainingCommandManager::activateTraining()
{
	fprintf(stderr, "Activating training...\n");
}

bool PronunciationTrainingCommandManager::trigger(const QString& triggerName)
{
	if (triggerName != PronunciationTrainingConfiguration::getInstance()->trigger()) return false;

	Logger::log(i18n("[INF] Activating pronunciationtraining"));
	activateTraining();
	return true;
}
 
bool PronunciationTrainingCommandManager::load()
{
	PronunciationTrainingConfiguration::getInstance(dynamic_cast<QWidget*>(parent()), QVariantList())->load();
	return true;
}

bool PronunciationTrainingCommandManager::save()
{
	return true;
}

PronunciationTrainingCommandManager::~PronunciationTrainingCommandManager()
{
	activateAction->deleteLater();
}
