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
#include <QTreeWidget>
#include <QSize>
#include <QVariant>
#include <QMenu>
#include <QFileInfo>
#include <KDateTime>
#include <QTreeWidgetItem>
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

  connect(ui->twAvailable, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
    this, SLOT(availableScenarioSelected()));
  connect(ui->twSelected, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)),
    this, SLOT(selectedScenarioSelected()));

  connect(ui->twAvailable, SIGNAL(clicked(const QModelIndex&)),
    this, SLOT(updateLastSelectedIndex(const QModelIndex&)));
  connect(ui->twSelected, SIGNAL(clicked(const QModelIndex&)),
    this, SLOT(updateLastSelectedIndex(const QModelIndex&)));

  connect(ui->pbAddToSelected, SIGNAL(clicked()), this, SLOT(slotAdded()));
  connect(ui->pbMoveUp, SIGNAL(clicked()), this, SLOT(slotMovedUp()));
  connect(ui->pbMoveDown, SIGNAL(clicked()), this, SLOT(slotMovedDown()));
  connect(ui->pbRemoveFromSelected, SIGNAL(clicked()), this, SLOT(slotRemoved()));

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
  ui->pbAddToSelected->setIcon(KIcon("arrow-right"));
  ui->pbRemoveFromSelected->setIcon(KIcon("arrow-left"));
  ui->pbMoveUp->setIcon(KIcon("arrow-up"));
  ui->pbMoveDown->setIcon(KIcon("arrow-down"));
  ui->pbAddToSelected->setIconSize(QSize(22, 22));
  ui->pbRemoveFromSelected->setIconSize(QSize(22, 22));
  ui->pbMoveUp->setIconSize(QSize(22, 22));
  ui->pbMoveDown->setIconSize(QSize(22, 22));

  ui->pbExportScenario->setMenu(exportMenu);
  ui->pbImportScenario->setMenu(importMenu);
}


void ScenarioManagementDialog::newScenario()
{
  NewScenario *newScenario = new NewScenario(this);
  Scenario *s = newScenario->newScenario();
  if (s) {
    //add scenario to available
    displayScenario(s, ui->twAvailable);
    m_dirty = true;
  }
  delete newScenario;
  s->deleteLater();
}


void ScenarioManagementDialog::editScenario()
{
  Scenario *s = getCurrentlySelectedScenario();
  if (!s) return;

  NewScenario *newScenario = new NewScenario(this);
  s = newScenario->editScenario(s);

  if (s) {
    //update description
    QTreeWidget *available = ui->twAvailable;
    QTreeWidget *selected = ui->twSelected;

    QTreeWidgetItem *itemToUpdate = 0;
    if (selected->currentIndex() == m_lastSelectedIndex)
      itemToUpdate = selected->currentItem();
    else
      itemToUpdate = available->currentItem();

    setupItemToScenario(itemToUpdate, s);
    m_dirty = true;
  }

  delete newScenario;
  s->deleteLater();
}


void ScenarioManagementDialog::importScenario()
{
  QString path = KFileDialog::getOpenFileName(KUrl(), QString(), this, i18n("Select scenario file"));
  if (path.isEmpty()) return;
  Scenario *s = new Scenario("");
  kDebug() << "Path: " << path;
  if (!s->init(path)) {
    KMessageBox::sorry(this, i18n("Could not load scenario."));
    s->deleteLater();
    return;
  }

  if (!s->save()) {
    kDebug() << "Hier bin ich!";
    KMessageBox::sorry(this, i18n("Failed to store scenario"));
    s->deleteLater();
    return;
  }
  displayScenario(s, ui->twAvailable);
  s->deleteLater();
}


void ScenarioManagementDialog::exportScenarioGHNS()
{
  Scenario *s = getCurrentlySelectedScenario();
  if (!s) return;

  QString path = KStandardDirs::locate("data", m_dataPrefix+"scenarios/"+s->id());
  QPointer<KNS3::UploadDialog> dialog = new KNS3::UploadDialog(KStandardDirs::locate("config", "simonscenarios.knsrc"));
  dialog->setUploadFile(path);
  dialog->setUploadName(s->name());
  dialog->exec();
  delete dialog;
}


void ScenarioManagementDialog::exportScenarioFile()
{
  Scenario *s = getCurrentlySelectedScenario();
  if (!s) return;

  if (!s->init()) {
    KMessageBox::sorry(this, i18n("Could not load scenario."));
    s->deleteLater();
    return;
  }

  QString path = KFileDialog::getSaveFileName(KUrl(), QString(), this, i18n("Select scenario output file"));
  if (path.isEmpty()) return;

  if (!s->save(path)) {
    KMessageBox::sorry(this, i18n("Failed to store scenario"));
  }
  s->deleteLater();
}


bool ScenarioManagementDialog::getNewScenarios()
{
  if (m_dirty && (KMessageBox::questionYesNoCancel(this, i18n("Downloading new scenarios requires you to save your current "
    "changes before continuing.\n\nSave your changes now?")) != KMessageBox::Yes))
    return false;

  if (!save()) return false;

  QPointer<KNS3::DownloadDialog> dialog = new KNS3::DownloadDialog(KStandardDirs::locate("config", "simonscenarios.knsrc"));
  dialog->exec();

  if (!dialog) return false;

  KSharedConfigPtr config = KSharedConfig::openConfig("simonscenariosrc");
  KConfigGroup cg(config, "");

  QStringList selectedIds = cg.readEntry("SelectedScenarios", QStringList() << "general");

  foreach (const KNS3::Entry& e, dialog->changedEntries()) {
    if (e.status() == KNS3::Entry::Installed) {
      QStringList installedFiles = e.installedFiles();
      foreach (const QString& file, installedFiles) {
        QFileInfo fi(file);
        selectedIds.append(fi.fileName());
      }
    }
    if (e.status() == KNS3::Entry::Deleted) {
      QStringList uninstalledFiles = e.uninstalledFiles();
      foreach (const QString& file, uninstalledFiles) {
        QFileInfo fi(file);
        selectedIds.removeAll(fi.fileName());
      }
    }
  }
  cg.writeEntry("SelectedScenarios", selectedIds);
  cg.writeEntry("LastModified", KDateTime::currentUtcDateTime().dateTime());
  cg.sync();
  initDisplay();
  delete dialog;

  return true;
}


Scenario* ScenarioManagementDialog::getCurrentlySelectedScenario()
{
  if (!m_lastSelectedIndex.isValid()) {
    KMessageBox::information(this, i18n("Please select a scenario from the list(s)"));
    return 0;
  }

  QString scenarioSource = m_lastSelectedIndex.data(Qt::UserRole).toString();
  Scenario *s = new Scenario(scenarioSource);
  if (!s || !s->skim()) {
    KMessageBox::sorry(this, i18n("Could not find scenario \"%1\"", scenarioSource));
    s->deleteLater();
    return 0;
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
    QTreeWidget *available = ui->twAvailable;
    QTreeWidget *selected = ui->twSelected;
    bool wasSelected = selected->currentIndex() == m_lastSelectedIndex;

    if (!QFile::remove(path)) {
      KMessageBox::information(this, i18n("Could not remove scenario at the following path:\n%1\n\nIf this is a system scenario, a normal user can not remove it. Please remove the file manually.\n\nIn the mean time, simon has automatically deactivated the scenario if it was not already.", path));
      //remove it from selected if needed
      if (wasSelected) {
        //scenario was selected
        //move to available
        available->addTopLevelItem(selected->takeTopLevelItem(m_lastSelectedIndex.row()));
      }
    }
    else {
      //worked out ok
      QTreeWidgetItem *scenarioItem = 0;
      if (wasSelected)
        scenarioItem = selected->takeTopLevelItem(m_lastSelectedIndex.row());
      else scenarioItem = available->takeTopLevelItem(m_lastSelectedIndex.row());

      KSharedConfigPtr config = KSharedConfig::openConfig("simonscenariosrc");
      KConfigGroup cg(config, "");

      QStringList deletedScenarios = cg.readEntry("DeletedScenarios", QStringList());
      QStringList deletedScenariosTimes = cg.readEntry("DeletedScenariosTimes", QStringList());

      deletedScenarios << s->id();
      deletedScenariosTimes << KDateTime::currentUtcDateTime().dateTime().toString("yyyy-MM-dd-hh-mm-ss");

      cg.writeEntry("DeletedScenarios", deletedScenarios);
      cg.writeEntry("DeletedScenariosTimes", deletedScenariosTimes);
      cg.sync();

      delete scenarioItem;
    }
    m_dirty = true;
  }

  s->deleteLater();
  //ui->asScenarios->setButtonsEnabled();
}


void ScenarioManagementDialog::updateLastSelectedIndex(const QModelIndex& index)
{
    kDebug() << "Updating index";
    m_lastSelectedIndex = index;
}


void ScenarioManagementDialog::slotAdded()
{
    if (!ui->twAvailable->currentItem())
        return;
    if (ui->twAvailable->invisibleRootItem()->indexOfChild(ui->twAvailable->currentItem()) < 0)
        return;

    ui->twSelected->addTopLevelItem(ui->twAvailable->takeTopLevelItem(ui->twAvailable->currentIndex().row()));

    updateLastSelectedIndex(ui->twSelected->currentIndex());
    m_dirty = true;
}


void ScenarioManagementDialog::slotMovedDown()
{
    if (!ui->twSelected->currentItem())
        return;
    if (ui->twSelected->invisibleRootItem()->indexOfChild(ui->twSelected->currentItem()) < 0)
        return;
    if (ui->twSelected->invisibleRootItem()->indexOfChild(ui->twSelected->currentItem()) >= ui->twSelected->invisibleRootItem()->childCount()-1)
        return;

    int newRow = ui->twSelected->currentIndex().row()+1;
    QTreeWidgetItem* item =  ui->twSelected->takeTopLevelItem(newRow-1);
    ui->twSelected->insertTopLevelItem(newRow, item);

    ui->twSelected->setCurrentItem(item, 0);

    updateLastSelectedIndex(ui->twSelected->currentIndex());
    m_dirty = true;
}


void ScenarioManagementDialog::slotMovedUp()
{
    if (!ui->twSelected->currentItem())
        return;
    if (ui->twSelected->invisibleRootItem()->indexOfChild(ui->twSelected->currentItem()) < 1)
        return;

    int newRow = ui->twSelected->currentIndex().row()-1;
    QTreeWidgetItem* item =  ui->twSelected->takeTopLevelItem(newRow+1);
    ui->twSelected->insertTopLevelItem(newRow, item);

    ui->twSelected->setCurrentItem(item, 0);

    updateLastSelectedIndex(ui->twSelected->currentIndex());
    m_dirty = true;
}


void ScenarioManagementDialog::slotRemoved()
{
    if (!ui->twSelected->currentItem())
        return;
    if (ui->twSelected->invisibleRootItem()->indexOfChild(ui->twSelected->currentItem()) < 0)
        return;

    ui->twAvailable->addTopLevelItem(ui->twSelected->takeTopLevelItem(ui->twSelected->currentIndex().row()));

    updateLastSelectedIndex(ui->twAvailable->currentIndex());
    m_dirty = true;
}


void ScenarioManagementDialog::displayScenario(Scenario *scenario, QTreeWidget* widget)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(widget);

    setupItemToScenario(item, scenario);
}


void ScenarioManagementDialog::setupItemToScenario(QTreeWidgetItem *item, Scenario *s)
{
  item->setIcon(0, s->icon());
  item->setText(0, s->name());
  QString tooltip;
  QString license = s->license();
  QString minVersion = s->simonMinVersion()->toString();

  QString maxVersion;
  if (s->simonMaxVersion())
    maxVersion = s->simonMaxVersion()->toString();
  else
    maxVersion = '-';

  QString strAuthors;
  QList<Author*>  authors = s->authors();
  foreach (Author* a, authors)
    strAuthors += i18nc("Name and contact information", "<p>%1 (%2)</p>", a->name(), a->contact());

  tooltip = i18nc("Information about the scenario; 1: name, 2: version, 3: license, "
		  "4: minimum version, 5: maximum version, 6: authors", "<html><head /><body>"
    "<h3>%1</h3>"
    "<h4>Version</h4><p>%2</p>"
    "<h4>License</h4><p>%3</p>"
    "<h4>Compatibility</h4><p>Minimum version: %4</p><p>Maximum version: %5</p>"
    "<h4>Authors</h4><p>%6</p></body></html>", s->name(), s->version(), license, minVersion, maxVersion, strAuthors);

  item->setToolTip(0, tooltip);
  item->setData(0, Qt::UserRole, s->id());
}


void ScenarioManagementDialog::initDisplay()
{
  QTreeWidget *available = ui->twAvailable;
  QTreeWidget *selected = ui->twSelected;
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
    }
    else {
      if (selectedIds.contains(id))
      {
        selectedList.insert(id, s);
      }
      else
      {
        displayScenario(s, available);
        s->deleteLater();
      }
    }
  }

  foreach (const QString& id, selectedIds) {
    Scenario *s = selectedList.value(id);
    if (!s) continue;

    displayScenario(s, selected);
    s->deleteLater();
  }

  m_dirty = false;
}


void ScenarioManagementDialog::availableScenarioSelected()
{
  //ui->asScenarios->setButtonsEnabled();
}


void ScenarioManagementDialog::selectedScenarioSelected()
{
  //ui->asScenarios->setButtonsEnabled();
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
  cg.writeEntry("LastModified", KDateTime::currentUtcDateTime().dateTime());
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

  QTreeWidget *s = ui->twSelected;
  for (int i=0; i < s->topLevelItemCount(); i++)
    ids << s->topLevelItem(i)->data(0, Qt::UserRole).toString();

  return ids;
}


bool ScenarioManagementDialog::updateScenarioConfiguration()
{
  int ret;
  do {
    ret = exec();
    if (!ret) break;
  } while (!save());
  return ret;

}


ScenarioManagementDialog::~ScenarioManagementDialog()
{
  delete ui;
}
