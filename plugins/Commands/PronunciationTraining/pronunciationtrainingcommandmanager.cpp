/*
 *   Copyright (C) 2008 Peter Grasch <peter.grasch@bedahr.org>
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
#include "pronunciationtraining.h"
#include <simonlogging/logger.h>
#include <KLocalizedString>
#include <KGenericFactory>
#include <KMessageBox>
#include <KDebug>
#include <KAction>
#include "pronunciationtrainingconfiguration.h"

K_PLUGIN_FACTORY( PronunciationTrainingPluginFactory,
registerPlugin< PronunciationTrainingCommandManager >();
)

K_EXPORT_PLUGIN( PronunciationTrainingPluginFactory("simonpronunciationtrainingcommand") )

PronunciationTrainingCommandManager::PronunciationTrainingCommandManager(QObject* parent, const QVariantList& args) : CommandManager((Scenario*) parent, args),
activateAction(new KAction(this))
{
  activateAction->setText(i18n("Activate Pronunciation Training"));
  activateAction->setIcon(icon());
  connect(activateAction, SIGNAL(triggered(bool)),
    this, SLOT(activateTraining()));
  guiActions << activateAction;
}


const QString PronunciationTrainingCommandManager::name() const
{

  return i18n("Pronunciation Training");
}


void PronunciationTrainingCommandManager::activateTraining()
{
  PronunciationTraining *training = new PronunciationTraining(static_cast<PronunciationTrainingConfiguration*>(config)->terminal());
  training->init();
  training->show();
}


const QString PronunciationTrainingCommandManager::preferredTrigger() const
{
  return i18n("Pronunciation Training");
}


bool PronunciationTrainingCommandManager::trigger(const QString& triggerName, bool silent)
{
  Q_UNUSED(silent);
  
  if (!triggerName.isEmpty()) return false;

  Logger::log(i18n("Activating pronunciationtraining"));
  activateTraining();
  return true;
}


bool PronunciationTrainingCommandManager::deSerializeConfig(const QDomElement& elem)
{
  config = new PronunciationTrainingConfiguration(parentScenario);
  return config->deSerialize(elem);
}


const QString PronunciationTrainingCommandManager::iconSrc() const
{
  return ("applications-education");
}


PronunciationTrainingCommandManager::~PronunciationTrainingCommandManager()
{
}
