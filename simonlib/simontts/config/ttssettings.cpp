/*
 *   Copyright (C) 2010 Peter Grasch <grasch@simon-listens.org>
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

#include "ttssettings.h"
#include "ttsconfiguration.h"
#include "editrecording.h"
#include <simontts/simontts.h>
#include <simontts/recordingset.h>
#include <simontts/recordingsetcollection.h>
#include <QListWidget>
#include <QSortFilterProxyModel>
#include <kgenericfactory.h>
#include <KActionSelector>
#include <KInputDialog>
#include <KMessageBox>
#include <KFileDialog>

K_PLUGIN_FACTORY( TTSSettingsFactory,
  registerPlugin< TTSSettings >();
)

K_EXPORT_PLUGIN( TTSSettingsFactory("TTSSettings") )

TTSSettings::TTSSettings(QWidget* parent, const QVariantList& args): KCModule(KGlobal::mainComponent(), parent),
  sets(0)
{ 
  Q_UNUSED(args);

  ui.setupUi(this);

  ui.pbAddRecording->setIcon(KIcon("list-add"));
  ui.pbEditRecording->setIcon(KIcon("document-edit"));
  ui.pbRemoveRecording->setIcon(KIcon("list-remove"));
  ui.pbAddSet->setIcon(KIcon("list-add"));
  ui.pbRenameSet->setIcon(KIcon("document-edit"));
  ui.pbRemoveSet->setIcon(KIcon("list-remove"));
  ui.pbExportSet->setIcon(KIcon("document-export"));
  ui.pbImportSet->setIcon(KIcon("document-import"));

  recordings = new QSortFilterProxyModel(this);
  recordings->setSortCaseSensitivity(Qt::CaseInsensitive);
  ui.tvRecordings->setModel(recordings);

  connect(ui.asBackends, SIGNAL(added(QListWidgetItem*)), this, SLOT(slotChanged()));
  connect(ui.asBackends, SIGNAL(movedUp(QListWidgetItem*)), this, SLOT(slotChanged()));
  connect(ui.asBackends, SIGNAL(movedDown(QListWidgetItem*)), this, SLOT(slotChanged()));
  connect(ui.asBackends, SIGNAL(removed(QListWidgetItem*)), this, SLOT(slotChanged()));
  connect(ui.asBackends->availableListWidget(), SIGNAL(currentRowChanged(int)), ui.asBackends, SLOT(polish()));
  connect(ui.asBackends->selectedListWidget(), SIGNAL(currentRowChanged(int)), ui.asBackends, SLOT(polish()));

  connect(ui.cbActiveSet, SIGNAL(currentIndexChanged(int)), this, SLOT(displayCurrentSet()));
  connect(ui.pbAddSet, SIGNAL(clicked()), this, SLOT(addSet()));
  connect(ui.pbRenameSet, SIGNAL(clicked()), this, SLOT(renameSet()));
  connect(ui.pbRemoveSet, SIGNAL(clicked()), this, SLOT(removeSet()));
  connect(ui.pbAddRecording, SIGNAL(clicked()), this, SLOT(addRecording()));
  connect(ui.pbEditRecording, SIGNAL(clicked()), this, SLOT(editRecording()));
  connect(ui.pbRemoveRecording, SIGNAL(clicked()), this, SLOT(removeRecording()));

  connect(ui.pbExportSet, SIGNAL(clicked()), this, SLOT(exportSet()));
  connect(ui.pbImportSet, SIGNAL(clicked()), this, SLOT(importSet()));

  connect(ui.kcfg_useRecordingsAcrossSets, SIGNAL(clicked()), this, SLOT(slotChanged()));

  connect(ui.leFilter, SIGNAL(textChanged(const QString&)), recordings, SLOT(setFilterWildcard(const QString&)));
  ui.tvRecordings->setSortingEnabled(true);

  addConfig(TTSConfiguration::self(), this);
}

void TTSSettings::exportSet()
{
  int currentSet = getCurrentlySelectedSet();
  if (currentSet == -1) return;

  QString path = KFileDialog::getSaveFileName(KUrl(), i18n("TTS Files *.tts"), this, i18n("Set output file"));
  if (path.isEmpty()) return;

  kDebug() << "Exporting set to: " << path;
  if (!sets->exportSet(path, currentSet))
    KMessageBox::sorry(this, i18nc("%1 is path", "Could not export set to \"%1\".", path));
}

void TTSSettings::importSet()
{
  QString path = KFileDialog::getOpenFileName(KUrl(), i18n("TTS Files *.tts"), this, i18nc("\"Set file\" is a substantive stating the file type", "Set file"));
  if (path.isEmpty()) return;

  kDebug() << "Importing set from: " << path;
  if (!sets->importSet(path))
    KMessageBox::sorry(this, i18n("Could not import set from \"%1\".", path));
  displaySets();
}

void TTSSettings::displayCurrentSet()
{
  int currentSet = getCurrentlySelectedSet();
  if (currentSet == -1)
    recordings->setSourceModel(0);
  else
    recordings->setSourceModel(sets->getSet(currentSet));

  ui.tvRecordings->resizeColumnsToContents();
  emit changed(true);
}

void TTSSettings::slotChanged()
{
  emit changed(true);
}

QString TTSSettings::translateBackend(const QString& backend)
{
  if (backend == "File")
    return i18nc("The option to use pre-recorded sounds instead of the tts system", "Recordings");

  if (backend == "Webservice")
    return i18nc("The option to use a web service to convert text to speech", "Webservice");

  if (backend == "Jovie")
    return i18nc("KDEs text to speech system Jovie", "Jovie");

  return i18n("Error");
}

void TTSSettings::load()
{
  KCModule::load();
  QStringList availableBackends;
  availableBackends << "File";
  availableBackends << "Jovie";
  availableBackends << "Webservice";
  QStringList selectedBackends = TTSConfiguration::backends();
  foreach (const QString& s, selectedBackends)
  {
    if (!availableBackends.contains(s))
    {
      kWarning() <<"Ignoring selected but anavailable backend " << s;
      continue;
    }
    availableBackends.removeAll(s);
    QListWidgetItem *item = new QListWidgetItem(translateBackend(s), ui.asBackends->selectedListWidget());
    item->setData(Qt::UserRole, s);
    ui.asBackends->selectedListWidget()->addItem(item);
  }
  foreach (const QString& a, availableBackends)
  {
    QListWidgetItem *item = new QListWidgetItem(translateBackend(a), ui.asBackends->availableListWidget());
    item->setData(Qt::UserRole, a);
    ui.asBackends->availableListWidget()->addItem(item);
  }

  setupSets();
  emit changed(false);
}

void TTSSettings::setupSets()
{
  delete sets;
  sets =  new RecordingSetCollection;
  if (!sets->init(KStandardDirs::locate("appdata", "ttsrec/ttssets.xml")))
    KMessageBox::sorry(this, i18n("Could not read recording sets from the configuration file."));

  displaySets(TTSConfiguration::activeSet());
}

void TTSSettings::displaySets(int changeToIndex)
{
  kDebug() << "Entering displaySets..." << sender();
  int oldSetIndex;
  if (changeToIndex != -1) oldSetIndex = changeToIndex;
  else oldSetIndex = ui.cbActiveSet->currentIndex();

  ui.cbActiveSet->clear();
  QList<int> setIds = sets->getSets();
  foreach (int id, setIds)
  {
    RecordingSet* set = sets->getSet(id);
    ui.cbActiveSet->addItem(set->name(), set->id());
  }
  kDebug() << "Old set index: " << oldSetIndex;
  if (((oldSetIndex == -1) && (ui.cbActiveSet->count() > 0)) ||
      (oldSetIndex >= ui.cbActiveSet->count()))
  {
    kDebug() << "Setting 0 index";
    ui.cbActiveSet->setCurrentIndex(0);
  }
  else
    ui.cbActiveSet->setCurrentIndex(oldSetIndex);
  kDebug() << "Old set index: " << oldSetIndex;

  displayCurrentSet();
}

void TTSSettings::save()
{
  KCModule::save();

  QStringList selectedBackends;
  for (int i=0; i < ui.asBackends->selectedListWidget()->count(); i++)
    selectedBackends << ui.asBackends->selectedListWidget()->item(i)->data(Qt::UserRole).toString();
  TTSConfiguration::setBackends(selectedBackends);
  kDebug() << "Selected backends: " << selectedBackends;

  TTSConfiguration::setActiveSet(getCurrentlySelectedSet());
  TTSConfiguration::self()->writeConfig();

  kDebug() << "Saving sets...";
  if (!sets->save(KStandardDirs::locateLocal("appdata", "ttsrec/ttssets.xml")))
    KMessageBox::sorry(this, i18n("Could not write recording sets to the configuration file."));

  SimonTTS::uninitialize();
  emit changed(false);
}

void TTSSettings::addSet()
{
  if (!sets) return;

  bool ok = false;
  QString setName = KInputDialog::getText(i18nc("Means: the name of the set", "Set name"),
      i18n("Please enter the name of the new set of recordings:"), QString(), 
      &ok, this);

  if (!ok) return;

  if (!sets->addSet(setName))
    KMessageBox::sorry(this, i18n("Could not add set: %1", setName));

  emit changed(true);
  displaySets();
  ui.cbActiveSet->setCurrentIndex(ui.cbActiveSet->count()-1);
}

void TTSSettings::renameSet()
{
  int currentIndex = ui.cbActiveSet->currentIndex();
  if (currentIndex == -1) return;

  bool ok = false;
  QString setName = KInputDialog::getText(i18nc("Means: the name of the set", "Set name"),
      i18n("Please enter the changed name of the set of recordings:"), ui.cbActiveSet->currentText(), 
      &ok, this);

  if (!ok) return;

  if (!sets->renameSet(getCurrentlySelectedSet(), setName))
    KMessageBox::sorry(this, i18n("Could not rename set to: %1", setName));

  emit changed(true);
  displaySets();
}

void TTSSettings::removeSet()
{
  int currentIndex = ui.cbActiveSet->currentIndex();
  if (currentIndex == -1) return;
  if (KMessageBox::questionYesNoCancel(this, i18n("Do you really want to irreversibly delete "
          "the current set and all associated samples?")) == KMessageBox::Yes)
    if (!sets->removeSet(getCurrentlySelectedSet()))
      KMessageBox::sorry(this, i18n("Could not remove set"));

  emit changed(true);
  displaySets();
}

int TTSSettings::getCurrentlySelectedSet()
{
  int currentIndex = ui.cbActiveSet->currentIndex();
  if (currentIndex == -1) return -1;
  return ui.cbActiveSet->itemData(currentIndex, Qt::UserRole).toInt();
}

QString TTSSettings::getCurrentlySelectedRecording()
{
  QModelIndexList selectedIndexes = ui.tvRecordings->selectionModel()->selectedIndexes();
  if (selectedIndexes.count() != 1)
    return QString();

  return recordings->data(selectedIndexes.at(0)).toString();
}

QString TTSSettings::getCurrentlySelectedPath()
{
  QModelIndexList selectedIndexes = ui.tvRecordings->selectionModel()->selectedIndexes();
  if (selectedIndexes.count() != 1)
    return QString();

  return recordings->data(selectedIndexes.at(0), Qt::UserRole).toString();
}

void TTSSettings::addRecording()
{
  int currentSet = getCurrentlySelectedSet();
  if (currentSet == -1) {
    KMessageBox::information(this, i18n("Please add or select a set above."));
    return;
  }

  QString text;
  QString path;
  QPointer<EditRecording> dlg = new EditRecording(this);
  if (!dlg->addRecording(text, path))
  {
    delete dlg;
    return;
  }

  kDebug() << "Adding: " << text << path << " to set " << currentSet;
  if (!sets->addRecording(currentSet, text, path))
    KMessageBox::sorry(this, i18n("Could not add recording to set."));

  emit changed(true);
  delete dlg;
}

void TTSSettings::editRecording()
{
  int currentSet = getCurrentlySelectedSet();
  if (currentSet == -1) {
    KMessageBox::information(this, i18n("Please add or select a set above."));
    return;
  }
  QString text = getCurrentlySelectedRecording();
  if (text.isNull()) {
    KMessageBox::information(this, i18n("Please select a recording above."));
    return;
  }

  QString path = getCurrentlySelectedPath();
  QPointer<EditRecording> dlg = new EditRecording(this);
  if (!dlg->editRecording(text, path))
  {
    delete dlg;
    return;
  }

  if (!sets->editRecording(currentSet, text, path))
    KMessageBox::sorry(this, i18n("Could not edit recording of set."));
  emit changed(true);

  delete dlg;
}

void TTSSettings::removeRecording()
{
  int currentSet = getCurrentlySelectedSet();
  if (currentSet == -1) {
    KMessageBox::information(this, i18n("Please add or select a set above."));
    return;
  }
  QString text = getCurrentlySelectedRecording();
  if (text.isNull()) {
    KMessageBox::information(this, i18n("Please select a recording above."));
    return;
  }

  if (KMessageBox::questionYesNoCancel(this, i18n("Do you really want to irreversibly remove this recording?")) == 
      KMessageBox::Yes)
  {
    if (!sets->removeRecording(currentSet, text))
      KMessageBox::sorry(this, i18n("Failed to remove the recording."));
  }
  emit changed(true);
}


void TTSSettings::defaults()
{
  KCModule::defaults();
}

TTSSettings::~TTSSettings()
{
  delete recordings;
  delete sets;
}
