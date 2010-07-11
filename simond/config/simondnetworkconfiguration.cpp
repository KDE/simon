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

#include "simondnetworkconfiguration.h"
#include "simondconfiguration.h"
#include <QSslSocket>
#include <QSslCipher>

SimondNetworkConfiguration::SimondNetworkConfiguration(QWidget* parent, const QVariantList& args)
: KCModule(KGlobal::mainComponent(), parent)
{
  Q_UNUSED(args);

  ui.setupUi(this);

  addConfig(SimondConfiguration::self(), this);
  connect(ui.cbCipher, SIGNAL(currentIndexChanged(int)), this, SLOT(slotChanged()));
}


void SimondNetworkConfiguration::slotChanged()
{
  emit changed(true);
}


void SimondNetworkConfiguration::load()
{
  ui.cbCipher->clear();
  QString selectedCipher = SimondConfiguration::encryptionMethod();

  QList<QSslCipher> ciphers = QSslSocket::supportedCiphers();
  QStringList cipherStrs;
  QString cipherName;
  int selectedIndex=0;
  for (int i=0; i < ciphers.count(); i++) {
    cipherName = ciphers[i].name();
    if (cipherName == selectedCipher)
      selectedIndex =i;
    cipherStrs << cipherName;
  }

  ui.cbCipher->addItems(cipherStrs);
  ui.cbCipher->setCurrentIndex(selectedIndex);

  KCModule::load();
}


void SimondNetworkConfiguration::save()
{
  SimondConfiguration::setEncryptionMethod(ui.cbCipher->currentText());
  KCModule::save();
}


SimondNetworkConfiguration::~SimondNetworkConfiguration()
{

}
