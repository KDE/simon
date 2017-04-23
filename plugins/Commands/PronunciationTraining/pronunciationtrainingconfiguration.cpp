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

#include "pronunciationtrainingconfiguration.h"
#include "pronunciationtrainingcommandmanager.h"
#include <simonscenarios/scenariomanager.h>
#include <QVariantList>
#include <KDebug>
#include <KPluginFactory>
#include <KAboutData>
#include <KMessageBox>

K_PLUGIN_FACTORY_DECLARATION(PronunciationTrainingPluginFactory)

PronunciationTrainingConfiguration::PronunciationTrainingConfiguration(Scenario *parent, const QVariantList &args)
: CommandConfiguration(parent, "pronunciationtraining", ki18n( "Pronunciation Training" ),
"0.1", ki18n("Pronunciation Training"),
"applications-education",
PronunciationTrainingPluginFactory::componentData())
{
  Q_UNUSED(args);
  ui.setupUi(this);

  ui.cbCategory->clear();

  QObject::connect(ui.cbCategory, SIGNAL(currentIndexChanged(int)), this, SLOT(slotChanged()));
}


void PronunciationTrainingConfiguration::initCategories()
{
  ui.cbCategory->clear();
  QStringList categories = ScenarioManager::getInstance()->getCategories(
    (SpeechModel::ModelElements)
    (SpeechModel::ShadowVocabulary|
    SpeechModel::AllScenariosVocabulary|
    SpeechModel::AllScenariosGrammar));
  ui.cbCategory->addItems(categories);
  ui.cbCategory->setCurrentIndex(ui.cbCategory->findText(storedCategory));
}


void PronunciationTrainingConfiguration::setVisible(bool visible)
{
  initCategories();
  QWidget::setVisible(visible);
}


bool PronunciationTrainingConfiguration::deSerialize(const QDomElement& elem)
{
  storedCategory = elem.firstChildElement("category").text();
  return true;
}


QDomElement PronunciationTrainingConfiguration::serialize(QDomDocument *doc)
{
  storedCategory = ui.cbCategory->currentText();
  QDomElement configElem = doc->createElement("config");
  QDomElement categoryElem = doc->createElement("category");
  categoryElem.appendChild(doc->createTextNode(storedCategory));
  configElem.appendChild(categoryElem);
  return configElem;
}


QString PronunciationTrainingConfiguration::category()
{
  return storedCategory;
}


void PronunciationTrainingConfiguration::defaults()
{
}


PronunciationTrainingConfiguration::~PronunciationTrainingConfiguration()
{

}
