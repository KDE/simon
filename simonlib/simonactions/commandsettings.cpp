/*
 *   Copyright (C) 2008 Peter Grasch <peter.grasch@bedahr.org>
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

#include "commandsettings.h"
#include "commandsettingsinternal.h"
#include "ui_commandsettingsdlg.h"

#include <simonactions/listconfiguration.h>

#include <QVariant>
#include <KAboutData>
#include <KPluginFactory>

K_PLUGIN_FACTORY( CommandSettingsFactory,
registerPlugin< CommandSettings >();
)

// K_EXPORT_PLUGIN( CommandSettingsFactory("simonlib") )

/**
 * \brief Constructor
 *
 * Initializes the Systemwidget by giving name, icon and description
 *
 * \author Peter Grasch
 * @param parent The parent of the widget
 */
CommandSettings::CommandSettings(QWidget* parent, const QVariantList& args)
  : KCModule(parent),
    ui(new Ui::CommandSettingsDlg)
{
  Q_UNUSED(args)

  ui->setupUi(this);

  KAboutData *about = new KAboutData("commandsettings",
                                     i18n("Command Settings"),
                                     "0.1",
                                     i18n("Configuration about the Commands and the Selection of Plugins to use"),
                                     KAboutLicense::GPL);
  // #if KDE_IS_VERSION(4,0,80)
  // about->setProgramIconName("fork");
  // #endif
  setAboutData( about );

  QObject::connect(ui->cbUseDYM, SIGNAL(toggled(bool)), this, SLOT(slotChanged()));
  QObject::connect(ui->sbMinimumConfidence, SIGNAL(valueChanged(double)), this, SLOT(slotChanged()));
  QObject::connect(ui->fcFont, SIGNAL(fontSelected(QFont)), this, SLOT(slotChanged()));

  qDebug() << "List configuration: " << CommandSettingsInternal::getInstance()->minimumConfidence();
  ListConfiguration *listConfig = CommandSettingsInternal::getInstance()->getListConfiguration();
  qDebug() << "Returned...";

  qDebug() << "Got list config: " << listConfig;
  connect(listConfig, SIGNAL(changed()), this, SLOT(slotChanged()));

  ui->twActionConfig->addTab(CommandSettingsInternal::getInstance()->getListConfiguration(), i18n("Lists"));

  load();
}


void CommandSettings::save()
{
  CommandSettingsInternal::getInstance()->save(
    ui->cbUseDYM->isChecked(), ui->sbMinimumConfidence->value(), ui->fcFont->font());
  KCModule::save();
  emit changed(false);
}


void CommandSettings::load()
{
  CommandSettingsInternal::getInstance()->load();
  adaptViewToValues();

  emit changed(false);
  KCModule::load();
}


void CommandSettings::adaptViewToValues()
{
  ui->sbMinimumConfidence->setValue(CommandSettingsInternal::getInstance()->minimumConfidence());

  ui->cbUseDYM->setChecked(CommandSettingsInternal::getInstance()->useDYM());
  ui->fcFont->setFont(CommandSettingsInternal::getInstance()->pluginBaseFont());
}


void CommandSettings::defaults()
{
  KCModule::defaults();
  CommandSettingsInternal::getInstance()->defaults();

  load();
  save();
}


void CommandSettings::slotChanged()
{
  emit changed(true);
}


CommandSettings::~CommandSettings()
{
  ui->twActionConfig->removePage(CommandSettingsInternal::getInstance()->getListConfiguration());
  CommandSettingsInternal::getInstance()->getListConfiguration()->setParent(0);
  delete ui;
}

#include "commandsettings.moc"
