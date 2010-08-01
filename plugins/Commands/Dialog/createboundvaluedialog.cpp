/*
 *   Copyright (C) 2010 Peter Grasch <grasch@simon-listens.org>
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


#include "createboundvaluedialog.h"
#include "ui_createboundvalue.h"
#include "staticboundvalue.h"
#include "scriptboundvalue.h"

#include <QWidget>

#include <KMessageBox>
#include <KLocalizedString>

CreateBoundValueDialog::CreateBoundValueDialog(QWidget *parent) : KDialog(parent),
  ui(new Ui::CreateBoundValueDialog())
{
  QWidget *main = new QWidget(this);
  ui->setupUi(main);
  setMainWidget(main);
  setCaption(i18n("Bound value"));
}

BoundValue* CreateBoundValueDialog::createBoundValue(BoundValue *init)
{
  if (init)
  {
    //init
    ui->leName->setText(init->getName());

    //static
    StaticBoundValue *staticBoundValue = dynamic_cast<StaticBoundValue*>(init);
    if (staticBoundValue)
    {
      ui->cbType->setCurrentIndex(0);
      ui->leStaticValue->setText(staticBoundValue->getValue().toString());
    }

    //script
    ScriptBoundValue *scriptBoundValue = dynamic_cast<ScriptBoundValue*>(init);
    if (scriptBoundValue)
    {
      ui->cbType->setCurrentIndex(1);
      ui->teScript->setText(scriptBoundValue->getValue().toString());
    }
  }

  QString name;
  do
  {
    if (!exec()) return 0;
    name = ui->leName->text();
    if (name.isEmpty())
      KMessageBox::information(this, i18n("Each bound value has to have a name."));
  } 
  while (name.isEmpty());


  BoundValue *value = 0;

  switch (ui->cbType->currentIndex())
  {
    case 0:
      //static
      value = new StaticBoundValue(name, ui->leStaticValue->text());
      break;
    case 1:
      //script
      value = new ScriptBoundValue(name, ui->teScript->toPlainText());
      break;
    case 2:
      //plasma
      break;
  }

  return value;
}

CreateBoundValueDialog::~CreateBoundValueDialog()
{
}

