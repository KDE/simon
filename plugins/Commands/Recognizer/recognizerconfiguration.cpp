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

#include "recognizerconfiguration.h"
#include <QVariantList>
#include <KDebug>
#include <kgenericfactory.h>
#include <KAboutData>
#include <KMessageBox>

K_PLUGIN_FACTORY_DECLARATION(RecognizerCommandPluginFactory)

RecognizerConfiguration::RecognizerConfiguration(Scenario *parent, const QVariantList &args)
  : CommandConfiguration(parent, "recognizer", ki18n( "Recognizer" ),
  "0.1", ki18n("Recognizer"),
  "text-field",
  RecognizerCommandPluginFactory::componentData())
{
  Q_UNUSED(args);
  ui.setupUi(this);

  QObject::connect(ui.leAppendText, SIGNAL(textChanged(const QString&)), this, SLOT(slotChanged()));
  
}


bool RecognizerConfiguration::deSerialize(const QDomElement& elem)
{
  QString text = elem.firstChildElement("postText").attribute("value");
  ui.leAppendText->setText(text);
  return true;
}


QDomElement RecognizerConfiguration::serialize(QDomDocument *doc)
{
  QDomElement configElem = doc->createElement("config");
  QDomElement postTextElem = doc->createElement("postText");
  postTextElem.setAttribute("value", ui.leAppendText->text());
  configElem.appendChild(postTextElem);
  return configElem;
}


QString RecognizerConfiguration::appendText()
{
  return ui.leAppendText->text();
}


void RecognizerConfiguration::defaults()
{
  ui.leAppendText->setText(" ");
}


RecognizerConfiguration::~RecognizerConfiguration()
{

}
