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

#include "dialogdoublefield.h"
#include <klocalizedstring.h>

const DialogFieldTypeInfo DialogDoubleField::typeInfo = DialogFieldTypeInfo("double",
									     i18nc("Double","Variable type name of the dialog double field"),
									     i18n("All decimal values"),
									     &DialogDoubleField::deSerializeDialogDoubleField,
									     &DialogDoubleField::createDialogDoubleField,
									     &DialogDoubleField::createDialogDoubleField);

DialogFieldBase* DialogDoubleField::deSerializeDialogDoubleField(const QDomElement& elem)
{
  DialogDoubleField* retval = new DialogDoubleField();
  if(!retval->deSerialize(elem))
  {
    delete retval;
    retval = 0;
    kWarning() << "deSerialization of DialogDoubleField failed!";
  }
  return retval;
}

DialogFieldBase* DialogDoubleField::createDialogDoubleField(const QString& name)
{
  DialogDoubleField* retval = new DialogDoubleField(name);
  retval->value = QSharedPointer<VariableType>(new VariableType());
  return retval;
}

DialogFieldBase* DialogDoubleField::createDialogDoubleField(const QString& name, const QString& value)
{
  DialogDoubleField* retval = new DialogDoubleField(name);
  retval->value = retval->parseValue(value);
  if(retval->value.isNull())
  {
    delete retval;
    retval = 0;
    kWarning() << "creation of DialogDoubleField failed!";
  }
  return retval;
}

QSharedPointer<DialogDoubleField::VariableType> DialogDoubleField::parseValue(const QString& value)
{
  bool ok;
  VariableType v = value.toDouble(&ok);
  if(ok)
  {
    return QSharedPointer<VariableType>(new VariableType(v));
  }
  kWarning() << "Could not convert value into a QString.";
  return QSharedPointer<VariableType>(0);

}

QSharedPointer< DialogDoubleField::VariableType > DialogDoubleField::deSerializeValue(const QDomElement& elem)
{
  if(elem.isNull())
    return QSharedPointer<VariableType>(0);
  return parseValue(elem.text());
}

QDomElement DialogDoubleField::seriaizeValue(QDomDocument* doc)
{
  QDomElement result = doc->createElement("value");
  result.appendChild(doc->createTextNode(this->toString()));
  return result;
}
