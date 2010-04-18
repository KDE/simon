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
#include <simonscenarios/author.h>
#include <simonscenariobase/versionnumber.h>
#include <simonscenarios/scenario.h>
#include <simonscenarios/scenariomanager.h>
#include <QWidget>
#include <QListWidget>
#include <QSize>
#include <QVariant>
#include <QMenu>
#include <QFileInfo>
#include <QDateTime>
#include <QFileInfo>
#include <QListWidgetItem>
#include <KSharedConfig>
#include <KConfigGroup>
#include <KDebug>
#include <KGlobal>
#include <KMessageBox>
#include <KStandardDirs>
#include <KFileDialog>

 
#include <knewstuff3/downloaddialog.h>
#include <knewstuff3/uploaddialog.h>

#include "ui_scenariomanagementdlg.h"

ScenarioManagementDialog::ScenarioManagementDialog(const QString& dataPrefix, QWidget *parent) : KDialog(parent), 
	ui(new Ui::ScenarioManagementDialog()), m_dataPrefix(dataPrefix), m_dirty(false)
{
	QWidget *widget = new QWidget( this );
	ui->setupUi(widget);

	setMainWidget( widget );
	setCaption( i18n("Manage scenarios") );

	initDisplay();

	connect(ui->asScenarios->availableListWidget(), SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)),
			  this, SLOT(availableScenarioSelected()));
	connect(ui->asScenarios->selectedListWidget(), SIGNAL(currentItemChanged(QListWidgetItem*, QListWidgetItem*)),
			  this, SLOT(selectedScenarioSelected()));

	connect(ui->asScenarios->availableListWidget(), SIGNAL(clicked(const QModelIndex&)),
			  this, SLOT(updateLastSelectedIndex(const QModelIndex&)));
	connect(ui->asScenarios->selectedListWidget(), SIGNAL(clicked(const QModelIndex&)),
			  this, SLOT(updateLastSelectedIndex(const QModelIndex&)));

	ui->asScenarios->selectedListWidget()->setIconSize(QSize(22,22));
	ui->asScenarios->availableListWidget()->setIconSize(QSize(22,22));

	connect(ui->asScenarios, SIGNAL(added(QListWidgetItem*)), this, SLOT(slotAdded(QListWidgetItem*)));
	connect(ui->asScenarios, SIGNAL(movedUp(QListWidgetItem*)), this, SLOT(slotMovedUp(QListWidgetItem*)));
	connect(ui->asScenarios, SIGNAL(movedDown(QListWidgetItem*)), this, SLOT(slotMovedDown(QListWidgetItem*)));
	connect(ui->asScenarios, SIGNAL(removed(QListWidgetItem*)), this, SLOT(slotRemoved(QListWidgetItem*)));

	connect(ui->pbCreateScenario, SIGNAL(clicked()), this, SLOT(newScenario()));
	connect(ui->pbEditScenario, SIGNAL(clicked()), this, SLOT(editScenario()));
	connect(ui->pbDeleteScenario, SIGNAL(clicked()), this, SLOT(deleteScenario()));

	QMenu *exportMenu = new QMenu(this);
	QAction *ghnsExport = exportMenu->addAction(KIcon("get-hot-new-stuff"), i18n("Publish"));
	QAction *fileExport = exportMenu->addAction(KIcon("document-export"), i18n("Export to file"));

	QMenu *importMenu = new QMenu(this);
	QAction *ghnsImport = importMenu->addAction(KIcon("get-hot-new-stuff"), i18n("Download"));
	QAction *fileImport = importMenu->addAction(KIcon("document-import"), i18n("Import from file"));

	connect(ghnsExport, SIGNAL(triggered()), this, SLOT(exportScenarioGHNS()));
	connect(fileExport, SIGNAL(triggered()), this, SLOT(exportScenarioFile()));

	connect(ghnsImport, SIGNAL(triggered()), this, SLOT(getNewScenarios()));
	connect(fileImport, SIGNAL(triggered()), this, SLOT(importScenario()));

	ui->pbCreateScenario->setIcon(KIcon("list-add"));
	ui->pbImportScenario->setIcon(KIcon("document-import"));
	ui->pbExportScenario->setIcon(KIcon("document-export"));
	ui->pbEditScenario->setIcon(KIcon("document-edit"));
	ui->pbDeleteScenario->setIcon(KIcon("list-remove"));

	ui->pbExportScenario->setMenu(exportMenu);
	ui->pbImportScenario->setMenu(importMenu);
}

void ScenarioManagementDialog::newScenario()
{
	NewScenario *newScenario = new NewScenario(this);
	Scenario *s = newScenario->newScenario();
	if (s) {
		//add scenario to available
		displayScenario(s, ui->asScenarios->availableListWidget());
		m_dirty = true;
	}
	delete newScenario;
	delete s;
}

void ScenarioManagementDialog::editScenario()
{
	Scenario *s = getCurrentlySelectedScenario();
	if (!s) return;

	NewScenario *newScenario = new NewScenario(this);
	s = newScenario->editScenario(s);

	if (s) {
		//update description
		QListWidget *available = ui->asScenarios->availableListWidget();
		QListWidget *selected = ui->asScenarios->selectedListWidget();

		QListWidgetItem *itemToUpdate = NULL;
		if (selected->currentIndex() == m_lastSelectedIndex)
			itemToUpdate = selected->currentItem();
		else
			itemToUpdate = available->currentItem();

		setupItemToScenario(itemToUpdate, s);
		m_dirty = true;
	}

	delete newScenario;
	delete s;
}

void ScenarioManagementDialog::importScenario()
{
	QString path = KFileDialog::getOpenFileName(KUrl(), QString(), this, i18n("Select scenario file"));
	if (path.isEmpty()) return;
	Scenario *s = new Scenario("");
	kDebug() << "Path: " << path;
	if (!s->init(path)) {
		KMessageBox::sorry(this, i18n("Could not load scenario."));
		delete s;
		return;
	}

	if (!s->save()) {
		kDebug() << "Hier bin ich!";
		KMessageBox::sorry(this, i18n("Failed to store scenario"));
		delete s;
		return;
	}
	displayScenario(s, ui->asScenarios->availableListWidget());
	delete s;
}

void ScenarioManagementDialog::exportScenarioGHNS()
{
	Scenario *s = getCurrentlySelectedScenario();
	if (!s) return;

	QString path = KStandardDirs::locate("data", m_dataPrefix+"scenarios/"+s->id());
	KNS3::UploadDialog dialog(KStandardDirs::locate("config", "simonscenarios.knsrc"));
	dialog.setUploadFile(path);
	dialog.setUploadName(s->name());
	dialog.exec();
}

void ScenarioManagementDialog::exportScenarioFile()
{
	Scenario *s = getCurrentlySelectedScenario();
	if (!s) return;


	if (!s->init()) {
		KMessageBox::sorry(this, i18n("Could not load scenario."));
		delete s;
		return;
	}

	QString path = KFileDialog::getSaveFileName(KUrl(), QString(), this, i18n("Select scenario output file"));
	if (path.isEmpty()) return;

	if (!s->save(path)) {
		KMessageBox::sorry(this, i18n("Failed to store scenario"));
	}
	delete s;
}

bool ScenarioManagementDialog::getNewScenarios()
{
	if (m_dirty && (KMessageBox::questionYesNoCancel(this, i18n("Downloading new scenarios requires you to save your current "
					"changes before continuing.\n\nSave your changes now?")) != KMessageBox::Yes))
		return false;

	if (!save()) return false;

	KNS3::DownloadDialog dialog(KStandardDirs::locate("config", "simonscenarios.knsrc"));
	dialog.exec();

	KSharedConfigPtr config = KSharedConfig::openConfig("simonscenariosrc");
	KConfigGroup cg(config, "");
	
	QStringList selectedIds = cg.readEntry("SelectedScenarios", QStringList() << "general");

	foreach (const KNS3::Entry& e, dialog.changedEntries()) {
		if (e.status() == KNS3::Entry::Installed)
		{
			QStringList installedFiles = e.installedFiles();
			foreach (const QString& file, installedFiles)
			{
				QFileInfo fi(file);
				selectedIds.append(fi.fileName());
			}
		}
		if (e.status() == KNS3::Entry::Deleted)
		{
			QStringList uninstalledFiles = e.uninstalledFiles();
			foreach (const QString& file, uninstalledFiles)
			{
				QFileInfo fi(file);
				selectedIds.removeAll(fi.fileName());
			}
		}
	}
	cg.writeEntry("SelectedScenarios", selectedIds);
	cg.writeEntry("LastModified", QDateTime::currentDateTime());
	cg.sync();
	initDisplay();
	return true;
}

Scenario* ScenarioManagementDialog::getCurrentlySelectedScenario()
{
	if (!m_lastSelectedIndex.isValid())  {
		KMessageBox::information(this, i18n("Please select a scenario from the list(s)"));
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
		QString path = KStandardDirs::locate("data", m_dataPrefix+"scenarios/"+s->id());
		QListWidget *available = ui->asScenarios->availableListWidget();
		QListWidget *selected = ui->asScenarios->selectedListWidget();
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
		m_dirty = true;
	}

	delete s;
	ui->asScenarios->setButtonsEnabled();
}

void ScenarioManagementDialog::updateLastSelectedIndex(const QModelIndex& index)
{
	kDebug() << "Updating index";
	m_lastSelectedIndex = index;
}

void ScenarioManagementDialog::slotAdded(QListWidgetItem*)
{
	updateLastSelectedIndex(ui->asScenarios->selectedListWidget()->currentIndex());
	m_dirty = true;
}

void ScenarioManagementDialog::slotMovedDown(QListWidgetItem*)
{
	updateLastSelectedIndex(ui->asScenarios->selectedListWidget()->currentIndex());
	m_dirty = true;
}

void ScenarioManagementDialog::slotMovedUp(QListWidgetItem*)
{
	updateLastSelectedIndex(ui->asScenarios->selectedListWidget()->currentIndex());
	m_dirty = true;
}

void ScenarioManagementDialog::slotRemoved(QListWidgetItem*)
{
	updateLastSelectedIndex(ui->asScenarios->availableListWidget()->currentIndex());
	m_dirty = true;
}



void ScenarioManagementDialog::displayScenario(Scenario *scenario, QListWidget* widget)
{
	QListWidgetItem *item = new QListWidgetItem(widget);

	setupItemToScenario(item, scenario);
}

void ScenarioManagementDialog::setupItemToScenario(QListWidgetItem *item, Scenario *s)
{
	item->setIcon(s->icon());
	item->setText(s->name());
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
			"<h3>%1</h3>"
			"<h4>Version</h4><p>%2</p>"
			"<h4>Licence</h4><p>%3</p>"
			"<h4>Compatibility</h4><p>Minimum version: %4</p><p>Maximum version: %5</p>"
			"<h4>Authors</h4><p>%6</p>", s->name(), s->version(), licence, minVersion, maxVersion, strAuthors);

	item->setToolTip(tooltip);
	item->setData(Qt::UserRole, s->id());
}

void ScenarioManagementDialog::initDisplay()
{
	QListWidget *available = ui->asScenarios->availableListWidget();
	QListWidget *selected = ui->asScenarios->selectedListWidget();
	available->clear();
	selected->clear();

	KSharedConfigPtr config = KSharedConfig::openConfig("simonscenariosrc");
	KConfigGroup cg(config, "");
	QStringList  selectedIds = cg.readEntry("SelectedScenarios", QStringList() << "general");


	QStringList scenarioIds = ScenarioManager::getInstance()->getAllAvailableScenarioIds(m_dataPrefix);
	kDebug() << "Found scenarios: " << scenarioIds;

	QHash<QString, Scenario*> selectedList;

	foreach (const QString& id, scenarioIds) {
		Scenario *s = new Scenario(id, m_dataPrefix);
		if (!s->skim()) {
			KMessageBox::information(this, i18n("Could not init scenario \"%1\"", id));
		} else {
			if (selectedIds.contains(id))
				selectedList.insert(id, s);
			else displayScenario(s, available);
		}
	}

	foreach (const QString& id, selectedIds) {
		Scenario *s = selectedList.value(id);
		if (!s) continue;

		displayScenario(s, selected);
	}
	ui->asScenarios->setButtonsEnabled();
	m_dirty = false;
}

void ScenarioManagementDialog::availableScenarioSelected()
{
	ui->asScenarios->setButtonsEnabled();
}

void ScenarioManagementDialog::selectedScenarioSelected()
{
	ui->asScenarios->setButtonsEnabled();
}

bool ScenarioManagementDialog::save()
{
	QStringList ids = getSelectedScenarioIds();

	if (ids.count() == 0) {
		KMessageBox::information(this, i18n("You need at least one active scenario.\n\nPlease load a scenario.\n\nYour current (empty) configuration was not saved."));
		return false;
	}

	KSharedConfigPtr config = KSharedConfig::openConfig("simonscenariosrc");
	KConfigGroup cg(config, "");
	cg.writeEntry("SelectedScenarios", ids);
	cg.writeEntry("LastModified", QDateTime::currentDateTime());
	cg.sync();
	m_dirty = false;
	return true;
}

void ScenarioManagementDialog::init()
{
	m_dirty = false;
}

int ScenarioManagementDialog::exec()
{
	init();
	return KDialog::exec();
}

QStringList ScenarioManagementDialog::getSelectedScenarioIds()
{
	QStringList ids;

	QListWidget *s = ui->asScenarios->selectedListWidget();
	for (int i=0; i < s->count(); i++)
		ids << s->item(i)->data(Qt::UserRole).toString();

	return ids;
}

bool ScenarioManagementDialog::updateScenarioConfiguration()
{
	int ret;
	do
	{
		ret = exec();
		if (!ret) break;
	} while (!save());
	return ret;

}

ScenarioManagementDialog::~ScenarioManagementDialog()
{
	delete ui;
}

