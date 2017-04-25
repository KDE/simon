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

#include "dialogboundvalues.h"
#include "boundvalue.h"
#include "argumentboundvalue.h"
#include <QDomDocument>
#include <KLocalizedString>
#include <QDebug>

DialogBoundValues::DialogBoundValues()
{
}

DialogBoundValues::~DialogBoundValues()
{
  qDeleteAll(boundValues);
}

bool DialogBoundValues::deSerialize(const QDomElement& elem)
{
  boundValues.clear();
 
  QDomElement valueElem = elem.firstChildElement("boundValue");
  while (!valueElem.isNull())
  {
    BoundValue *value = BoundValue::createInstance(valueElem);

    if (value)
    {
      boundValues << value;
    }
    else 
    {
      qWarning() << "Failed to create bound value instance";
    }

    valueElem = valueElem.nextSiblingElement("boundValue");
  }
  reset();

  return true;
}


Qt::ItemFlags DialogBoundValues::flags(const QModelIndex &index) const
{
  if (!index.isValid())
    return 0;

  return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant DialogBoundValues::headerData(int column, Qt::Orientation orientation,
                  int role) const
{
  if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
    switch (column) {
      case 0:
        return i18nc("Name of the bound value", "Name");
      case 1:
        return i18nc("Type of the bound value", "Type");
      case 2:
        return i18nc("Value of the bound value", "Value");
    }
  }

  //default
  return QVariant();
}

QModelIndex DialogBoundValues::parent(const QModelIndex &index) const
{
  Q_UNUSED(index);
  return QModelIndex();
}

int DialogBoundValues::rowCount(const QModelIndex &) const
{
  return boundValues.count();
}


QModelIndex DialogBoundValues::index(int row, int column,const QModelIndex &parent) const
{
  if (!hasIndex(row, column, parent) || parent.isValid())
    return QModelIndex();

   return createIndex(row, column, boundValues.at(row));
}

QVariant DialogBoundValues::data(const QModelIndex &index, int role) const
{
  if (!index.isValid()) return QVariant();

  if (role == Qt::DisplayRole)
  {
    BoundValue *value = boundValues.at(index.row());
    switch (index.column())
    {
      case 0:
        return value->getName();
      case 1:
        return value->getTypeName();
      case 2:
        return value->getValueDescription();
    }
  }

  return QVariant();
}

int DialogBoundValues::columnCount(const QModelIndex &) const
{
  return 3;
}


DialogBoundValues* DialogBoundValues::createInstance(const QDomElement& elem)
{
  DialogBoundValues *boundValues = new DialogBoundValues();

  if (!boundValues->deSerialize(elem))
  {
    delete boundValues;
    return NULL;
  }

  return boundValues;
}

QDomElement DialogBoundValues::serialize(QDomDocument *doc)
{
  QDomElement elem = doc->createElement("boundValues");

  foreach (BoundValue *value, boundValues)
  {
    QDomElement subElem = value->serialize(doc);
    elem.appendChild(subElem);
  }
  
  return elem;
}

bool DialogBoundValues::addBoundValue(BoundValue *value)
{
  if (!value) return false;

  beginInsertRows(QModelIndex(), boundValues.count(), boundValues.count());
  boundValues << value;
  endInsertRows();
  return true;
}

bool DialogBoundValues::removeBoundValue(BoundValue *value)
{
  for (int i=0; i < boundValues.count(); i++) {
    if (boundValues.at(i) == value) {
      beginRemoveRows(QModelIndex(), i, i);
      boundValues.removeAt(i);
      endRemoveRows();
      delete value;
      return true;
    }
  }
  return false;
}

QVariant DialogBoundValues::getBoundValue(const QString& name)
{
  foreach (BoundValue *value, boundValues)
    if (value->getName() == name)
      return value->getValue();

  return QVariant();
}



void DialogBoundValues::setArguments(const QStringList& arguments)
{
  foreach (BoundValue* b, boundValues)
  {
    ArgumentBoundValue *a = dynamic_cast<ArgumentBoundValue*>(b);
    if (a)
      a->setArguments(arguments);
  }
}
