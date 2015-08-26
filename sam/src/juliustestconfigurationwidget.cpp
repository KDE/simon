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

#include "juliustestconfigurationwidget.h"
#include "ui_juliustestconfigurationwidget.h"
#include "samxmlhelper.h"

JuliusTestConfigurationWidget::JuliusTestConfigurationWidget(QWidget *parent) :
  TestConfigurationWidget(parent),
  ui(new Ui::JuliusTestConfigurationWidget)
{
  setupUi();
}

JuliusTestConfigurationWidget::JuliusTestConfigurationWidget(CorpusInformation* corpusInfo, const QUrl& testPromptsUrl,
                                                             const QUrl& testPromptsBasePathUrl, int sampleRate,
                                                             QWidget *parent):
                                  TestConfigurationWidget(corpusInfo, testPromptsUrl, testPromptsBasePathUrl, sampleRate, parent),
                                  ui(new Ui::JuliusTestConfigurationWidget){ setupUi(); }

JuliusTestConfigurationWidget::~JuliusTestConfigurationWidget()
{
  delete ui;
}

void JuliusTestConfigurationWidget::changeEvent(QEvent *e)
{
  QFrame::changeEvent(e);
  switch (e->type()) {
    case QEvent::LanguageChange:
      ui->retranslateUi(this);
      break;
    default:
      break;
  }
}

void JuliusTestConfigurationWidget::setupUi()
{
  ui->setupUi(this);

  ui->urJConf->setMode(KFile::File|KFile::ExistingOnly|KFile::LocalOnly);
  ui->urTestPromptsBasePath->setMode(KFile::Directory|KFile::ExistingOnly|KFile::LocalOnly);
  ui->urTestPrompts->setMode(KFile::File|KFile::ExistingOnly|KFile::LocalOnly);
  ui->urHmmDefs->setMode(KFile::File|KFile::LocalOnly);
  ui->urTiedlist->setMode(KFile::File|KFile::LocalOnly);
  ui->urDict->setMode(KFile::File|KFile::LocalOnly);
  ui->urDFA->setMode(KFile::File|KFile::LocalOnly);

  ui->urTestPrompts->setUrl(m_testPrompts);
  ui->urTestPromptsBasePath->setUrl(m_testPromptsBasePath);
  ui->sbSampleRate->setValue(m_sampRate);
  ui->leTag->setText(m_corpusInfo->tag());

  connect(ui->urTestPromptsBasePath, SIGNAL(textChanged(QString)), this, SLOT(update()));
  connect(ui->urTestPrompts, SIGNAL(textChanged(QString)), this, SLOT(update()));
  connect(ui->sbSampleRate, SIGNAL(valueChanged(int)), this, SLOT(update()));

  connect(ui->urJConf, SIGNAL(textChanged(QString)), this, SIGNAL(changed()));
  connect(ui->urHmmDefs, SIGNAL(textChanged(QString)), this, SIGNAL(changed()));
  connect(ui->urTiedlist, SIGNAL(textChanged(QString)), this, SIGNAL(changed()));
  connect(ui->urDict, SIGNAL(textChanged(QString)), this, SIGNAL(changed()));
  connect(ui->urDFA, SIGNAL(textChanged(QString)), this, SIGNAL(changed()));

  connect(ui->pbRemove, SIGNAL(clicked()), this, SLOT(deleteLater()));
  connect(ui->leTag, SIGNAL(editingFinished()), this, SIGNAL(tagChanged()));
  connect(ui->leTag, SIGNAL(textChanged(QString)), this, SLOT(updateTag(QString)));

}

void JuliusTestConfigurationWidget::update()
{
  emit changed();
  updateGeneralParams(ui->urTestPrompts->url(), ui->urTestPromptsBasePath->url(), ui->sbSampleRate->value());
}


QUrl JuliusTestConfigurationWidget::hmmDefs() const
{
  return ui->urHmmDefs->url();
}

QUrl JuliusTestConfigurationWidget::tiedlist() const
{
  return ui->urTiedlist->url();
}

QUrl JuliusTestConfigurationWidget::dict() const
{
  return ui->urDict->url();
}

QUrl JuliusTestConfigurationWidget::dfa() const
{
  return ui->urDFA->url();
}

QUrl JuliusTestConfigurationWidget::jconf() const
{
  return ui->urJConf->url();
}

void JuliusTestConfigurationWidget::init(const QHash<QString, QString> &params)
{
  ui->urHmmDefs->setUrl(QUrl(params.value("hmm")));
  ui->urTiedlist->setUrl(QUrl(params.value("tiedlist")));
  ui->urDict->setUrl(QUrl(params.value("dict")));
  ui->urDFA->setUrl(QUrl(params.value("dfa")));
  ui->urJConf->setUrl(QUrl(params.value("jconf")));

}

QDomElement JuliusTestConfigurationWidget::serialize(QDomDocument *doc)
{
  QDomElement elem = TestConfigurationWidget::serialize(doc);
  elem.setAttribute("Type", "Julius");

  SamXMLHelper::serializePath(doc, elem, ui->urHmmDefs, "hmm");
  SamXMLHelper::serializePath(doc, elem, ui->urTiedlist, "tiedlist");
  SamXMLHelper::serializePath(doc, elem, ui->urDict, "dict");
  SamXMLHelper::serializePath(doc, elem, ui->urDFA, "dfa");
  SamXMLHelper::serializePath(doc, elem, ui->urJConf, "jconf");

  return elem;
}
