/*   Copyright (C) 2011 Grasch Peter <peter.grasch@bedahr.org>
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

#include "atspiconfiguration.h"
#include "atspicommandmanager.h"

#include <QVariantList>
#include <kgenericfactory.h>
#include <QList>
#include <KAboutData>
#include <KMessageBox>
#include <KStandardDirs>
#include <KPluginFactory>
#include <QString>
#include <QTableView>
#include <QThread>
#include <QApplication>
#include <simonscenarios/scenario.h>

K_PLUGIN_FACTORY_DECLARATION(ATSPICommandPluginFactory)

ATSPIConfiguration::ATSPIConfiguration(ATSPICommandManager* _commandManager, Scenario *parent, const QVariantList &args)
: CommandConfiguration(parent,  "ATSPI", ki18n( "ATSPI" ),
"0.1", ki18n("Workspace integration"),
"help-hint",
ATSPICommandPluginFactory::componentData())
{
  Q_UNUSED(args);
  Q_UNUSED(_commandManager);
  ui.setupUi(this);

}

bool ATSPIConfiguration::createLanguageModel()
{
  return ui.cbCreateLanguageModel->isChecked();
}

QDomElement ATSPIConfiguration::serialize(QDomDocument* doc)
{
  QDomElement configElem = doc->createElement("config");

  //general
  QDomElement createLMElem = doc->createElement("createLM");
  createLMElem.appendChild(doc->createTextNode(ui.cbCreateLanguageModel->isChecked() ? "1" : "0"));
  configElem.appendChild(createLMElem);

  return configElem;
}


bool ATSPIConfiguration::deSerialize(const QDomElement& elem)
{
  QDomElement createLMElem = elem.firstChildElement("createLM");
  if (createLMElem.isNull()) {
    defaults();
    return true;
  }
  ui.cbCreateLanguageModel->setChecked(createLMElem.text() == "1");
  return true;
}


void ATSPIConfiguration::defaults()
{
  ui.cbCreateLanguageModel->setChecked(true);
}


ATSPIConfiguration::~ATSPIConfiguration()
{
}
