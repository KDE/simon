/*
 *  Copyright (C) 2010 Peter Grasch <grasch@simon-listens.org>
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

#include <KDebug>

TestConfigurationWidget::TestConfigurationWidget(QWidget *parent) : QFrame(parent)
{
  m_corpusInfo = new CorpusInformation;
}

void TestConfigurationWidget::remove()
{
  deleteLater();
}

TestConfigurationWidget::TestConfigurationWidget(CorpusInformation *info,
                                                 const KUrl& testPromptsUrl, const KUrl& testPromptsBasePathUrl,
                                                 int sampleRate, QWidget *parent) :
  QFrame(parent),
  m_corpusInfo(info),
  m_SampRate(sampleRate),
  m_TestPrompts(testPromptsUrl),
  m_TestPromptsBasePath(testPromptsBasePathUrl)
{
//  ui.leTag->setText(info->tag());
//  ui.urHmmDefs->setUrl(hmmDefsUrl);
//  ui.urTiedlist->setUrl(tiedlistUrl);
//  ui.urDict->setUrl(dictUrl);
//  ui.urDFA->setUrl(dfaUrl);
//  ui.urTestPrompts->setUrl(testPromptsUrl);
//  ui.urTestPromptsBasePath->setUrl(testPromptsBasePathUrl);
//  ui.urJConf->setUrl(jconfUrl);
//  ui.urModelDir->setUrl(sphinxModelDir);
//  ui.urSphinxDict->setUrl(sphinxDictionary);
//  ui.urSphinxGrammar->setUrl(sphinxGrammar);
//  ui.sbSampleRate->setValue(sampleRate);

//  ui.cbType->setCurrentIndex(BackendTypeToInt(type));

}

int TestConfigurationWidget::BackendTypeToInt(TestConfigurationWidget::BackendType type)
{
  int ind(-1);
  if(type == SPHINX)
    ind = 0;
  else if(type == JHTK)
    ind = 1;

  return ind;
}

TestConfigurationWidget::BackendType TestConfigurationWidget::IntToBackendType(int type)
{
  BackendType btype = SPHINX;
  if(type == 0)
    btype = SPHINX;
  else if(type == 1)
    btype = JHTK;

  return btype;
}

TestConfigurationWidget::BackendType TestConfigurationWidget::StringToBackendType(const QString& type)
{
  BackendType btype = SPHINX;
  if(type == "Sphinx")
    btype = SPHINX;
  else if(type == "Julius")
    btype = JHTK;

  return btype;
}

//void TestConfigurationWidget::setupUi()
//{
//  ui.setupUi(this);
//  ui.urJConf->setMode(KFile::File|KFile::ExistingOnly|KFile::LocalOnly);
//  ui.urTestPromptsBasePath->setMode(KFile::Directory|KFile::ExistingOnly|KFile::LocalOnly);
//  ui.urTestPrompts->setMode(KFile::File|KFile::ExistingOnly|KFile::LocalOnly);
//  ui.urHmmDefs->setMode(KFile::File|KFile::LocalOnly);
//  ui.urTiedlist->setMode(KFile::File|KFile::LocalOnly);
//  ui.urDict->setMode(KFile::File|KFile::LocalOnly);
//  ui.urDFA->setMode(KFile::File|KFile::LocalOnly);

//  ui.urModelDir->setMode(KFile::Directory|KFile::ExistingOnly|KFile::LocalOnly);
//  ui.urSphinxDict->setMode(KFile::File|KFile::ExistingOnly|KFile::LocalOnly);
//  ui.urSphinxGrammar->setMode(KFile::File|KFile::ExistingOnly|KFile::LocalOnly);

//  connect(ui.urJConf, SIGNAL(textChanged(QString)), this, SIGNAL(changed()));
//  connect(ui.urTestPromptsBasePath, SIGNAL(textChanged(QString)), this, SIGNAL(changed()));
//  connect(ui.urTestPrompts, SIGNAL(textChanged(QString)), this, SIGNAL(changed()));
//  connect(ui.urHmmDefs, SIGNAL(textChanged(QString)), this, SIGNAL(changed()));
//  connect(ui.urTiedlist, SIGNAL(textChanged(QString)), this, SIGNAL(changed()));
//  connect(ui.urDict, SIGNAL(textChanged(QString)), this, SIGNAL(changed()));
//  connect(ui.urDFA, SIGNAL(textChanged(QString)), this, SIGNAL(changed()));
//  connect(ui.sbSampleRate, SIGNAL(valueChanged(int)), this, SIGNAL(changed()));

//  connect(ui.cbType, SIGNAL(currentIndexChanged(int)), this, SIGNAL(changed()));

//  connect(ui.urModelDir, SIGNAL(textChanged(QString)), this, SIGNAL(changed()));
//  connect(ui.urSphinxDict, SIGNAL(textChanged(QString)), this, SIGNAL(changed()));
//  connect(ui.urSphinxGrammar, SIGNAL(textChanged(QString)), this, SIGNAL(changed()));

//  connect(ui.pbRemove, SIGNAL(clicked()), this, SLOT(deleteLater()));
//  connect(ui.leTag, SIGNAL(editingFinished()), this, SIGNAL(tagChanged()));
//  connect(ui.leTag, SIGNAL(textChanged(QString)), this, SLOT(updateTag(QString)));
//}

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



//TestConfigurationWidget::BackendType TestConfigurationWidget::backendType() const
//{
//  kDebug()<<ui.cbType->currentIndex();
//  return IntToBackendType(ui.cbType->currentIndex());
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

  BackendType type = StringToBackendType(elem.attribute("Type"));

  KUrl testPromptsUrl = KUrl(SamXMLHelper::getText(elem, "testPrompts"));
  KUrl testPromptsBasePathUrl = KUrl(SamXMLHelper::getText(elem, "testPromptsBasePath"));
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
  
  SamXMLHelper::serializePath(doc, elem, m_TestPrompts, "testPrompts");
  SamXMLHelper::serializePath(doc, elem, m_TestPromptsBasePath, "testPromptsBasePath");

  SamXMLHelper::serializeInt(doc, elem, m_SampRate, "sampleRate");
//  SamXMLHelper::serializeInt(doc, elem, ui.cbType->currentIndex(), "backendType");
  
  return elem;
}

void TestConfigurationWidget::updateGeneralParams(const KUrl &testPromptsUrl, const KUrl &testPromptsBasePathUrl, int sampleRate)
{
//  m_Tag = tag;
  m_TestPrompts = testPromptsUrl;
  m_TestPromptsBasePath = testPromptsBasePathUrl;
  m_SampRate = sampleRate;
}
