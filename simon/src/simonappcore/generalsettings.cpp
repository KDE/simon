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

#include "generalsettings.h"
#include <KMessageBox>
#include <knewpassworddialog.h>
#include <KLocalizedString>
#include "coreconfiguration.h"
#include <kgenericfactory.h>

#ifdef Q_OS_WIN32
#include <QSettings>
#endif

K_PLUGIN_FACTORY( GeneralSettingsFactory,
registerPlugin< GeneralSettings >();
)

K_EXPORT_PLUGIN( GeneralSettingsFactory("GeneralSettings") )
/**
 * \brief Constructs a new GeneralSettings object
 * \author Peter Grasch
 * @param parent The parent of the widget
 */
GeneralSettings::GeneralSettings(QWidget* parent, const QVariantList& args):
KCModule(KGlobal::mainComponent(), parent)
{
  Q_UNUSED(args);

  ui.setupUi(this);

  addConfig(CoreConfiguration::self(), this);

  connect(ui.cbAskBeforeQuit, SIGNAL(toggled(bool)), this, SLOT(slotChanged()));
  connect(ui.cbShowSampleWarning, SIGNAL(toggled(bool)), this, SLOT(slotChanged()));
}


void GeneralSettings::slotChanged()
{
  emit changed(true);
}


void GeneralSettings::load()
{
  KCModule::load();

  KSharedConfig::Ptr config = KSharedConfig::openConfig("simonrc");
  KConfigGroup group(config, "Notification Messages");
  ui.cbAskBeforeQuit->setChecked(!group.readEntry("AskForQuitSimonMainWindow", false));
  ui.cbShowSampleWarning->setChecked(group.readEntry("ShowSampleWarning", false));
}


void GeneralSettings::save()
{
  KCModule::save();

  KSharedConfig::Ptr config = KSharedConfig::openConfig("simonrc");
  KConfigGroup group(config, "Notification Messages");

  if (ui.cbAskBeforeQuit->isChecked())
    group.deleteEntry("AskForQuitSimonMainWindow");
  else
    group.writeEntry("AskForQuitSimonMainWindow", true);

  if (!ui.cbShowSampleWarning->isChecked())
    group.deleteEntry("ShowSampleWarning");
  else
    group.writeEntry("ShowSampleWarning", true);

  config->sync();

  #ifdef Q_OS_WIN32
  QSettings settings(QSettings::UserScope, "Microsoft", "Windows");
  if (ui.kcfg_AutoStart->isChecked()) {
    // Want to start on boot up
    QString appPath = qApp->applicationFilePath();
    appPath.replace('/', '\\');
    settings.setValue("/CurrentVersion/Run/simon.exe", appPath);
  }
  else {
    // Do not want to start on boot up
    settings.remove("/CurrentVersion/Run/simon.exe");
  }
  settings.sync();
  #endif
}
