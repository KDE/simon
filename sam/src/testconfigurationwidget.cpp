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

#include "testconfigurationwidget.h"
#include "corpusinformation.h"

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
        const KUrl& jconfUrl, int sampleRate, QWidget *parent) : QFrame(parent),
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
  ui.sbSampleRate->setValue(sampleRate);
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

  connect(ui.pbRemove, SIGNAL(clicked()), this, SLOT(deleteLater()));
  connect(ui.leTag, SIGNAL(editingFinished()), this, SIGNAL(tagChanged()));
  connect(ui.leTag, SIGNAL(editingFinished()), this, SLOT(updateTag()));
}

void TestConfigurationWidget::updateTag()
{
  m_corpusInfo->setTag(ui.leTag->text());
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

int TestConfigurationWidget::sampleRate() const
{
  return ui.sbSampleRate->value();
}

TestConfigurationWidget::~TestConfigurationWidget()
{
  delete m_corpusInfo;
}

