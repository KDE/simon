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

#include "dialogfield.h"
#include <QString>

//TODO: Add Debug statements

const DialogFieldTypeInfo DialogIntegerField::typeInfo = DialogFieldTypeInfo("int","Integer",
									     "All whole negative and non-negative numbers",
									     &DialogIntegerField::deSerializeDialogIntegerField,
									     &DialogIntegerField::createDialogIntegerField);

DialogFieldBase* DialogIntegerField::deSerializeDialogIntegerField(const QDomElement& elem)
{
  DialogIntegerField* retval = new DialogIntegerField();
  if(!retval->deSerialize(elem))
  {
    delete retval;
    retval = 0;
  }
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
  }
  return retval;
}

QSharedPointer<int> DialogIntegerField::parseValue(const QString& value)
{
  bool ok;
  int v = value.toInt(&ok);
  if(ok)
  {
    return QSharedPointer<int>(new int(v));
  }
  return QSharedPointer<int>(0);

}

