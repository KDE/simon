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

#include <KDebug>
#include "testconfigurationwidget.h"
#include "corpusinformation.h"
#include "samxmlhelper.h"

TestConfigurationWidget::TestConfigurationWidget(QWidget *parent) : QFrame(parent)
{
  m_corpusInfo = new CorpusInformation;
  setupUi();
}

void TestConfigurationWidget::remove()
{
  deleteLater();
}

TestConfigurationWidget::TestConfigurationWidget(CorpusInformation *info, //const QString& tag,
        const KUrl& hmmDefsUrl,
        const KUrl& tiedlistUrl, const KUrl& dictUrl, const KUrl& dfaUrl,
        const KUrl& testPromptsUrl, const KUrl& testPromptsBasePathUrl,
        const KUrl& jconfUrl, BackendType type,
        const KUrl &sphinxModelDir, const KUrl &sphinxGrammar, const KUrl &sphinxDictionary,
        int sampleRate, QWidget *parent) : QFrame(parent),
  m_corpusInfo(info)
{
  setupUi();
  ui.leTag->setText(info->tag());
  ui.urHmmDefs->setUrl(hmmDefsUrl);
  ui.urTiedlist->setUrl(tiedlistUrl);
  ui.urDict->setUrl(dictUrl);
  ui.urDFA->setUrl(dfaUrl);
  ui.urTestPrompts->setUrl(testPromptsUrl);
  ui.urTestPromptsBasePath->setUrl(testPromptsBasePathUrl);
  ui.urJConf->setUrl(jconfUrl);
  ui.urModelDir->setUrl(sphinxModelDir);
  ui.urSphinxDict->setUrl(sphinxDictionary);
  ui.urSphinxGrammar->setUrl(sphinxGrammar);
  ui.sbSampleRate->setValue(sampleRate);

  ui.cbType->setCurrentIndex(BackendTypeToInt(type));
}

void TestConfigurationWidget::setupUi()
{
  ui.setupUi(this);
  ui.urJConf->setMode(KFile::File|KFile::ExistingOnly|KFile::LocalOnly);
  ui.urTestPromptsBasePath->setMode(KFile::Directory|KFile::ExistingOnly|KFile::LocalOnly);
  ui.urTestPrompts->setMode(KFile::File|KFile::ExistingOnly|KFile::LocalOnly);
  ui.urHmmDefs->setMode(KFile::File|KFile::LocalOnly);
  ui.urTiedlist->setMode(KFile::File|KFile::LocalOnly);
  ui.urDict->setMode(KFile::File|KFile::LocalOnly);
  ui.urDFA->setMode(KFile::File|KFile::LocalOnly);

  ui.urModelDir->setMode(KFile::Directory|KFile::ExistingOnly|KFile::LocalOnly);
  ui.urSphinxDict->setMode(KFile::File|KFile::ExistingOnly|KFile::LocalOnly);
  ui.urSphinxGrammar->setMode(KFile::File|KFile::ExistingOnly|KFile::LocalOnly);

  connect(ui.urJConf, SIGNAL(textChanged(QString)), this, SIGNAL(changed()));
  connect(ui.urTestPromptsBasePath, SIGNAL(textChanged(QString)), this, SIGNAL(changed()));
  connect(ui.urTestPrompts, SIGNAL(textChanged(QString)), this, SIGNAL(changed()));
  connect(ui.urHmmDefs, SIGNAL(textChanged(QString)), this, SIGNAL(changed()));
  connect(ui.urTiedlist, SIGNAL(textChanged(QString)), this, SIGNAL(changed()));
  connect(ui.urDict, SIGNAL(textChanged(QString)), this, SIGNAL(changed()));
  connect(ui.urDFA, SIGNAL(textChanged(QString)), this, SIGNAL(changed()));
  connect(ui.sbSampleRate, SIGNAL(valueChanged(int)), this, SIGNAL(changed()));

  connect(ui.cbType, SIGNAL(currentIndexChanged(int)), this, SIGNAL(changed()));

  connect(ui.urModelDir, SIGNAL(textChanged(QString)), this, SIGNAL(changed()));
  connect(ui.urSphinxDict, SIGNAL(textChanged(QString)), this, SIGNAL(changed()));
  connect(ui.urSphinxGrammar, SIGNAL(textChanged(QString)), this, SIGNAL(changed()));

  connect(ui.pbRemove, SIGNAL(clicked()), this, SLOT(deleteLater()));
  connect(ui.leTag, SIGNAL(editingFinished()), this, SIGNAL(tagChanged()));
  connect(ui.leTag, SIGNAL(textChanged(QString)), this, SLOT(updateTag(QString)));
}

void TestConfigurationWidget::updateTag(const QString& tag)
{
  m_corpusInfo->setTag(tag);
}

void TestConfigurationWidget::retrieveTag()
{
  QString oldTag = ui.leTag->text();
  QString currentTag = m_corpusInfo->tag();
  ui.leTag->setText(currentTag);
  if (currentTag != oldTag)
    emit tagChanged();
}

QString TestConfigurationWidget::tag() const
{
  return ui.leTag->text();
}

KUrl TestConfigurationWidget::hmmDefs() const
{
  return ui.urHmmDefs->url();
}

KUrl TestConfigurationWidget::tiedlist() const
{
  return ui.urTiedlist->url();
}

KUrl TestConfigurationWidget::dict() const
{
  return ui.urDict->url();
}

KUrl TestConfigurationWidget::dfa() const
{
  return ui.urDFA->url();
}

KUrl TestConfigurationWidget::testPrompts() const
{
  return ui.urTestPrompts->url();
}

KUrl TestConfigurationWidget::testPromptsBasePath() const
{
  return ui.urTestPromptsBasePath->url();
}

KUrl TestConfigurationWidget::jconf() const
{
  return ui.urJConf->url();
}

KUrl TestConfigurationWidget::sphinxModelDir() const
{
  return ui.urModelDir->url();
}

KUrl TestConfigurationWidget::sphinxGrammar() const
{
  return ui.urSphinxGrammar->url();
}

KUrl TestConfigurationWidget::sphinxDictionary() const
{
  return ui.urSphinxDict->url();
}

int TestConfigurationWidget::sampleRate() const
{
  return ui.sbSampleRate->value();
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

TestConfigurationWidget::BackendType TestConfigurationWidget::backendType() const
{
  kDebug()<<ui.cbType->currentIndex();
  return IntToBackendType(ui.cbType->currentIndex());
}

TestConfigurationWidget::~TestConfigurationWidget()
{
  delete m_corpusInfo;
}

TestConfigurationWidget* TestConfigurationWidget::deSerialize(const QDomElement& elem)
{
  QDomElement corpusElem = elem.firstChildElement("corpus");
  CorpusInformation *corpusInfo = CorpusInformation::deSerialize(corpusElem);
  if (!corpusInfo) return 0;
  
  KUrl hmmDefsUrl = KUrl(SamXMLHelper::getText(elem, "hmm"));
  KUrl tiedlistUrl = KUrl(SamXMLHelper::getText(elem, "tiedlist"));
  KUrl dictUrl = KUrl(SamXMLHelper::getText(elem, "dict"));
  KUrl dfaUrl = KUrl(SamXMLHelper::getText(elem, "dfa"));
  KUrl testPromptsUrl = KUrl(SamXMLHelper::getText(elem, "testPrompts"));
  KUrl testPromptsBasePathUrl = KUrl(SamXMLHelper::getText(elem, "testPromptsBasePath"));
  KUrl jconfUrl = KUrl(SamXMLHelper::getText(elem, "jconf"));

  KUrl sphinxModelDir = KUrl(SamXMLHelper::getText(elem, "sphinxModelDir"));
  KUrl sphinxGrammar = KUrl(SamXMLHelper::getText(elem, "sphinxGrammar"));
  KUrl sphinxDictionary = KUrl(SamXMLHelper::getText(elem, "sphinxDictionary"));
  
  int sampleRate = SamXMLHelper::getInt(elem, "sampleRate");
  int bType = SamXMLHelper::getInt(elem, "backendType");
  
  return new TestConfigurationWidget(corpusInfo,
        hmmDefsUrl, tiedlistUrl, dictUrl, dfaUrl,
        testPromptsUrl, testPromptsBasePathUrl,                                    
        jconfUrl, IntToBackendType(bType),
        sphinxModelDir, sphinxGrammar, sphinxDictionary,
        sampleRate);
}

QDomElement TestConfigurationWidget::serialize(QDomDocument* doc)
{
  QDomElement elem = doc->createElement("testConfiguration");
  elem.appendChild(corpusInformation()->serialize(doc));
  
  SamXMLHelper::serializePath(doc, elem, ui.urHmmDefs, "hmm");
  SamXMLHelper::serializePath(doc, elem, ui.urTiedlist, "tiedlist");
  SamXMLHelper::serializePath(doc, elem, ui.urDict, "dict");
  SamXMLHelper::serializePath(doc, elem, ui.urDFA, "dfa");
  SamXMLHelper::serializePath(doc, elem, ui.urTestPrompts, "testPrompts");
  SamXMLHelper::serializePath(doc, elem, ui.urTestPromptsBasePath, "testPromptsBasePath");
  SamXMLHelper::serializePath(doc, elem, ui.urJConf, "jconf");

  SamXMLHelper::serializePath(doc, elem, ui.urModelDir, "sphinxModelDir");
  SamXMLHelper::serializePath(doc, elem, ui.urSphinxDict, "sphinxDictionary");
  SamXMLHelper::serializePath(doc, elem, ui.urSphinxGrammar, "sphinxGrammar");
  
  SamXMLHelper::serializeInt(doc, elem, sampleRate(), "sampleRate");
  SamXMLHelper::serializeInt(doc, elem, ui.cbType->currentIndex(), "backendType");
  
  return elem;
}

