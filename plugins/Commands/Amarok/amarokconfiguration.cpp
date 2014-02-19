/*   Copyright (C) 2009 Grasch Peter <peter.grasch@bedahr.org>
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

#include "amarokconfiguration.h"
#include "amarokcommandmanager.h"

#include <QVariantList>
#include <kgenericfactory.h>
#include <QList>
#include <KAboutData>
#include <KMessageBox>
#include <KStandardDirs>
#include <QString>
#include <QApplication>
#include <simonscenarios/scenario.h>

K_PLUGIN_FACTORY_DECLARATION(AmarokCommandPluginFactory)

AmarokConfiguration::AmarokConfiguration(AmarokCommandManager* _commandManager, Scenario *parent, const QVariantList &args)
: CommandConfiguration(parent,  "amarok", ki18n( "Amarok" ),
"0.1", ki18n("Control the Amarok music player"),
"amarok",
AmarokCommandPluginFactory::componentData())
{
  Q_UNUSED(args);
  ui.setupUi(this);

}



QDomElement AmarokConfiguration::serialize(QDomDocument* doc)
{
  QDomElement configElem = doc->createElement("config");

  //general
//   QDomElement caseSensitivityElem = doc->createElement("caseSensitivity");
//   caseSensitivityElem.appendChild(doc->createTextNode(ui.cbCaseSensitivity->isChecked() ? "1" : "0"));
//   configElem.appendChild(caseSensitivityElem);

  return configElem;
}


bool AmarokConfiguration::deSerialize(const QDomElement& elem)
{
//   QDomElement caseSensitivityElem = elem.firstChildElement("caseSensitivity");
//   if (caseSensitivityElem.isNull()) {
//     defaults();
//     return true;
//   }
//   ui.cbCaseSensitivity->setChecked(caseSensitivityElem.text() == "1");
  return true;
}


void AmarokConfiguration::defaults()
{
  
}


AmarokConfiguration::~AmarokConfiguration()
{
}
