/*
 *   Copyright (C) 2011 Alessandro Buggin <alessandrobuggin@gmail.com>
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

#include <KMessageBox>
#include <KDialog>
#include <KDebug>
#include "sampleshare.h"
#include "ui_sampleshare.h"


SampleShare::SampleShare(QWidget* parent): 
    KDialog(parent),ui(new Ui::SampleShareDlg){
  QWidget *sampleShareWidget = new QWidget( this );
  ui->setupUi(sampleShareWidget);
  setMainWidget(sampleShareWidget);
  setButtonText(Ok, i18n("Upload"));
  setButtonIcon(Ok,KIcon("repository"));
  enableButtonOk(false);
  connect(ui->licenseBox, SIGNAL(clicked(bool)), this, SLOT(enableButtonOk(bool)));
  //connect(this, SIGNAL (okClicked()), this, SLOT(slotButtonClicked()));
  ui->kTextEdit->setPlainText(i18n("path : " )+ TrainingManager::getInstance()->getTrainingDir()+i18n(" ; \nrecorded files to upload:"));
  ui->progressBar->setValue(0);
  kDebug() << TrainingManager::getInstance()->getTrainingDir() << true;
  kDebug() << TrainingManager::getInstance()->getPrompts()->keys() <<true;
}

SampleShare::~SampleShare(){
delete ui;
}

void SampleShare::slotButtonClicked(int button) {
if (button == KDialog::Ok){
  if (ui->stackedWidget->currentWidget()==ui->stackedWidgetPage1)
    ui->stackedWidget->setCurrentWidget(ui->stackedWidgetPage2);
  else
    kDebug() << "already on second tab"<< true;
  }
else
KDialog::slotButtonClicked(button);
}

void enableButtonOk( bool state ){
  kDebug() << "license accepted"<< true ;
  if (state == (true)){
  enableButtonOk(true);
  kDebug() << "enabled Upload" << true;}
  else{
    enableButtonOk(false);
    kDebug() << "enabled Upload" << false;};
}
