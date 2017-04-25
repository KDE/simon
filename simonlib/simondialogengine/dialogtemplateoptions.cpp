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

#include "dialogtemplateoptions.h"
#include <QDomDocument>
#include <KLocalizedString>

DialogTemplateOptions::DialogTemplateOptions()
{

}

bool DialogTemplateOptions::deSerialize(const QDomElement& elem)
{
  options.clear();
 
  QDomElement option = elem.firstChildElement("option");
  while (!option.isNull())
  {
    options.insert(option.attribute("name"),
        (option.attribute("value") != "0"));

    option = option.nextSiblingElement("option");
  }

  return true;
}


Qt::ItemFlags DialogTemplateOptions::flags(const QModelIndex &index) const
{
  if (!index.isValid())
    return 0;

  return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant DialogTemplateOptions::headerData(int column, Qt::Orientation orientation,
                  int role) const
{
  if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
    switch (column) {
      case 0:
        return i18nc("Name of the template option", "Name");
      case 1:
        return i18nc("Value of the template option", "Value");
    }
  }

  //default
  return QVariant();
}

QModelIndex DialogTemplateOptions::parent(const QModelIndex &index) const
{
  Q_UNUSED(index);
  return QModelIndex();
}

int DialogTemplateOptions::rowCount(const QModelIndex &) const
{
  return options.keys().count();
}


QModelIndex DialogTemplateOptions::index(int row, int column,const QModelIndex &parent) const
{
  if (!hasIndex(row, column, parent) || parent.isValid())
    return QModelIndex();

   return createIndex(row, column, nullptr);
}

QVariant DialogTemplateOptions::data(const QModelIndex &index, int role) const
{
  if (!index.isValid()) return QVariant();

  if (role == Qt::DisplayRole)
  {
    switch (index.column())
    {
      case 0:
        return options.keys().at(index.row());
      case 1:
        return options.value(options.keys().at(index.row()));
    }
  }

  //always return internal key
  if (role == Qt::UserRole)
    return options.keys().at(index.row());

  return QVariant();
}

int DialogTemplateOptions::columnCount(const QModelIndex &) const
{
  return 2;
}


DialogTemplateOptions* DialogTemplateOptions::createInstance(const QDomElement& elem)
{
  DialogTemplateOptions *options = new DialogTemplateOptions();

  if (!options->deSerialize(elem))
  {
    delete options;
    return NULL;
  }

  return options;
}
QDomElement DialogTemplateOptions::serialize(QDomDocument *doc)
{
  QDomElement elem = doc->createElement("options");
  
  QMap<QString,bool>::const_iterator i = options.constBegin();
  while (i != options.constEnd())
  {
    QDomElement option = doc->createElement("option");
    option.setAttribute("name", i.key());
    option.setAttribute("value", QString::number(i.value()));
    elem.appendChild(option);
    ++i;
  }

  return elem;
}

void DialogTemplateOptions::addOption(const QString& name, bool enabled)
{
  options.insert(name, enabled);
  reset();
}

void DialogTemplateOptions::removeOption(const QString& name)
{
  options.remove(name);
  reset();
}

