/*
 *   Copyright (C) 2012 Vladislav Sitalo <root@stvad.org>
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

#include "sphinxtestconfigurationwidget.h"
#include "ui_sphinxtestconfigurationwidget.h"
#include "samxmlhelper.h"

SphinxTestConfigurationWidget::SphinxTestConfigurationWidget(QWidget *parent) :
  TestConfigurationWidget(parent),
  ui(new Ui::SphinxTestConfigurationWidget)
{
  setupUi();
}

SphinxTestConfigurationWidget::SphinxTestConfigurationWidget(CorpusInformation* corpusInfo, const KUrl& testPromptsUrl,
                                                             const KUrl& testPromptsBasePathUrl, int sampleRate,
                                                             QWidget *parent):
                                  TestConfigurationWidget(corpusInfo, testPromptsUrl, testPromptsBasePathUrl, sampleRate, parent),
                                  ui(new Ui::SphinxTestConfigurationWidget){ setupUi(); }

SphinxTestConfigurationWidget::~SphinxTestConfigurationWidget()
{
  delete ui;
}

KUrl SphinxTestConfigurationWidget::sphinxModelDir() const
{
  return ui->urModelDir->url();
}

KUrl SphinxTestConfigurationWidget::sphinxGrammar() const
{
  return ui->urSphinxGrammar->url();
}

KUrl SphinxTestConfigurationWidget::sphinxDictionary() const
{
  return ui->urSphinxDict->url();
}

void SphinxTestConfigurationWidget::init(const QHash<QString, QString> &params)
{
  ui->urModelDir->setUrl(KUrl(params.value("sphinxModelDir")));
  ui->urSphinxDict->setUrl(KUrl(params.value("sphinxDictionary")));
  ui->urSphinxGrammar->setUrl(KUrl(params.value("sphinxGrammar")));
}

QDomElement SphinxTestConfigurationWidget::serialize(QDomDocument *doc)
{
  QDomElement elem = TestConfigurationWidget::serialize(doc);
  elem.setAttribute("Type", "Sphinx");

  SamXMLHelper::serializePath(doc, elem, ui->urModelDir, "sphinxModelDir");
  SamXMLHelper::serializePath(doc, elem, ui->urSphinxDict, "sphinxDictionary");
  SamXMLHelper::serializePath(doc, elem, ui->urSphinxGrammar, "sphinxGrammar");

  return elem;
}

void SphinxTestConfigurationWidget::setupUi()
{
  ui->setupUi(this);

  ui->urModelDir->setMode(KFile::Directory|KFile::ExistingOnly|KFile::LocalOnly);
  ui->urSphinxDict->setMode(KFile::File|KFile::ExistingOnly|KFile::LocalOnly);
  ui->urSphinxGrammar->setMode(KFile::File|KFile::ExistingOnly|KFile::LocalOnly);
  ui->urTestPromptsBasePath->setMode(KFile::Directory|KFile::ExistingOnly|KFile::LocalOnly);
  ui->urTestPrompts->setMode(KFile::File|KFile::ExistingOnly|KFile::LocalOnly);

  ui->urTestPrompts->setUrl(m_TestPrompts);
  ui->urTestPromptsBasePath->setUrl(m_TestPromptsBasePath);
  ui->sbSampleRate->setValue(m_SampRate);
  ui->leTag->setText(m_corpusInfo->tag());

  connect(ui->urTestPromptsBasePath, SIGNAL(textChanged(QString)), this, SLOT(update()));
  connect(ui->urTestPrompts, SIGNAL(textChanged(QString)), this, SLOT(update()));
  connect(ui->sbSampleRate, SIGNAL(valueChanged(int)), this, SLOT(update()));

  connect(ui->urModelDir, SIGNAL(textChanged(QString)), this, SIGNAL(changed()));
  connect(ui->urSphinxDict, SIGNAL(textChanged(QString)), this, SIGNAL(changed()));
  connect(ui->urSphinxGrammar, SIGNAL(textChanged(QString)), this, SIGNAL(changed()));

  connect(ui->pbRemove, SIGNAL(clicked()), this, SLOT(deleteLater()));
  connect(ui->leTag, SIGNAL(editingFinished()), this, SIGNAL(tagChanged()));
  connect(ui->leTag, SIGNAL(textChanged(QString)), this, SLOT(updateTag(QString)));


}

void SphinxTestConfigurationWidget::update()
{
  emit changed();
  updateGeneralParams(ui->urTestPrompts->url(), ui->urTestPromptsBasePath->url(), ui->sbSampleRate->value());
}
