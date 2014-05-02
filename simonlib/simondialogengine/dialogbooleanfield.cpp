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

#include "dialogbooleanfield.h"
#include <klocalizedstring.h>

const DialogFieldTypeInfo DialogBooleanField::typeInfo = DialogFieldTypeInfo("bool",
									     i18nc("Boolean","Variable type name of the dialog boolean field."),
									     i18n("Value that can be represented as true or false."),
									     &DialogBooleanField::deSerializeDialogBooleanField,
									     &DialogBooleanField::createDialogBooleanField,
									     &DialogBooleanField::createDialogBooleanField);

DialogFieldBase* DialogBooleanField::deSerializeDialogBooleanField(const QDomElement& elem)
{
  DialogBooleanField* retval = new DialogBooleanField();
  if(!retval->deSerialize(elem))
  {
    delete retval;
    retval = 0;
    kWarning() << "deSerialization of DialogBooleanField failed!";
  }
  return retval;
}

DialogFieldBase* DialogBooleanField::createDialogBooleanField(const QString& name)
{
  DialogBooleanField* retval = new DialogBooleanField(name);
  retval->value = QSharedPointer<VariableType>(new VariableType());
  return retval;
}

DialogFieldBase* DialogBooleanField::createDialogBooleanField(const QString& name, const QString& value)
{
  DialogBooleanField* retval = new DialogBooleanField(name);
  retval->value = retval->parseValue(value);
  if(retval->value.isNull())
  {
    delete retval;
    retval = 0;
    kWarning() << "creation of DialogBooleanField failed!";
  }
  return retval;
}

QSharedPointer<DialogBooleanField::VariableType> DialogBooleanField::parseValue(const QString& value)
{
  bool ok = value.toLower() == QString("true") || value.toLower() == QString("false");
  VariableType v = value.toLower() == QString("true");
  if(ok)
  {
    return QSharedPointer<VariableType>(new VariableType(v));
  }
  kWarning() << "Could not convert value into a QString.";
  return QSharedPointer<VariableType>(0);

}

QSharedPointer< DialogBooleanField::VariableType > DialogBooleanField::deSerializeValue(const QDomElement& elem)
{
  if(elem.isNull())
    return QSharedPointer<VariableType>(0);
  return parseValue(elem.text());
}

QDomElement DialogBooleanField::seriaizeValue(QDomDocument* doc)
{
  QDomElement result = doc->createElement("value");
  result.appendChild(doc->createTextNode(this->toString()));
  return result;
}

QString DialogBooleanField::toString()
{
  return QString(*getVal().data() ? "true" : "false");
}
