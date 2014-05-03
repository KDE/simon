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

#include "dialogstringfield.h"
#include <klocalizedstring.h>

const DialogFieldTypeInfo DialogStringField::typeInfo = DialogFieldTypeInfo("QString",
									     i18nc("String","Variable type name of the dialog string field"),
									     i18n("Any arrangement of characters"),
									     &DialogStringField::deSerializeDialogStringField,
									     &DialogStringField::createDialogStringField,
									     &DialogStringField::createDialogStringField,
									     &DialogStringFieldCreator::getCreatorInstance
									   );

DialogFieldBase* DialogStringField::deSerializeDialogStringField(const QDomElement& elem)
{
  DialogStringField* retval = new DialogStringField();
  if(!retval->deSerialize(elem))
  {
    delete retval;
    retval = 0;
    kWarning() << "deSerialization of DialogStringField failed!";
  }
  return retval;
}

DialogFieldBase* DialogStringField::createDialogStringField(const QString& name)
{
  DialogStringField* retval = new DialogStringField(name);
  retval->value = QSharedPointer<VariableType>(new VariableType());
  return retval;
}

DialogFieldBase* DialogStringField::createDialogStringField(const QString& name, const QString& value)
{
  DialogStringField* retval = new DialogStringField(name);
  retval->value = retval->parseValue(value);
  if(retval->value.isNull())
  {
    delete retval;
    retval = 0;
    kWarning() << "creation of DialogStringField failed!";
  }
  return retval;
}

QSharedPointer< DialogStringField::VariableType > DialogStringField::deSerializeValue(const QDomElement& elem)
{
  if(elem.isNull())
    return QSharedPointer<VariableType>(0);
  return parseValue(elem.text());
}

QDomElement DialogStringField::seriaizeValue(QDomDocument* doc)
{
  QDomElement result = doc->createElement("value");
  result.appendChild(doc->createTextNode(this->toString()));
  return result;
}
