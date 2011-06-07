/*   Copyright (C) 2010 Grasch Peter <grasch@simon-listens.org>
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

#include "boundvaluesconfiguration.h"
#include "createboundvaluedialog.h"
#include "ui_boundvaluesconfiguration.h"
#include <simondialogengine/dialogboundvalues.h>
#include <simondialogengine/boundvalue.h>
#include <KDebug>
#include <KMessageBox>

BoundValuesConfiguration::BoundValuesConfiguration(QWidget *parent) : QWidget(parent),
  ui(new Ui::BoundValuesDlg()),
  boundValues(0)
{
  ui->setupUi(this);
  ui->pbAddBoundValue->setIcon(KIcon("list-add"));
  ui->pbRemoveBoundValue->setIcon(KIcon("list-remove"));
  ui->pbEditBoundValue->setIcon(KIcon("document-edit"));

  connect(ui->pbAddBoundValue, SIGNAL(clicked()), this, SLOT(addBoundValue()));
  connect(ui->pbEditBoundValue, SIGNAL(clicked()), this, SLOT(editBoundValue()));
  connect(ui->pbRemoveBoundValue, SIGNAL(clicked()), this, SLOT(removeBoundValue()));
}

QDomElement BoundValuesConfiguration::serialize(QDomDocument* doc)
{
  return boundValues->serialize(doc);
}

bool BoundValuesConfiguration::deSerialize(const QDomElement& elem)
{
  kDebug() << "Calling deSerialize()";
  QDomElement boundValuesElem = elem.firstChildElement("boundValues");
  if (!boundValues)
  {
    boundValues = DialogBoundValues::createInstance(boundValuesElem);
    if (!boundValues)
      return false;
    
    ui->tvBoundValues->setModel(boundValues);
  } else {
    boundValues->deSerialize(boundValuesElem);
  }
  return true;
}

void BoundValuesConfiguration::defaults()
{
  delete boundValues;
  boundValues = new DialogBoundValues();
  ui->tvBoundValues->setModel(boundValues);
}
  

BoundValue* BoundValuesConfiguration::getCurrentBoundValue()
{
  return static_cast<BoundValue*>(ui->tvBoundValues->currentIndex().internalPointer());
}

BoundValue* BoundValuesConfiguration::getCurrentBoundValueGraphical()
{
  BoundValue *value = getCurrentBoundValue();
  if (!value)
    KMessageBox::information(this, i18n("Please select a bound value from the table above."));
  return value;
}

void BoundValuesConfiguration::addBoundValue()
{
  kDebug() << "adding bound value...";
  CreateBoundValueDialog *dialog = new CreateBoundValueDialog(this);

  BoundValue *boundValue = dialog->createBoundValue();
 
  if (!boundValue)
    return;
  kDebug() << "got bound value...";

  boundValues->addBoundValue(boundValue);
  delete dialog;
  kDebug() << "done adding...";
  kDebug() << "Bound values: " << boundValues;
}

void BoundValuesConfiguration::editBoundValue()
{
  BoundValue *value = getCurrentBoundValueGraphical();
  if (!value) return;

  CreateBoundValueDialog *dialog = new CreateBoundValueDialog(this);
  BoundValue *newValue = dialog->createBoundValue(value);
  if (newValue)
  {
    if (!boundValues->removeBoundValue(value))
      delete value;
    if (!boundValues->addBoundValue(newValue))
      KMessageBox::sorry(this, i18n("Could not edit bound value."));
  }
  delete dialog;
}

void BoundValuesConfiguration::removeBoundValue()
{
  BoundValue *value = getCurrentBoundValueGraphical();
  if (!value || KMessageBox::questionYesNoCancel(this, 
        i18n("Do you really want to remove the selected bound value option?"))
      != KMessageBox::Yes) return;

  boundValues->removeBoundValue(value);
}



