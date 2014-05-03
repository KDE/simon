/*
 *   Copyright (C) 2014 Benjamin Bowley-Bryant <benbb@utexas.edu>
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

#include "dialogintegerfield.h"
#include <klocalizedstring.h>

const DialogFieldTypeInfo DialogIntegerField::typeInfo = DialogFieldTypeInfo("int",
									     i18nc("Integer","Variable type name of the dialog integer field"),
									     i18n("All whole negative and non-negative numbers"),
									     &DialogIntegerField::deSerializeDialogIntegerField,
									     &DialogIntegerField::createDialogIntegerField,
									     &DialogIntegerField::createDialogIntegerField,
									     &DialogIntegerFieldCreator::getCreatorInstance
									    );

DialogFieldBase* DialogIntegerField::deSerializeDialogIntegerField(const QDomElement& elem)
{
  DialogIntegerField* retval = new DialogIntegerField();
  if(!retval->deSerialize(elem))
  {
    delete retval;
    retval = 0;
    kWarning() << "deSerialization of DialogIntegerField failed!";
  }
  return retval;
}

DialogFieldBase* DialogIntegerField::createDialogIntegerField(const QString& name)
{
  DialogIntegerField* retval = new DialogIntegerField(name);
  retval->value = QSharedPointer<VariableType>(new VariableType());
  return retval;
}

DialogFieldBase* DialogIntegerField::createDialogIntegerField(const QString& name, const QString& value)
{
  DialogIntegerField* retval = new DialogIntegerField(name);
  retval->value = retval->parseValue(value);
  if(retval->value.isNull())
  {
    delete retval;
    retval = 0;
    kWarning() << "creation of DialogIntegerField failed!";
  }
  return retval;
}

QSharedPointer<DialogIntegerField::VariableType> DialogIntegerField::parseValue(const QString& value)
{
  bool ok;
  VariableType v = value.toInt(&ok);
  if(ok)
  {
    return QSharedPointer<VariableType>(new VariableType(v));
  }
  kWarning() << "Could not convert value into a QString.";
  return QSharedPointer<VariableType>(0);

}

QSharedPointer< DialogIntegerField::VariableType > DialogIntegerField::deSerializeValue(const QDomElement& elem)
{
  if(elem.isNull())
    return QSharedPointer<VariableType>(0);
  return parseValue(elem.text());
}

QDomElement DialogIntegerField::seriaizeValue(QDomDocument* doc)
{
  QDomElement result = doc->createElement("value");
  result.appendChild(doc->createTextNode(this->toString()));
  return result;
}
