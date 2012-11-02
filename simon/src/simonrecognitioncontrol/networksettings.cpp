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

#include "networksettings.h"
#include <simonuicomponents/serveraddressselector.h>
#include "recognitionconfiguration.h"
#include <QPointer>
#include <kdeversion.h>
#include <KCMultiDialog>

/**
 * \brief Constructor - inits the help text and the gui
 * \author Peter Grasch
 * @param parent the parent of the widget
 */
NetworkSettings::NetworkSettings(QWidget* parent, const QVariantList& args): KCModule(KGlobal::mainComponent(), parent)
{
  Q_UNUSED(args);

  ui.setupUi(this);

  #if KDE_IS_VERSION(4,0,80)
  ServerAddressSelector *saSelector = new ServerAddressSelector(this);
  ui.kcfg_JuliusdServers->setCustomEditor(*(new KEditListBox::CustomEditor(saSelector, saSelector->lineEdit())));
  #endif

  addConfig(RecognitionConfiguration::self(), this);
  connect(ui.pbConfigureSimond, SIGNAL(clicked()), this, SLOT(configureSimond()));
}


void NetworkSettings::configureSimond()
{
  QPointer<KCMultiDialog> configDialog = new KCMultiDialog(this);
  configDialog->addModule("simondconfiguration", QStringList() << "");
  configDialog->addModule("simonmodelcompilationconfig", QStringList() << "");
  configDialog->exec();
  delete configDialog;
}


NetworkSettings::~NetworkSettings()
{}
