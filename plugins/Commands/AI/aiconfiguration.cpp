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

#include "aiconfiguration.h"
#include "aicommandmanager.h"
#include <QVariantList>
#include <QDir>
#include <KDELibs4Support/kgenericfactory.h>
#include <QStandardPaths>
#include <KPluginFactory>

K_PLUGIN_FACTORY_DECLARATION(AIPluginFactory)

AIConfiguration::AIConfiguration(Scenario *parent, const QVariantList &args)
: CommandConfiguration(parent,
                       QString("ai"),
                       i18n( "Artificial Intelligence"),
                       QByteArray("0.1"),
                       i18n("Voice controlled chatbot"),
                       QString("view-media-artist"))
, manager(0)
{
  Q_UNUSED(args);
  ui.setupUi(this);

  QObject::connect(ui.cbAimlSets, SIGNAL(currentIndexChanged(int)), this, SLOT(slotChanged()));
}


void AIConfiguration::setManager(AICommandManager *manager)
{
  this->manager = manager;
}


QDomElement AIConfiguration::serialize(QDomDocument *doc)
{
  QString oldStoredAimlSet = storedAimlSet;
  storedAimlSet = ui.cbAimlSets->currentText();

  QDomElement elem = doc->createElement("config");
  QDomElement personalityElem = doc->createElement("personality");
  personalityElem.appendChild(doc->createTextNode(storedAimlSet));
  elem.appendChild(personalityElem);

  if (storedAimlSet != oldStoredAimlSet)
    if (manager) manager->setupParser();

  emit changed(false);

  return elem;
}


bool AIConfiguration::deSerialize(const QDomElement& elem)
{
  ui.cbAimlSets->clear();
  ui.cbAimlSets->addItems(QDir(QStandardPaths::locate(QStandardPaths::GenericDataLocation, "ai/aimls/")).entryList(QStringList(), QDir::Dirs|QDir::NoDotAndDotDot));

  QString personality = elem.firstChildElement("personality").text();
  if (personality.isNull()) personality = "Alice";//default

  ui.cbAimlSets->setCurrentIndex(ui.cbAimlSets->findText(personality));

  storedAimlSet = ui.cbAimlSets->currentText();

  emit changed(false);
  return true;
}


void AIConfiguration::defaults()
{
  ui.cbAimlSets->setCurrentIndex(ui.cbAimlSets->findText("Alice"));
}


AIConfiguration::~AIConfiguration()
{

}
