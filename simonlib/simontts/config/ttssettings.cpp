/*
 *   Copyright (C) 2008 Peter Grasch <grasch@simon-listens.org>
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
  oldSetIndex(-1)
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

  connect(ui.kcfg_activeSet, SIGNAL(currentIndexChanged(int)), this, SLOT(setSelectionChanged(int)));
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
        ui.kcfg_activeSet->setCurrentIndex(oldSetIndex);
        return;
    }
  }
  oldSetIndex = newIndex;

  displayCurrentSet();
}

void TTSSettings::displayCurrentSet()
{
  kDebug() << "Displaying set: " << ui.kcfg_activeSet->currentText();
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

  displayCurrentSet();

  emit changed(false);
}

void TTSSettings::save()
{
  KCModule::save();

  QStringList selectedBackends;
  for (int i=0; i < ui.asBackends->selectedListWidget()->count(); i++)
    selectedBackends << ui.asBackends->selectedListWidget()->item(i)->data(Qt::UserRole).toString();
  TTSConfiguration::setBackends(selectedBackends);

  emit changed(false);
}

void TTSSettings::addSet()
{
  bool ok = false;
  QString setName = KInputDialog::getText(i18n("Set name"), 
      i18n("Please enter the name of the new set of recordings:"), QString(), 
      &ok, this);

  if (!ok) return;

  ui.kcfg_activeSet->addItem(setName);
}

void TTSSettings::renameSet()
{
  int currentIndex = ui.kcfg_activeSet->currentIndex();
  if (currentIndex == -1) return;

  bool ok = false;
  QString setName = KInputDialog::getText(i18n("Set name"), 
      i18n("Please enter the changed name of the set of recordings:"), ui.kcfg_activeSet->currentText(), 
      &ok, this);

  if (!ok) return;

  ui.kcfg_activeSet->insertItem(currentIndex, setName);
  ui.kcfg_activeSet->removeItem(currentIndex+1);
}

void TTSSettings::removeSet()
{
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
