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

K_PLUGIN_FACTORY( TTSSettingsFactory,
  registerPlugin< TTSSettings >();
)

K_EXPORT_PLUGIN( TTSSettingsFactory("TTSSettings") )

TTSSettings::TTSSettings(QWidget* parent, const QVariantList& args): KCModule(KGlobal::mainComponent(), parent)
{
  Q_UNUSED(args);

  ui.setupUi(this);

  ui.pbAdd->setIcon(KIcon("list-add"));
  ui.pbEdit->setIcon(KIcon("document-edit"));
  ui.pbRemove->setIcon(KIcon("list-remove"));
  connect(ui.asBackends, SIGNAL(added(QListWidgetItem*)), this, SLOT(slotChanged()));
  connect(ui.asBackends, SIGNAL(movedUp(QListWidgetItem*)), this, SLOT(slotChanged()));
  connect(ui.asBackends, SIGNAL(movedDown(QListWidgetItem*)), this, SLOT(slotChanged()));
  connect(ui.asBackends, SIGNAL(removed(QListWidgetItem*)), this, SLOT(slotChanged()));
  connect(ui.asBackends->availableListWidget(), SIGNAL(currentRowChanged(int)), ui.asBackends, SLOT(polish()));
  connect(ui.asBackends->selectedListWidget(), SIGNAL(currentRowChanged(int)), ui.asBackends, SLOT(polish()));

  addConfig(TTSConfiguration::self(), this);
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
  emit changed(false);
}

void TTSSettings::save()
{
  KCModule::save();

  QStringList selectedBackends;
  for (int i=0; i < ui.asBackends->selectedListWidget()->count(); i++)
    selectedBackends << ui.asBackends->selectedListWidget()->item(i)->data(Qt::UserRole).toString();
}


void TTSSettings::defaults()
{
  KCModule::defaults();
}

TTSSettings::~TTSSettings()
{}
