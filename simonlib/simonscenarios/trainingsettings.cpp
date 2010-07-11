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

#include "modelcompilationsettings.h"
#include "acousticmodelmanagementconfiguration.h"
#include "trainingmanager.h"
#include <KLineEdit>
#include <KPageWidget>
#include <kgenericfactory.h>
#include <QDebug>

K_PLUGIN_FACTORY( TrainingSettingsFactory,
registerPlugin< TrainingSettings >();
)

K_EXPORT_PLUGIN( TrainingSettingsFactory("simonlib") )

TrainingSettings::TrainingSettings(QWidget* parent, const QVariantList& args): KCModule(KGlobal::mainComponent(), parent)
{
  QVBoxLayout *lay = new QVBoxLayout(this);
  KPageWidget *pageWidget = new KPageWidget(this);
  lay->addWidget(pageWidget);

  if (args.count() == 1)
    pageWidget->setFaceType(KPageView::Tabbed);

  QWidget *trainingsDataWidget = new QWidget(pageWidget);
  trainingUi.setupUi(trainingsDataWidget);

  KPageWidgetItem *trainingsData = pageWidget->addPage(trainingsDataWidget, i18n("Trainingsdata"));

  trainingsData->setIcon(KIcon("view-pim-news"));

  trainingsData->setHeader("");

  addConfig(AcousticModelManagementConfiguration::self(), this);
}


void TrainingSettings::save()
{
  TrainingManager::getInstance()->trainingSettingsSaved();

  KCModule::save();
}


TrainingSettings::~TrainingSettings()
{}
