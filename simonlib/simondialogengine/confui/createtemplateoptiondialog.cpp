/*
 *   Copyright (C) 2010 Peter Grasch <peter.grasch@bedahr.org>
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


#include "createtemplateoptiondialog.h"
#include "ui_createtemplateoptiondialog.h"

#include <QWidget>
#include <KLocalizedString>

CreateTemplateOptionDialog::CreateTemplateOptionDialog(QWidget *parent) : KDialog(parent),
  ui(new Ui::CreateTemplateOptionDialog())
{
  QWidget *main = new QWidget(this);
  ui->setupUi(main);
  setMainWidget(main);
  setCaption(i18n("Template option"));
}

QString CreateTemplateOptionDialog::getName()
{
  return ui->leOptionName->text();
}

bool CreateTemplateOptionDialog::getEnabled()
{
  return ui->cbEnabled->isChecked();
}

void CreateTemplateOptionDialog::setName(const QString& id)
{
  ui->leOptionName->setText(id);
}

void CreateTemplateOptionDialog::setNameReadOnly(bool readOnly)
{
  ui->leOptionName->setReadOnly(readOnly);
}

void CreateTemplateOptionDialog::setEnabled(bool enabled)
{
  ui->cbEnabled->setChecked(enabled);
}



CreateTemplateOptionDialog::~CreateTemplateOptionDialog()
{
}

