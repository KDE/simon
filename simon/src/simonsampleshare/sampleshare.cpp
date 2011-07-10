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
#include "sampleshare.h"
#include "ui_sampleshare.h"
#include <KDebug>

SampleShare::SampleShare(QWidget* parent): 
    KDialog(parent),ui(new Ui::SampleShareDlg){
  QWidget *sampleShareWidget = new QWidget( this );
  ui->setupUi(sampleShareWidget);
  setMainWidget(sampleShareWidget);
  setButtons( User1 /*| KDialog::Apply*/ | ui->kdialog->Cancel);
  setButtonText(ui->kdialog->User1, "Upload");
  setButtonIcon(User1,KIcon("go-next"));
  enableButton(User1,false);
 // ui->ageComboBox->itemData();
  connect(ui->licenseBox, SIGNAL(clicked(bool)), this, SLOT(enableButtonUser1(bool)));
  
}

SampleShare::~SampleShare(){
delete ui;
}

void SampleShare::enableButtonUser1(bool down){
  kDebug() << "license accepted"<< down ;
  if (down == true){
    kDebug() << "enabled Upload" << true;
    enableButton(User1, true);
  }
  else{
    kDebug() << "enabled Upload" << false;
    enableButton(User1,false);
  };
}
