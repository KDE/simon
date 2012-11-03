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

#include "manageactionsdialog.h"
#include "manageactionsautorunwidget.h"
#include "addactiondialog.h"

#include <simonactions/listconfiguration.h>

#include <simonscenarios/scenariomanager.h>
#include <simonscenarios/scenario.h>
#include <simonscenarios/action.h>
#include <simonscenarios/commandconfiguration.h>
#include <simonscenarios/actioncollection.h>

#include <KMessageBox>
#include <KAboutData>
#include <KPageWidget>

ManageActionsDialog::ManageActionsDialog(QWidget* parent) : KDialog(parent),
pageWidget(new KPageWidget(this)),
manageActionsAutorunWidget(new ManageActionsAutorunWidget(this)),
listConfiguration(new ListConfiguration(this))
{
  setCaption( i18n("Manage actions") );
  setMainWidget( pageWidget );

  QWidget *baseWidget = new QWidget( this );
  ui.setupUi(baseWidget);

  KPageWidgetItem *generalItem = pageWidget->addPage(baseWidget, i18nc("General settings page", "General"));
  generalItem->setIcon(KIcon("fork"));
  generalItem->setHeader("");

  ui.pbApplyForAll->setIcon(KIcon("arrow-down-double"));

  ui.pbAdd->setIcon(KIcon("list-add"));
  ui.pbRemove->setIcon(KIcon("list-remove"));
  ui.pbUp->setIcon(KIcon("go-up"));
  ui.pbDown->setIcon(KIcon("go-down"));

  connect(ui.pbAdd, SIGNAL(clicked()), this, SLOT(add()));
  connect(ui.pbRemove, SIGNAL(clicked()), this, SLOT(remove()));
  connect(ui.pbUp, SIGNAL(clicked()), this, SLOT(moveUp()));
  connect(ui.pbDown, SIGNAL(clicked()), this, SLOT(moveDown()));
  connect(ui.leTrigger, SIGNAL(textChanged(QString)), this, SLOT(applyTrigger(QString)));
  connect(ui.pbApplyForAll, SIGNAL(clicked()), this, SLOT(applyTriggerToAll()));

  ui.lvPlugins->setIconSize(QSize(24,24));
  ui.lvPlugins->setSpacing(2);

  setButtons(KDialog::Ok);

  connect(ui.lvPlugins, SIGNAL(clicked(QModelIndex)), this, SLOT(currentSelectionChanged()));

  ui.twActionConfig->addTab(listConfiguration, i18n("Lists"));
  ui.twActionConfig->addTab(manageActionsAutorunWidget, i18n("Autorun"));
}


void ManageActionsDialog::applyTrigger(const QString& newTrigger)
{
  Action *a = getCurrentlySelectedAction();
  if (!a) return;

  a->setTrigger(newTrigger);
}


void ManageActionsDialog::applyTriggerToAll()
{
  QString trigger = ui.leTrigger->text();
  ScenarioManager::getInstance()->getCurrentScenario()->actionCollection()->setTrigger(trigger);
}


void ManageActionsDialog::currentSelectionChanged()
{
  Action *a = getCurrentlySelectedAction();
  ui.leTrigger->setText(a->trigger());
}


int ManageActionsDialog::exec()
{
  ActionCollection *aC = ScenarioManager::getInstance()->getCurrentScenario()->actionCollection();
  ui.lvPlugins->setModel(aC);

  configurationPages = aC->getConfigurationPages();

  foreach (CommandConfiguration *m, configurationPages) {
    registerCommandConfiguration(m);
  }

  listConfiguration->prepareToLoad();
  listConfiguration->registerVoiceInterfaceCommands(ScenarioManager::getInstance()->
    getCurrentScenario()->getListInterfaceCommands());
  listConfiguration->loadFinished();

  ScenarioManager::getInstance()->startGroup();
  int ret = KDialog::exec();
  //write configuration changes of the plugins
  if (ret) {
    listConfiguration->prepareToSave();
    ScenarioManager::getInstance()->getCurrentScenario()->setListInterfaceCommands(
      listConfiguration->getListInterfaceCommands());
    manageActionsAutorunWidget->save();
    ScenarioManager::getInstance()->getCurrentScenario()->save();
  }
  ScenarioManager::getInstance()->commitGroup();
  return ret;
}


void ManageActionsDialog::registerCommandConfiguration(CommandConfiguration *m)
{
  if (!m) return;
  QString moduleName = m->aboutData()->programName();
  ProtectorWidget *p = new ProtectorWidget(m, pageWidget);
  KPageWidgetItem *newItem = pageWidget->addPage(p, moduleName);

  QString moduleIcon = m->aboutData()->programIconName();
  newItem->setIcon(KIcon(moduleIcon));

  pages.insert(m, newItem);
}


void ManageActionsDialog::add()
{
  QPointer<AddActionDialog> a = new AddActionDialog(this);
  if (a->exec()) {
    Action *newAction = a->getAction();

    Q_ASSERT(newAction);

    ScenarioManager::getInstance()->getCurrentScenario()->actionCollection()->addAction(newAction, false /* silent */, true /* save */);

    registerCommandConfiguration(newAction->getConfigurationPage());
  }
  delete a;
}


Action* ManageActionsDialog::getCurrentlySelectedAction()
{
  QModelIndex i = ui.lvPlugins->currentIndex();
  if (!i.isValid()) return 0;

  return static_cast<Action*>(i.internalPointer());
}


void ManageActionsDialog::remove()
{
  Action *a = getCurrentlySelectedAction();
  if (!a) {
    KMessageBox::information(this, i18n("Please select an action"));
    return;
  }

  if (KMessageBox::questionYesNo(this, i18n("Do you really want to remove this action from this scenario?\n\nWARNING: All associated commands and configuration will be irreversibly deleted!")) == KMessageBox::Yes) {
    CommandConfiguration *c = a->getConfigurationPage();
    pageWidget->removePage(pages.value(c));
    pages.remove(c);

    ScenarioManager::getInstance()->getCurrentScenario()->actionCollection()->deleteAction(a);

  }
}


void ManageActionsDialog::moveUp()
{
  Action *a = getCurrentlySelectedAction();
  if (!a) KMessageBox::information(this, i18n("Please select an action"));

  if (ScenarioManager::getInstance()->getCurrentScenario()->actionCollection()->moveActionUp(a)) {
    int rowToSelect = ui.lvPlugins->currentIndex().row()-1;
    ui.lvPlugins->setCurrentIndex(ui.lvPlugins->model()->index(rowToSelect,0));
  }
}


void ManageActionsDialog::moveDown()
{
  Action *a = getCurrentlySelectedAction();
  if (!a) KMessageBox::information(this, i18n("Please select an action"));

  if (ScenarioManager::getInstance()->getCurrentScenario()->actionCollection()->moveActionDown(a)) {
    int rowToSelect = ui.lvPlugins->currentIndex().row()+1;
    ui.lvPlugins->setCurrentIndex(ui.lvPlugins->model()->index(rowToSelect,0));
  }
}


ManageActionsDialog::~ManageActionsDialog()
{
  QList<KPageWidgetItem*> items = pages.values();
  foreach (KPageWidgetItem *i, items) {
    pageWidget->removePage(i);
  }
  pages.clear();
  delete manageActionsAutorunWidget;
}
