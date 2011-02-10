/*   Copyright (C) 2010 Grasch Peter <grasch@simon-listens.org>
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

#include "outputconfiguration.h"
#include "ui_outputconfiguration.h"

OutputConfiguration::OutputConfiguration(QWidget* parent): QWidget(parent),
  ui(new Ui::OutputConfigurationDlg())
{
  ui->setupUi(this);
}

void OutputConfiguration::defaults()
{
  ui->gbGraphical->setChecked(true);
  ui->gbTextToSpeech->setChecked(false);
  ui->leOptionSeparator->setText(i18n("Please answer with any of the following options."));

  ui->elwRepeatTriggers->setItems(QStringList() << i18n("Repeat"));
  ui->leAnnounceRepeat->setText(i18n("Say \"Repeat\" to hear this text again."));

  ui->cbRepeatOnInvalidInput->setChecked(true);
  
  ui->sbAvatarSize->setValue(96),
  ui->cbDisplayAvatarNames->setChecked(true);
}

QDomElement OutputConfiguration::serialize(QDomDocument* doc)
{
  QDomElement outputElem = doc->createElement("output");
  QDomElement graphicalOutput = doc->createElement("gui");
  QDomElement ttsOutput = doc->createElement("tts");
  graphicalOutput.appendChild(doc->createTextNode(ui->gbGraphical->isChecked() ? "1" : "0"));
  
  graphicalOutput.setAttribute("avatarSize", QString::number(ui->sbAvatarSize->value()));
  graphicalOutput.setAttribute("avatarDisplayNames", ui->cbDisplayAvatarNames->isChecked() ? "1" : "0");
  
  ttsOutput.appendChild(doc->createTextNode(ui->gbTextToSpeech->isChecked() ? "1" : "0"));

  outputElem.appendChild(graphicalOutput);
  outputElem.appendChild(ttsOutput);


  QDomElement ttsOptions = doc->createElement("ttsOptions");
  QDomElement separatorElem = doc->createElement("optionsSeparator");
  separatorElem.appendChild(doc->createTextNode(ui->leOptionSeparator->text()));
  ttsOptions.appendChild(separatorElem);
      
  QDomElement repeatTriggersElem = doc->createElement("repeatTriggers");
  QStringList repeatTriggers = ui->elwRepeatTriggers->items();
  foreach (const QString& trigger, repeatTriggers)
  {
    QDomElement repeatTriggerElem = doc->createElement("repeatTrigger");
    repeatTriggerElem.appendChild(doc->createTextNode(trigger));
    repeatTriggersElem.appendChild(repeatTriggerElem);
  }
  ttsOptions.appendChild(repeatTriggersElem);

  QDomElement announceRepeatElem = doc->createElement("announceRepeat");
  announceRepeatElem.appendChild(doc->createTextNode(ui->leAnnounceRepeat->text()));
  ttsOptions.appendChild(announceRepeatElem);

  QDomElement repeatOnInvalidInputElem = doc->createElement("repeatOnInvalidInput");
  repeatOnInvalidInputElem.appendChild(doc->createTextNode(ui->cbRepeatOnInvalidInput->isChecked() ? "1" : "0"));
  ttsOptions.appendChild(repeatOnInvalidInputElem);
  outputElem.appendChild(ttsOptions);
  return outputElem;
}

#include <KDebug>
bool OutputConfiguration::deSerialize(const QDomElement& elem)
{
  QDomElement outputElem = elem.firstChildElement("output");
  if (outputElem.isNull()) {
    kDebug() << "Given null node";
    return false;
  }

  QDomElement graphicalOutput = outputElem.firstChildElement("gui");
  QDomElement ttsOutput = outputElem.firstChildElement("tts");
  ui->gbGraphical->setChecked(graphicalOutput.text() == "1");
  ui->gbTextToSpeech->setChecked(ttsOutput.text() == "1");
  
  ui->sbAvatarSize->setValue(graphicalOutput.attribute("avatarSize").toInt());
  ui->cbDisplayAvatarNames->setChecked(graphicalOutput.attribute("avatarDisplayNames") == "1");

  QDomElement ttsOptions = outputElem.firstChildElement("ttsOptions");
  QDomElement separatorElem = ttsOptions.firstChildElement("optionsSeparator");
  ui->leOptionSeparator->setText(separatorElem.text());
  QDomElement repeatTriggers = ttsOptions.firstChildElement("repeatTriggers");
  QDomElement repeatTrigger = repeatTriggers.firstChildElement("repeatTrigger");
  QStringList repeatTriggersStrs;
  while (!repeatTrigger.isNull())
  {
    repeatTriggersStrs << repeatTrigger.text();
    repeatTrigger = repeatTrigger.nextSiblingElement("repeatTrigger");
  }
  ui->elwRepeatTriggers->setItems(repeatTriggersStrs);

  QDomElement announceRepeatElem = ttsOptions.firstChildElement("announceRepeat");
  ui->leAnnounceRepeat->setText(announceRepeatElem.text());

  QDomElement repeatOnInvalidInputElem = ttsOptions.firstChildElement("repeatOnInvalidInput");
  ui->cbRepeatOnInvalidInput->setChecked(repeatOnInvalidInputElem.text() == "1");

  return true;
}


bool OutputConfiguration::useGUIOutput() const
{
  return ui->gbGraphical->isChecked();
}

bool OutputConfiguration::useTTSOutput() const
{
  return ui->gbTextToSpeech->isChecked();
}


QString OutputConfiguration::getOptionSeparatorText() const
{
  return ui->leOptionSeparator->text();
}

QString OutputConfiguration::getRepeatAnnouncement() const
{
  return ui->leAnnounceRepeat->text();
}

QStringList OutputConfiguration::getRepeatTriggers() const
{
  return ui->elwRepeatTriggers->items();
}

bool OutputConfiguration::getRepeatOnInvalidInput() const
{
  return ui->cbRepeatOnInvalidInput->isChecked();
}


bool OutputConfiguration::getDisplayAvatarNames() const
{
  return ui->cbDisplayAvatarNames->isChecked();
}

int OutputConfiguration::getAvatarSize() const
{
  return ui->sbAvatarSize->value();
}
