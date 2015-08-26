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

#include "addactiondialog.h"

#include <simonscenarios/actioncollection.h>
#include <simonscenarios/action.h>
#include <simonscenarios/actionmodel.h>
#include <simonscenarios/scenariomanager.h>
#include <simonscenarios/scenario.h>
#include <QSortFilterProxyModel>
#include <KServiceTypeTrader>

AddActionDialog::AddActionDialog(QWidget* parent) : KDialog(parent), proxyModel(new QSortFilterProxyModel(this)),
actionModel(new ActionModel(this))
{
  setWindowTitle( i18n("Add Action") );

  QWidget *baseWidget = new QWidget( this );
  ui.setupUi(baseWidget);
//PORTING: Verify that widget was added to mainLayout: //PORTING: Verify that widget was added to mainLayout: //PORTING: Verify that widget was added to mainLayout:   setMainWidget( baseWidget );
// Add mainLayout->addWidget(baseWidget); if necessary
// Add mainLayout->addWidget(baseWidget); if necessary
// Add mainLayout->addWidget(baseWidget); if necessary

  proxyModel->setSourceModel(actionModel);
  ui.lvActions->setModel(proxyModel);
  ui.lvActions->setIconSize(QSize(24,24));
  ui.lvActions->setSpacing(2);

  proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
  connect(ui.leFilter, SIGNAL(textChanged(QString)), proxyModel, SLOT(setFilterRegExp(QString)));
  enableButton(KDialog::Ok,false);

  connect(ui.lvActions, SIGNAL(clicked(QModelIndex)), this, SLOT(watchButtonOk()));
  connect(ui.leFilter, SIGNAL(textChanged(QString)),  this, SLOT(watchButtonOk()));
}


void AddActionDialog::watchButtonOk()
{
  QModelIndex index = ui.lvActions->currentIndex();
  enableButton(KDialog::Ok,index.isValid());
}


int AddActionDialog::exec()
{
  KService::List services;
  KServiceTypeTrader* trader = KServiceTypeTrader::self();

  services = trader->query("simon/CommandPlugin");

  foreach (KService::Ptr service, services) {
    Action* action = new Action(0 /* no parent scenario */, service->storageId(), QString(),
      ScenarioManager::getInstance()->getCurrentScenario()->actionCollection());
    actionModel->appendAction(action, false);
  }

  return QDialog::exec();
}


Action* AddActionDialog::getAction()
{
  QModelIndex index = proxyModel->mapToSource(ui.lvActions->currentIndex());

  if (!index.isValid()) return 0;

  Action *a = static_cast<Action*>(index.internalPointer());

  //if we cannot remove the action from the model, it will get deleted
  //soon afterwards so better throw and error here
  if (!a || !actionModel->takeAction(a))
    return 0;

  actionModel->clearActions();
  return a;
}


AddActionDialog::~AddActionDialog()
{
  actionModel->deleteLater();
}
