/*
 *   Copyright (C) 2010 Peter Grasch <peter.grasch@bedahr.org>
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

#include "firstrunscenariosconfig.h"
#include <KLocalizedString>
#include <KMessageBox>
#include <QLayout>
#include <simonscenarioui/scenariomanagementdialog.h>
#include <simonscenarioui/scenariomanagementwidget.h>
#include <simonscenarios/scenariomanager.h>

FirstRunScenariosConfig::FirstRunScenariosConfig(QWidget* parent)
: QWizardPage(parent), scenarioManagement(new ScenarioManagementWidget("simon/", true, this))
{
  ui.setupUi(this);
  setTitle(i18n("Scenarios"));
//   connect(ui.pbGetScenarios, SIGNAL(clicked()), this, SLOT(getScenarios()));
  layout()->addWidget(scenarioManagement);
}

void FirstRunScenariosConfig::initializePage()
{
  scenarioManagement->init();
  QWizardPage::initializePage();
}
bool FirstRunScenariosConfig::validatePage()
{
  bool succ = scenarioManagement->save();
  if (succ && !ScenarioManager::getInstance()->setupScenarios(true /* force change */))
    KMessageBox::sorry(this, i18n("Could not re-initialize scenarios. Please restart Simon!"));
  return succ && QWizardPage::validatePage();
}



FirstRunScenariosConfig::~FirstRunScenariosConfig()
{
}
