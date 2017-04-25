/*
 *   Copyright (C) 2011 Peter Grasch <peter.grasch@bedahr.org>
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


#include "languageprofileview.h"
#include "ui_languageprofileview.h"
#include <simonscenarios/modelmanager.h>
#include <simonscenarios/scenariomanager.h>
#include <simonscenarios/shadowvocabulary.h>
#include <simongraphemetophoneme/graphemetophoneme.h>
#include <QWidget>

#include <QDebug>
#include <QPushButton>
#include <QFile>
#include <QStandardPaths>
#include <KMessageBox>
#include <QDir>


LanguageProfileView::LanguageProfileView(QWidget* parent, Qt::WFlags flags)
    : KDialog(parent, flags),
      ui(new Ui::LanguageProfileView),
      g2p(new GraphemeToPhoneme)
{
    QWidget *widget = new QWidget(this);
    ui->setupUi(widget);
    setMainWidget(widget);
    setWindowTitle(i18n("Create language profile..."));


    setButtonText(Ok, i18n("Create profile"));

    connect(g2p, SIGNAL(state(QString,int,int)), this, SLOT(displayState(QString,int,int)));
    connect(g2p, SIGNAL(success(QString)), this, SLOT(success(QString)));
    connect(g2p, SIGNAL(failed()), this, SLOT(failed()));
}

void LanguageProfileView::displayState(const QString& state, int now, int max)
{
  ui->pbTotal->setFormat(state);
  ui->pbTotal->setValue(now);
  ui->pbTotal->setMaximum(max);
}

void LanguageProfileView::success(const QString& path)
{
  QString storePath = QStandardPaths::writableLocation(QStandardPaths::DataLocation) + QLatin1Char('/') + "model/languageProfile";
  if ((QFile::exists(storePath) && !QFile::remove(storePath)) ||
       !QFile::copy(path, storePath)) {
    KMessageBox::sorry(this, i18n("Could not copy model to final destination."));
    failed();
  } else {
    ModelManager::getInstance()->touchLanguageDescription();
    ModelManager::getInstance()->setLanguageProfileName(i18n("Generated from shadow dictionary"));
    setButtonText(Ok,i18n("OK"));
    enableButton(KDialog::Ok,true);
  }
}

void LanguageProfileView::failed()
{
  enableButton(KDialog::Ok,true);
}



LanguageProfileView::~LanguageProfileView()
{
  g2p->deleteLater();
}


void LanguageProfileView::slotButtonClicked(int button)
{
  if ((button == Ok) && (g2p->getState() == GraphemeToPhoneme::Idle))
    createProfile();
  // else
    //QT5TODO: Adapt code and connect okbutton or other to new slot. It doesn't exist in qdialog
    //QDialog::slotButtonClicked(button);
}


void LanguageProfileView::createProfile()
{
  enableButton(KDialog::Ok,false);

  //prepare sphinx dict
  if (!ScenarioManager::getInstance()->getShadowVocabulary()->
    exportToFile(QDir::tempPath() + QLatin1Char('/') +  "simon/sequitur/train.lex", Vocabulary::SPHINX)) {
    KMessageBox::sorry(this, i18n("Could not export current shadow dictionary to file for further processing."));
    return;
  }

  if (!g2p->createProfile())
    KMessageBox::sorry(this, g2p->getError(), QString(), KMessageBox::Notify|KMessageBox::AllowLink);
}

void LanguageProfileView::done(int p)
{
  disconnect(g2p);
  g2p->abort();
  QDialog::done(p);
}

