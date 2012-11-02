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
#include <kgenericfactory.h>
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

  ui.cbTerminal->clear();

  QObject::connect(ui.cbTerminal, SIGNAL(currentIndexChanged(int)), this, SLOT(slotChanged()));
}


void PronunciationTrainingConfiguration::initTerminals()
{
  ui.cbTerminal->clear();
  QStringList terminals = ScenarioManager::getInstance()->getTerminals(
    (SpeechModel::ModelElements)
    (SpeechModel::ShadowVocabulary|
    SpeechModel::AllScenariosVocabulary|
    SpeechModel::AllScenariosGrammar));
  ui.cbTerminal->addItems(terminals);
  ui.cbTerminal->setCurrentIndex(ui.cbTerminal->findText(storedTerminal));
}


void PronunciationTrainingConfiguration::setVisible(bool visible)
{
  initTerminals();
  QWidget::setVisible(visible);
}


bool PronunciationTrainingConfiguration::deSerialize(const QDomElement& elem)
{
  storedTerminal = elem.firstChildElement("terminal").text();
  return true;
}


QDomElement PronunciationTrainingConfiguration::serialize(QDomDocument *doc)
{
  storedTerminal = ui.cbTerminal->currentText();
  QDomElement configElem = doc->createElement("config");
  QDomElement terminalElem = doc->createElement("terminal");
  terminalElem.appendChild(doc->createTextNode(storedTerminal));
  configElem.appendChild(terminalElem);
  return configElem;
}


QString PronunciationTrainingConfiguration::terminal()
{
  return storedTerminal;
}


void PronunciationTrainingConfiguration::defaults()
{
}


PronunciationTrainingConfiguration::~PronunciationTrainingConfiguration()
{

}
