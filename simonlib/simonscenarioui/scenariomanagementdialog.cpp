/*
 *   Copyright (C) 2009 Peter Grasch <peter.grasch@bedahr.org>
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

#include "scenariomanagementdialog.h"
#include "newscenario.h"
#include "scenariomanagementwidget.h"
#include <simonscenarios/author.h>
#include <simonscenarios/versionnumber.h>
#include <simonscenarios/scenario.h>
#include <simonscenarios/scenariomanager.h>
#include <QWidget>
#include <QTreeWidget>
#include <KSharedConfig>
#include <QDebug>
#include <KWidgetsAddons/KMessageBox>


#include <knewstuff3/downloaddialog.h>
#include <knewstuff3/uploaddialog.h>

#include "ui_scenariomanagementdlg.h"

ScenarioManagementDialog::ScenarioManagementDialog(const QString& dataPrefix, QWidget *parent) : QDialog(parent),
  managementWidget(new ScenarioManagementWidget(dataPrefix, false /* not minimal */, parent))
{
  QVBoxLayout *mainLayout = new QVBoxLayout;
  setLayout(mainLayout);
  mainLayout->addWidget(managementWidget);
  setAcceptDrops(true);
  managementWidget->setAcceptDrops(true);
  setWindowTitle( i18n("Manage scenarios") );
}

int ScenarioManagementDialog::exec()
{
  managementWidget->init();
  return QDialog::exec();
}

bool ScenarioManagementDialog::updateScenarioConfiguration()
{
  int ret;
  do {
    ret = exec();
    if (!ret) break;
  } while (!managementWidget->save());
  return ret;

}

void ScenarioManagementDialog::configureScenarios(QWidget *parent)
{
  QPointer<ScenarioManagementDialog> dlg(new ScenarioManagementDialog("simon/", parent));
  if (dlg->updateScenarioConfiguration())
  {
    //reload scenario information
    qDebug() << "Reloading Scenario Information";

    if (!ScenarioManager::getInstance()->setupScenarios(true /* force change */))
      KMessageBox::sorry(parent, i18n("Could not re-initialize scenarios. Please restart Simon."));
  }
  delete dlg;
}

QStringList ScenarioManagementDialog::getSelectedScenarioIds()
{
  return managementWidget->getSelectedScenarioIds();
}

ScenarioManagementDialog::~ScenarioManagementDialog()
{
  delete managementWidget;
}
