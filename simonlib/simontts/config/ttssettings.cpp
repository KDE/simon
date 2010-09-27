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
#include <simontts/simontts.h>
#include <simontts/recordingset.h>
#include <simontts/recordingsetcollection.h>
#include <QListWidget>
#include <kgenericfactory.h>
#include <KActionSelector>
#include <KInputDialog>
#include <KMessageBox>

K_PLUGIN_FACTORY( TTSSettingsFactory,
  registerPlugin< TTSSettings >();
)

K_EXPORT_PLUGIN( TTSSettingsFactory("TTSSettings") )

TTSSettings::TTSSettings(QWidget* parent, const QVariantList& args): KCModule(KGlobal::mainComponent(), parent),
  setDirty(false),
  oldSetIndex(-1),
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
  connect(ui.asBackends, SIGNAL(added(QListWidgetItem*)), this, SLOT(slotChanged()));
  connect(ui.asBackends, SIGNAL(movedUp(QListWidgetItem*)), this, SLOT(slotChanged()));
  connect(ui.asBackends, SIGNAL(movedDown(QListWidgetItem*)), this, SLOT(slotChanged()));
  connect(ui.asBackends, SIGNAL(removed(QListWidgetItem*)), this, SLOT(slotChanged()));
  connect(ui.asBackends->availableListWidget(), SIGNAL(currentRowChanged(int)), ui.asBackends, SLOT(polish()));
  connect(ui.asBackends->selectedListWidget(), SIGNAL(currentRowChanged(int)), ui.asBackends, SLOT(polish()));

  connect(ui.cbActiveSet, SIGNAL(currentIndexChanged(int)), this, SLOT(setSelectionChanged(int)));
  connect(ui.pbAddSet, SIGNAL(clicked()), this, SLOT(addSet()));
  connect(ui.pbRenameSet, SIGNAL(clicked()), this, SLOT(renameSet()));
  connect(ui.pbRemoveSet, SIGNAL(clicked()), this, SLOT(removeSet()));
  connect(ui.pbAddRecording, SIGNAL(clicked()), this, SLOT(addRecording()));
  connect(ui.pbEditRecording, SIGNAL(clicked()), this, SLOT(editRecording()));
  connect(ui.pbRemoveRecording, SIGNAL(clicked()), this, SLOT(removeRecording()));

  connect(ui.kcfg_useRecordingsAcrossSets, SIGNAL(clicked()), this, SLOT(slotChanged()));

  addConfig(TTSConfiguration::self(), this);
}

void TTSSettings::setSelectionChanged(int newIndex)
{
  emit changed(true);
  if (setDirty && (newIndex != oldSetIndex))
  {
    int ret = KMessageBox::questionYesNoCancel(this, i18n("Before you change / view another set you have to either save or revert your changes to this set.\n\nDo you want to save your current changes before continuing?\n\nSelecting \"Cancel\" will keep the current set open."));

    switch (ret)
    {
      case KMessageBox::Yes:
        //save current changes and continue
        save();
        break;

      case KMessageBox::No:
        break; // just continue but keep changed at true because we changed the 
        //currently active set

      case KMessageBox::Cancel:
        ui.cbActiveSet->setCurrentIndex(oldSetIndex);
        return;
    }
  }
  oldSetIndex = newIndex;

  displayCurrentSet();
}

void TTSSettings::displayCurrentSet()
{
  kDebug() << "Displaying set: " << ui.cbActiveSet->currentText();
}

void TTSSettings::slotChanged()
{
  emit changed(true);
}

QString TTSSettings::translateBackend(const QString& backend)
{
  if (backend == "File")
    return i18nc("The option to use pre-recorded sounds instead of the tts syste", "Recordings");

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
    KMessageBox::sorry(this, i18n("Couldn't read recording sets from the configuration file."));

  displaySets();
}

void TTSSettings::displaySets()
{
  ui.cbActiveSet->clear();
  QList<int> setIds = sets->getSets();
  foreach (int id, setIds)
  {
    RecordingSet* set = sets->getSet(id);
    ui.cbActiveSet->addItem(set->name(), set->id());
  }
  ui.cbActiveSet->setCurrentIndex(oldSetIndex);

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

  TTSConfiguration::self()->writeConfig();

  kDebug() << "Saving sets...";
  if (!sets->save(KStandardDirs::locateLocal("appdata", "ttsrec/ttssets.xml")))
    KMessageBox::sorry(this, i18n("Couldn't write recording sets to the configuration file."));
  else kDebug() << "Saved sets!";

  SimonTTS::uninitialize();
  emit changed(false);
}

void TTSSettings::addSet()
{
  if (!sets) return;

  bool ok = false;
  QString setName = KInputDialog::getText(i18n("Set name"), 
      i18n("Please enter the name of the new set of recordings:"), QString(), 
      &ok, this);

  if (!ok) return;

  if (!sets->addSet(setName))
    KMessageBox::sorry(this, i18n("Couldn't add set: %1", setName));

  emit changed(true);
  displaySets();
}

void TTSSettings::renameSet()
{
  int currentIndex = ui.cbActiveSet->currentIndex();
  if (currentIndex == -1) return;

  bool ok = false;
  QString setName = KInputDialog::getText(i18n("Set name"), 
      i18n("Please enter the changed name of the set of recordings:"), ui.cbActiveSet->currentText(), 
      &ok, this);

  if (!ok) return;

  //FIXME shouldn't I rather change the recordingsetcollection and call displaySets()?
  //ui.cbActiveSet->insertItem(currentIndex, setName);
  if (!sets->renameSet(getCurrentlySelectedSet(), setName))
    KMessageBox::sorry(this, i18n("Couldn't rename set to: %1", setName));

  emit changed(true);
  displaySets();
}

void TTSSettings::removeSet()
{
  int currentIndex = ui.cbActiveSet->currentIndex();
  if (currentIndex == -1) return;
  if (KMessageBox::questionYesNoCancel(this, i18n("Do you really want to irreversibly delete the current set?")) == KMessageBox::Yes)
    if (!sets->removeSet(getCurrentlySelectedSet()))
      KMessageBox::sorry(this, i18n("Couldn't remove set"));

  emit changed(true);
  displaySets();
}

int TTSSettings::getCurrentlySelectedSet()
{
  int currentIndex = ui.cbActiveSet->currentIndex();
  return ui.cbActiveSet->itemData(currentIndex, Qt::UserRole).toInt();
}

void TTSSettings::addRecording()
{
}

void TTSSettings::editRecording()
{
}

void TTSSettings::removeRecording()
{
}


void TTSSettings::defaults()
{
  KCModule::defaults();
}

TTSSettings::~TTSSettings()
{}
