/*
 *  Copyright (C) 2010 Peter Grasch <peter.grasch@bedahr.org>
 *  Copyright (C) 2012 Vladislav Sitalo <root@stvad.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2,
 *  or (at your option) any later version, as published by the Free
 *  Software Foundation
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details
 *
 *  You should have received a copy of the GNU General Public
 *  License along with this program; if not, write to the
 *  Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "sphinxtestconfigurationwidget.h"
#include "juliustestconfigurationwidget.h"
#include "samxmlhelper.h"

#include <QDebug>

TestConfigurationWidget::TestConfigurationWidget(QWidget *parent) : QFrame(parent)
{
  m_corpusInfo = new CorpusInformation;
}

void TestConfigurationWidget::remove()
{
  deleteLater();
}

TestConfigurationWidget::TestConfigurationWidget(CorpusInformation *info,
                                                 const QUrl& testPromptsUrl, const QUrl& testPromptsBasePathUrl,
                                                 int sampleRate, QWidget *parent) :
  QFrame(parent),
  m_corpusInfo(info),
  m_sampRate(sampleRate),
  m_testPrompts(testPromptsUrl),
  m_testPromptsBasePath(testPromptsBasePathUrl)
{
}

TestConfigurationWidget::BackendType TestConfigurationWidget::intToBackendType(int type)
{
  BackendType btype = SPHINX;
  if(type == 0)
    btype = SPHINX;
  else if(type == 1)
    btype = JHTK;

  return btype;
}

TestConfigurationWidget::BackendType TestConfigurationWidget::stringToBackendType(const QString& type)
{
  BackendType btype = SPHINX;
  if(type == "Sphinx")
    btype = SPHINX;
  else if(type == "Julius")
    btype = JHTK;

  return btype;
}

void TestConfigurationWidget::updateTag(const QString& tag)
{
  m_corpusInfo->setTag(tag);
}

//void TestConfigurationWidget::retrieveTag()
//{
//  QString oldTag = ui.leTag->text();
//  QString currentTag = m_corpusInfo->tag();
//  ui.leTag->setText(currentTag);
//  if (currentTag != oldTag)
//    emit tagChanged();
//}


TestConfigurationWidget::~TestConfigurationWidget()
{
  delete m_corpusInfo;
}

TestConfigurationWidget* TestConfigurationWidget::deSerialize(const QDomElement& elem)
{
  TestConfigurationWidget* result = 0;

  QDomElement corpusElem = elem.firstChildElement("corpus");
  CorpusInformation *corpusInfo = CorpusInformation::deSerialize(corpusElem);
  if (!corpusInfo) return 0;

  BackendType type = stringToBackendType(elem.attribute("Type"));

  QUrl testPromptsUrl = QUrl(SamXMLHelper::getText(elem, "testPrompts"));
  QUrl testPromptsBasePathUrl = QUrl(SamXMLHelper::getText(elem, "testPromptsBasePath"));
  int sampleRate = SamXMLHelper::getInt(elem, "sampleRate");

  QHash<QString,QString> params;
  if(type == SPHINX)
  {
    params.insert("sphinxModelDir", SamXMLHelper::getText(elem, "sphinxModelDir"));
    params.insert("sphinxGrammar", SamXMLHelper::getText(elem, "sphinxGrammar"));
    params.insert("sphinxDictionary", SamXMLHelper::getText(elem, "sphinxDictionary"));

    result = new SphinxTestConfigurationWidget(corpusInfo, testPromptsUrl,
                                               testPromptsBasePathUrl, sampleRate);
  }
  else if(type == JHTK)
  {
    params.insert("hmm", SamXMLHelper::getText(elem, "hmm"));
    params.insert("tiedlist", SamXMLHelper::getText(elem, "tiedlist"));
    params.insert("dict", SamXMLHelper::getText(elem, "dict"));
    params.insert("dfa",SamXMLHelper::getText(elem, "dfa"));
    params.insert("jconf", SamXMLHelper::getText(elem, "jconf"));

    result = new JuliusTestConfigurationWidget(corpusInfo, testPromptsUrl,
                                               testPromptsBasePathUrl, sampleRate);
  }

  result->init(params);
  
  return result;
}

QDomElement TestConfigurationWidget::serialize(QDomDocument* doc)
{
  QDomElement elem = doc->createElement("testConfiguration");
  elem.appendChild(corpusInformation()->serialize(doc));
  
  SamXMLHelper::serializePath(doc, elem, m_testPrompts, "testPrompts");
  SamXMLHelper::serializePath(doc, elem, m_testPromptsBasePath, "testPromptsBasePath");

  SamXMLHelper::serializeInt(doc, elem, m_sampRate, "sampleRate");
  
  return elem;
}

void TestConfigurationWidget::updateGeneralParams(const QUrl &testPromptsUrl, const QUrl &testPromptsBasePathUrl, int sampleRate)
{
  m_testPrompts = testPromptsUrl;
  m_testPromptsBasePath = testPromptsBasePathUrl;
  m_sampRate = sampleRate;
}
