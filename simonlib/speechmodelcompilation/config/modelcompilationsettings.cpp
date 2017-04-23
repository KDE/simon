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

#include "modelcompilationsettings.h"
#include "modelcompilationconfiguration.h"
#include <KLineEdit>
#include <KPageWidget>
#include <QDebug>
#include <KPluginFactory>

K_PLUGIN_FACTORY( ModelCompilationSettingsFactory,
registerPlugin< ModelCompilationSettings >();
)

K_EXPORT_PLUGIN( ModelCompilationSettingsFactory("simonlib") )

ModelCompilationSettings::ModelCompilationSettings(QWidget* parent, const QVariantList& args): KCModule(KGlobal::mainComponent(), parent)
{
  QVBoxLayout *lay = new QVBoxLayout(this);
  KPageWidget *pageWidget = new KPageWidget(this);
  lay->addWidget(pageWidget);

  if (args.count() == 1)
    pageWidget->setFaceType(KPageView::Tabbed);

  // 	QWidget *trainingsDataWidget = new QWidget(pageWidget);
  // 	modelSettingsUi.setupUi(trainingsDataWidget);

  QWidget *externalProgramsWidget = new QWidget(pageWidget);
  externalProgramsUi.setupUi(externalProgramsWidget);

  // 	KPageWidgetItem *trainingsData = pageWidget->addPage(trainingsDataWidget, i18n("Trainingsdaten"));
  KPageWidgetItem *externalPrograms = pageWidget->addPage(externalProgramsWidget, i18n("External Programs"));

  // 	trainingsData->setIcon(KIcon("view-pim-news"));
  externalPrograms->setIcon(KIcon("applications-other"));

  // 	trainingsData->setHeader("");
  externalPrograms->setHeader("");

  addConfig(ModelCompilationConfiguration::self(), this);
}

void ModelCompilationSettings::load()
{
  KCModule::load();

#ifdef BACKEND_TYPE_JHTK
  externalProgramsUi.kcfg_backend->setCurrentIndex(1);
  externalProgramsUi.kcfg_backend->setEnabled(false);
  externalProgramsUi.gbSphinx->hide();
#endif
}

int ModelCompilationSettings::getDefaultBackendType()
{
  int type(0);
#ifdef BACKEND_TYPE_JHTK
  type = 1;
#endif
  return type;
}


ModelCompilationSettings::~ModelCompilationSettings()
{}
