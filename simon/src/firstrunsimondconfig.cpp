/*
 *   Copyright (C) 2010 Peter Grasch <peter.grasch@bedahr.org>
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

#include "firstrunsimondconfig.h"
#include <KI18n/klocalizedstring.h>
#include <KSharedConfig>
#include <KConfigCore/KConfigGroup>

FirstRunSimondConfig::FirstRunSimondConfig(QWidget* parent)
: QWizardPage(parent)
{
  ui.setupUi(this);
  setTitle(i18n("Server"));
  connect(ui.rbLocalInstallation, SIGNAL(clicked()), this, SLOT(setLocalInstallation()));
}


bool FirstRunSimondConfig::validatePage()
{
  KSharedConfig::Ptr config = KSharedConfig::openConfig("simonrecognitionrc");

  KConfigGroup group(config, "Recognition");

  group.writeEntry("StartLocalSimond", ui.cbStartSimond->isChecked());
  group.writeEntry("StopLocalSimond", ui.cbStopSimond->isChecked());
  group.writeEntry("JuliusdServers", QStringList() << ui.leServer->text().split(", "));
  group.writeEntry("JuliusdAutoConnect", ui.cbConnectAutomatically->isChecked());
  group.sync();
  return true;
}


void FirstRunSimondConfig::setLocalInstallation()
{
  ui.leServer->setText("127.0.0.1:4444");
}


void FirstRunSimondConfig::initializePage()
{
  KSharedConfig::Ptr config = KSharedConfig::openConfig("simonrecognitionrc");

  KConfigGroup group(config, "Recognition");

  ui.cbStopSimond->setChecked(group.readEntry("StartLocalSimond", true));
  ui.cbStartSimond->setChecked(group.readEntry("StopLocalSimond", true));
  ui.cbConnectAutomatically->setChecked(group.readEntry("JuliusdAutoConnect", true));

  QStringList servers = group.readEntry("JuliusdServers", QStringList() << "127.0.0.1:4444");
  if ((servers.count() == 1) && servers.at(0).startsWith(QLatin1String("127.0.0.1")))
    ui.rbLocalInstallation->animateClick();
  else
    ui.rbRemoteInstallation->animateClick();

  ui.leServer->setText(servers.join(","));
}


FirstRunSimondConfig::~FirstRunSimondConfig()
{
}
