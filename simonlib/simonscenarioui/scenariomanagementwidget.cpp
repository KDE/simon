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

#include "scenariomanagementwidget.h"
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

ScenarioManagementWidget::ScenarioManagementWidget(const QString& dataPrefix, bool minimal, QWidget* parent) : QWidget(parent),
  ui(new Ui::ScenarioManagementDialog()), m_dataPrefix(dataPrefix), m_dirty(false)
{
  ui->setupUi(this);
  
  ui->wgAvailable->setVisible(!minimal);
  ui->lbSelectedDetailed->setVisible(!minimal);
  ui->pbExportScenario->setVisible(!minimal);
  ui->pbEditScenario->setVisible(!minimal);
  
  initDisplay();

  connect(ui->twAvailable, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
    this, SLOT(availableScenarioSelected()));
  connect(ui->twSelected, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
    this, SLOT(selectedScenarioSelected()));

  connect(ui->twAvailable, SIGNAL(clicked(QModelIndex)),
    this, SLOT(updateLastSelectedIndex(QModelIndex)));
  connect(ui->twSelected, SIGNAL(clicked(QModelIndex)),
    this, SLOT(updateLastSelectedIndex(QModelIndex)));

  connect(ui->twAvailable, SIGNAL(doubleClicked(QModelIndex)),
          ui->pbAddToSelected, SIGNAL(clicked()));
  connect(ui->twSelected, SIGNAL(doubleClicked(QModelIndex)),
          ui->pbRemoveFromSelected, SIGNAL(clicked()));

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


void ScenarioManagementWidget::newScenario()
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


void ScenarioManagementWidget::editScenario()
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


void ScenarioManagementWidget::importScenario()
{
  QString path = KFileDialog::getOpenFileName(KUrl(), QString(), this, i18n("Select scenario file"));
  if (path.isEmpty()) return;
  kDebug() << "Path: " << path;

  QStringList exploded = Scenario::explode(path);
  if (exploded.isEmpty()) {
    KMessageBox::sorry(this, i18n("Failed to expand scenario."));
    return;
  }

  foreach (const QString& e, exploded) {
    Scenario *s = new Scenario(e);

    if (!s->init()) {
      KMessageBox::sorry(this, i18n("Could not load scenario \"%1\".", e));
      s->deleteLater();
      continue;
    }
    displayScenario(s, ui->twAvailable);
    s->deleteLater();
  }
}


void ScenarioManagementWidget::exportScenarioGHNS()
{
  Scenario *s = getCurrentlySelectedScenario();
  if (!s) return;

  QString path;
  if (askExportFull(s)) {
    //we need to merge this
    path = KStandardDirs::locate("tmp", m_dataPrefix+"mergedscenario");

    if (!s->init()) {
      KMessageBox::sorry(this, i18n("Could not load scenario."));
      s->deleteLater();
      return;
    }

    if (!s->save(path, true /*full*/)) {
      KMessageBox::sorry(this, i18n("Could not write temporary scenario."));
      return;
    }
  } else
    path = KStandardDirs::locate("data", m_dataPrefix+"scenarios/"+s->id());

  QPointer<KNS3::UploadDialog> dialog = new KNS3::UploadDialog(KStandardDirs::locate("config", "simonscenarios.knsrc"));
  dialog->setUploadFile(path);
  dialog->setUploadName(s->name());
  dialog->exec();
  delete dialog;
}

bool ScenarioManagementWidget::askExportFull(Scenario* s)
{
  kDebug() << "Child scenario ids: " << s->childScenarioIds();
  if (s->childScenarioIds().isEmpty())
    return false; // if we don't have children, we always export just the one scenario
  return (KMessageBox::questionYesNo(this, i18n("The selected scenario has children.\n\nDo you want to include them in the exported scenario (the hierarchy will be preserved)?")) == KMessageBox::Yes);
}

void ScenarioManagementWidget::exportScenarioFile()
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

  if (!s->save(path, askExportFull(s))) {
    KMessageBox::sorry(this, i18n("Failed to store scenario"));
  }
  s->deleteLater();
}


bool ScenarioManagementWidget::getNewScenarios()
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
        foreach (const QString& id, Scenario::explode(file))
          selectedIds.append(id);
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


Scenario* ScenarioManagementWidget::getCurrentlySelectedScenario()
{
  if (!m_lastSelectedIndex.isValid()) {
    KMessageBox::information(this, i18n("Please select a scenario from one of the lists"));
    return 0;
  }

  QString scenarioSource = m_lastSelectedIndex.data(Qt::UserRole).toString();
  Scenario *s = new Scenario(scenarioSource);
  if (!s || !s->skim()) {
    KMessageBox::sorry(this, i18nc("%1 is scenario id", "Could not find scenario \"%1\"", scenarioSource));
    s->deleteLater();
    return 0;
  }

  return s;
}


void ScenarioManagementWidget::deleteScenario()
{
  Scenario *s = getCurrentlySelectedScenario();
  if (!s) return;

  if (KMessageBox::questionYesNoCancel(this, i18nc("%1 is scenario name, %2 is scenario id", "Do you really want to irrecoverably delete the selected scenario \"%1\" (\"%2\")?",
  s->name(), s->id())) == KMessageBox::Yes) {
    QString path = KStandardDirs::locate("data", m_dataPrefix+"scenarios/"+s->id());
    QTreeWidget *available = ui->twAvailable;
    QTreeWidget *selected = ui->twSelected;
    bool wasSelected = selected->currentIndex() == m_lastSelectedIndex;

    if (!QFile::remove(path)) {
      KMessageBox::information(this, i18nc("%1 is scenario path", "Could not remove scenario at the following path:\n%1\n\nIf this is a system scenario, a normal user cannot remove it. Please remove the file manually.\n\nIn the meantime, simon has automatically deactivated the scenario if it was not already.", path));
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
  m_lastSelectedIndex = QModelIndex();
  //ui->asScenarios->setButtonsEnabled();
}


void ScenarioManagementWidget::updateLastSelectedIndex(const QModelIndex& index)
{
    kDebug() << "Updating index";
    m_lastSelectedIndex = index;
}


void ScenarioManagementWidget::slotAdded()
{
    if (!ui->twAvailable->currentItem())
        return;
    if (ui->twAvailable->invisibleRootItem()->indexOfChild(ui->twAvailable->currentItem()) < 0)
        return;

    ui->twSelected->addTopLevelItem(ui->twAvailable->takeTopLevelItem(ui->twAvailable->currentIndex().row()));

    updateLastSelectedIndex(ui->twSelected->currentIndex());
    m_dirty = true;
}


void ScenarioManagementWidget::slotMovedDown()
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


void ScenarioManagementWidget::slotMovedUp()
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


void ScenarioManagementWidget::slotRemoved()
{
    if (!ui->twSelected->currentItem())
        return;
    if (ui->twSelected->invisibleRootItem()->indexOfChild(ui->twSelected->currentItem()) < 0)
        return;

    ui->twAvailable->addTopLevelItem(ui->twSelected->takeTopLevelItem(ui->twSelected->currentIndex().row()));

    updateLastSelectedIndex(ui->twAvailable->currentIndex());
    m_dirty = true;
}


QTreeWidgetItem* ScenarioManagementWidget::displayScenario(Scenario *scenario, QTreeWidget* widget)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(widget);

    setupItemToScenario(item, scenario);

    return item;
}


void ScenarioManagementWidget::setupItemToScenario(QTreeWidgetItem *item, Scenario *s)
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


void ScenarioManagementWidget::initDisplay()
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

  QList<Scenario*> scenarios;
  QHash<QString, QTreeWidgetItem*> itemsByScenarioIds;

  foreach (const QString& id, scenarioIds) {

    Scenario *s = new Scenario(id, m_dataPrefix);
    if (!s->skim())
    {
      KMessageBox::information(this, i18nc("%1 is scenario id", "Could not init scenario \"%1\"", id));
    }
    else
    {
        scenarios << s;
    }
  }

  //display the scenarios
  foreach (Scenario* s, scenarios)
  {
      if (selectedIds.contains(s->id()))
      {
        itemsByScenarioIds.insert(s->id(), displayScenario(s, selected));
      }
      else
      {
        itemsByScenarioIds.insert(s->id(), displayScenario(s, available));
      }
  }

  //setup the scenarios according to their parent/child hierarchy
  foreach (Scenario* s, scenarios)
  {
      QStringList ids = s->childScenarioIds();
      kDebug() << "Setting up children of " << s->id() << ": " << ids;
      foreach (const QString& id, ids)
      {
          QTreeWidgetItem *childItem = itemsByScenarioIds.value(id);
          QTreeWidgetItem *parentItem = itemsByScenarioIds.value(s->id());
          QTreeWidget *treeWidget = parentItem->treeWidget();
          treeWidget->takeTopLevelItem(treeWidget->invisibleRootItem()->indexOfChild(childItem));
          parentItem->addChild(childItem);
          itemsByScenarioIds.value(s->id())->insertChild(0, itemsByScenarioIds.value(id));
      }
      s->deleteLater();
  }

  m_dirty = false;
}


void ScenarioManagementWidget::availableScenarioSelected()
{
  //ui->asScenarios->setButtonsEnabled();
}


void ScenarioManagementWidget::selectedScenarioSelected()
{
  //ui->asScenarios->setButtonsEnabled();
}

void ScenarioManagementWidget::saveChildConfiguration(QTreeWidgetItem *parentItem)
{
    for (int i=0; i<parentItem->childCount(); i++)
    {
        //setup the item
        QTreeWidgetItem *item = parentItem->child(i);

        kDebug() << "Configuring children of: " << item->data(0, Qt::UserRole).toString();

        //save the item's child configuration
        QStringList ids = getChildScenarioIds(item);
        Scenario *s = new Scenario(item->data(0, Qt::UserRole).toString());
        if (s->init())
        {
            s->setChildScenarioIds(ids);
            s->save();
        }

        ids.clear();

        //configure item's children's children
        saveChildConfiguration(item);
    }
}

bool ScenarioManagementWidget::save()
{
    saveChildConfiguration(ui->twSelected->invisibleRootItem());
    saveChildConfiguration(ui->twAvailable->invisibleRootItem());

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


void ScenarioManagementWidget::init()
{
  m_dirty = false;
}


QStringList ScenarioManagementWidget::getSelectedScenarioIds()
{
  QStringList ids;

  QTreeWidget *s = ui->twSelected;
  ids << getAllLevelChildScenarioIds(s->invisibleRootItem());

  return ids;
}

QStringList ScenarioManagementWidget::getChildScenarioIds(QTreeWidgetItem* parentItem)
{
  QStringList ids;

  kDebug() << "Getting child ids of: " << parentItem->data(0, Qt::UserRole).toString();

  for (int i=0; i < parentItem->childCount(); i++)
  {
    ids << parentItem->child(i)->data(0, Qt::UserRole).toString();
  }

  return ids;
}

QStringList ScenarioManagementWidget::getAllLevelChildScenarioIds(QTreeWidgetItem* parentItem)
{
  QStringList ids;

  kDebug() << "Getting child ids of: " << parentItem->data(0, Qt::UserRole).toString();

  for (int i=0; i < parentItem->childCount(); i++)
  {
    ids << parentItem->child(i)->data(0, Qt::UserRole).toString();
    ids << getAllLevelChildScenarioIds(parentItem->child(i));
  }

  return ids;
}

ScenarioManagementWidget::~ScenarioManagementWidget()
{
  delete ui;
}
