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


#include "scenariomanagementdialog.h"
#include "newscenario.h"
#include <QWidget>
#include <QListWidget>
#include <QVariant>
#include <QDateTime>
#include <QFileInfo>
#include <QListWidgetItem>
#include <KSharedConfig>
#include <KConfigGroup>
#include <KDebug>
#include <KMessageBox>
#include <KStandardDirs>
#include <simonscenarios/author.h>
#include <simonscenariobase/versionnumber.h>
#include <simonscenarios/scenario.h>
#include <simonscenarios/scenariomanager.h>


ScenarioManagementDialog::ScenarioManagementDialog(QWidget *parent) : KDialog(parent)
{
	QWidget *widget = new QWidget( this );
	ui.setupUi(widget);

	setMainWidget( widget );
	setCaption( i18n("Manage scenarios") );

	initDisplay();

	connect(ui.asScenarios->availableListWidget(), SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)),
			  this, SLOT(availableScenarioSelected()));
	connect(ui.asScenarios->selectedListWidget(), SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)),
			  this, SLOT(selectedScenarioSelected()));

	connect(ui.asScenarios->availableListWidget(), SIGNAL(clicked(const QModelIndex&)),
			  this, SLOT(updateLastSelectedIndex(const QModelIndex&)));
	connect(ui.asScenarios->selectedListWidget(), SIGNAL(clicked(const QModelIndex&)),
			  this, SLOT(updateLastSelectedIndex(const QModelIndex&)));

	connect(ui.pbCreateScenario, SIGNAL(clicked()), this, SLOT(newScenario()));
	connect(ui.pbGetNewScenarios, SIGNAL(clicked()), this, SLOT(getNewScenarios()));
	connect(ui.pbImportScenario, SIGNAL(clicked()), this, SLOT(importScenario()));
	connect(ui.pbExportScenario, SIGNAL(clicked()), this, SLOT(exportScenario()));
	connect(ui.pbEditScenario, SIGNAL(clicked()), this, SLOT(editScenario()));
	connect(ui.pbDeleteScenario, SIGNAL(clicked()), this, SLOT(deleteScenario()));
}

void ScenarioManagementDialog::newScenario()
{
	NewScenario *newScenario = new NewScenario(this);
	if (newScenario->newScenario())
		initDisplay();
	delete newScenario;
}

void ScenarioManagementDialog::editScenario()
{
	Scenario *s = getCurrentlySelectedScenario();
	if (!s) return;

	NewScenario *newScenario = new NewScenario(this);
	if (newScenario->editScenario(s))
		initDisplay();

	delete newScenario;
	delete s;
}

void ScenarioManagementDialog::importScenario()
{

}

void ScenarioManagementDialog::exportScenario()
{

}

void ScenarioManagementDialog::getNewScenarios()
{

}

Scenario* ScenarioManagementDialog::getCurrentlySelectedScenario()
{
	if (!m_lastSelectedIndex.isValid())  {
		KMessageBox::information(this, i18n("Please select a scenario to delete"));
		return NULL;
	}

	QString scenarioSource = m_lastSelectedIndex.data(Qt::UserRole).toString();
	Scenario *s = new Scenario(scenarioSource);
	if (!s || !s->skim()) {
		KMessageBox::sorry(this, i18n("Could not find scenario \"%1\"", scenarioSource));
		delete s;
		return NULL;
	}

	return s;
}

void ScenarioManagementDialog::deleteScenario()
{
	Scenario *s = getCurrentlySelectedScenario();
	if (!s) return;

	if (KMessageBox::questionYesNoCancel(this, i18n("Do you really want to irrecoverably delete the selected scenario \"%1\" (\"%2\")?", 
					s->name(), s->id())) == KMessageBox::Yes) {
		QString path = KStandardDirs::locate("appdata", "scenarios/"+s->id());
		QListWidget *available = ui.asScenarios->availableListWidget();
		QListWidget *selected = ui.asScenarios->selectedListWidget();
		bool wasSelected = selected->currentIndex() == m_lastSelectedIndex;

		if (!QFile::remove(path)) {
			KMessageBox::information(this, i18n("Could not remove scenario at the following path:\n%1\n\nIf this is a system scenario, a normal user can not remove it. Please remove the file manually.\n\nIn the mean time, simon has automatically deactivated the scenario if it wasn't already.", path));
			//remove it from selected if needed
			if (wasSelected) {
				//scenario was selected
				//move to available
				available->addItem(selected->takeItem(m_lastSelectedIndex.row()));
			}
		} else {
			//worked out ok
			QListWidgetItem *scenarioItem = NULL;
			if (wasSelected)
				scenarioItem = selected->takeItem(m_lastSelectedIndex.row());
			else scenarioItem = available->takeItem(m_lastSelectedIndex.row());

			delete scenarioItem;
		}
	}

	delete s;
	ui.asScenarios->setButtonsEnabled();
}

void ScenarioManagementDialog::updateLastSelectedIndex(const QModelIndex& index)
{
	kDebug() << "Updating index";
	m_lastSelectedIndex = index;
}

void ScenarioManagementDialog::initDisplay()
{
	QListWidget *available = ui.asScenarios->availableListWidget();
	QListWidget *selected = ui.asScenarios->selectedListWidget();
	available->clear();
	selected->clear();

	KSharedConfigPtr config = KSharedConfig::openConfig("simonscenariosrc");
	KConfigGroup cg(config, "");
	QStringList  selectedIds = cg.readEntry("SelectedScenarios", QStringList());


	QStringList scenarioIds = ScenarioManager::getInstance()->getAllAvailableScenarioIds();
	kDebug() << "Found scenarios: " << scenarioIds;

	foreach (const QString& id, scenarioIds) {
		Scenario *s = new Scenario(id);
		kDebug() << "Initializing scenario" << id;
		if (!s->skim()) {
			KMessageBox::information(this, i18n("Could not init scenario \"%1\"", id));
		} else {
			QListWidget *target;
			if (selectedIds.contains(id))
				target = selected;
			else target = available;

			QListWidgetItem *item = new QListWidgetItem(s->icon(), s->name(), target);

			QString tooltip;
			QString licence = s->licence();
			QString minVersion = s->simonMinVersion()->toString();

			QString maxVersion;
			if (s->simonMaxVersion())
				maxVersion = s->simonMaxVersion()->toString();
			else
				maxVersion = "-";

			QString strAuthors;
			QList<Author*>  authors = s->authors();
			foreach (Author* a, authors)
				strAuthors += i18nc("Name and contact information", "<p>%1 (%2)</p>", a->name(), a->contact());

			tooltip = i18nc("Infos about the scenario", "<html><head /><body>"
					"<h4>Licence</h4><p>%1</p>"
					"<h4>Compatibility</h4><p>Minimum version: %2</p><p>Maximum version: %3</p>"
					"<h4>Authors</h4><p>%4</p>", licence, minVersion, maxVersion, strAuthors);

			item->setToolTip(tooltip);
			item->setData(Qt::UserRole, id);
		}
	}
	ui.asScenarios->setButtonsEnabled();
}

void ScenarioManagementDialog::availableScenarioSelected()
{
	ui.asScenarios->setButtonsEnabled();
}

void ScenarioManagementDialog::selectedScenarioSelected()
{
	ui.asScenarios->setButtonsEnabled();
}


int ScenarioManagementDialog::exec()
{
	int ret = KDialog::exec();
	if (ret) {
		QListWidget *s = ui.asScenarios->selectedListWidget();
		QStringList ids;

		for (int i=0; i < s->count(); i++)
			ids << s->item(i)->data(Qt::UserRole).toString();

		KSharedConfigPtr config = KSharedConfig::openConfig("simonscenariosrc");
		KConfigGroup cg(config, "");
		cg.writeEntry("SelectedScenarios", ids);
		cg.writeEntry("LastModified", QDateTime::currentDateTime());
	}

	return ret;
}


ScenarioManagementDialog::~ScenarioManagementDialog()
{
}

