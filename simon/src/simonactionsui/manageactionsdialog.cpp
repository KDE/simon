/*
 *   Copyright (C) 2009 Peter Grasch <grasch@simon-listens.org>
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
#include "addactiondialog.h"

#include <simonscenarios/scenariomanager.h>
#include <simonscenarios/scenario.h>
#include <simonscenarios/actioncollection.h>
#include <KMessageBox>

ManageActionsDialog::ManageActionsDialog(QWidget* parent) : KDialog(parent),
	pageWidget(new KPageWidget(this))
{	
	setCaption( i18n("Manage actions") );
	setMainWidget( pageWidget );

	QWidget *baseWidget = new QWidget( this );
	ui.setupUi(baseWidget);


	KPageWidgetItem *generalItem = pageWidget->addPage(baseWidget, i18n("General"));
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

	ui.lvPlugins->setIconSize(QSize(24,24));
	ui.lvPlugins->setSpacing(2);
}

int ManageActionsDialog::exec()
{
	ActionCollection *aC = ScenarioManager::getInstance()->getCurrentScenario()->actionCollection();
	ui.lvPlugins->setModel(aC);

	return KDialog::exec();

}

void ManageActionsDialog::add()
{
	AddActionDialog *a = new AddActionDialog(this);
	if (a->exec()) {
		Action *newAction = a->getAction();

		Q_ASSERT(newAction);

		ScenarioManager::getInstance()->getCurrentScenario()->actionCollection()->addAction(newAction);
	}
	a->deleteLater();
}

Action* ManageActionsDialog::getCurrentlySelectedAction()
{
	QModelIndex i = ui.lvPlugins->currentIndex();
	if (!i.isValid()) return NULL;

	return static_cast<Action*>(i.internalPointer());
}

void ManageActionsDialog::remove()
{
	Action *a = getCurrentlySelectedAction();
	if (!a) KMessageBox::information(this, i18n("Please select an action"));

	if (KMessageBox::questionYesNo(this, i18n("Do you really want to remove this action from this scenario?\n\nWARNING: All associated commands and configuration will be irreversibly deleted!")) == KMessageBox::Yes)
		ScenarioManager::getInstance()->getCurrentScenario()->actionCollection()->deleteAction(a);
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
}

