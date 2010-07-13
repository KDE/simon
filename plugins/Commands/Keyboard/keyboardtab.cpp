/*
 *   Copyright (C) 2009 Grasch Peter <grasch@simon-listens.org>
 *   Copyright (C) 2009 Mario Strametz <strmam06@htl-kaindorf.ac.at>
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

#include "keyboardtab.h"
#include <QString>
#include <QList>
#include <QDomDocument>
#include <KDebug>
#include <KLocalizedString>

KeyboardTab::KeyboardTab(const QDomElement& elem) : m_isNull(false)
{
  if (elem.isNull()) m_isNull = true;
  else {
    tabName = elem.attribute("name");
    QDomElement buttonElement = elem.firstChildElement();
    while (!buttonElement.isNull()) {
      KeyboardButton *btn = new KeyboardButton(buttonElement);
      if (!btn->isNull())
        buttonList << btn;
      else btn->deleteLater();
      buttonElement = buttonElement.nextSiblingElement();
    }
  }
}


KeyboardTab::KeyboardTab(QString name, QList<KeyboardButton *> bList)
: buttonList(bList),
tabName(name),
m_isNull(false)
{
}


KeyboardButton* KeyboardTab::findButton(const QString& name, bool caseSensitive)
{
  if (m_isNull) return 0;

  foreach (KeyboardButton *btn, buttonList) {
    if (btn->getTriggerReal().compare(name, (caseSensitive ? Qt::CaseSensitive : Qt::CaseInsensitive))==0)
      return btn;
  }
  return 0;
}


bool KeyboardTab::addButton(KeyboardButton* b)
{
  if (!b || m_isNull) return false;

  if (findButton(b->getTriggerReal()))
    return false;

  kDebug() << "Adding button";
  beginInsertRows(QModelIndex(), rowCount(), rowCount());
  buttonList.append(b);
  endInsertRows();
  return true;
}


bool KeyboardTab::deleteButton(KeyboardButton* b)
{
  if (m_isNull) return false;

  bool found=false;
  for (int i=0; i<buttonList.count(); i++) {
    if (buttonList[i] == b) {
      beginRemoveRows(QModelIndex(), i, i);
      buttonList.removeAt(i);
      endRemoveRows();
      found = true;
      i--;
    }
  }

  return found;
}


bool KeyboardTab::moveButtonUp(KeyboardButton *button)
{
  if (m_isNull) return false;

  if (!button || !buttonList.contains(button))
    return false;

  int buttonIndex = buttonList.indexOf(button);
  if (buttonIndex == 0)
    //already first
    return false;

  buttonList.takeAt(buttonIndex);
  buttonList.insert(buttonIndex-1, button);

  emit dataChanged(index(buttonIndex-1, 0), index(buttonIndex, columnCount()));
  return true;
}


bool KeyboardTab::moveButtonDown(KeyboardButton *button)
{
  if (m_isNull) return false;

  if (!button || !buttonList.contains(button))
    return false;

  int buttonIndex = buttonList.indexOf(button);
  if (buttonIndex == buttonList.count()-1)
    //already last
    return false;

  buttonList.takeAt(buttonIndex);
  buttonList.insert(buttonIndex+1, button);

  emit dataChanged(index(buttonIndex, 0), index(buttonIndex+1, columnCount()));
  return true;
}


bool KeyboardTab::triggerButton(const QString& trigger, bool caseSensitive)
{
  if (m_isNull) return false;
  KeyboardButton *b = findButton(trigger, caseSensitive);
  if (!b) return false;

  return b->trigger();
}


QString KeyboardTab::getTabName()
{
  if (m_isNull) return QString();

  return tabName;
}


void KeyboardTab::setTabName(const QString& newName)
{
  if (m_isNull) return;

  tabName = newName;
}


QVariant KeyboardTab::data(const QModelIndex &index, int role) const
{
  if (m_isNull) return QVariant();

  if (!index.isValid() || role != Qt::DisplayRole)
    return QVariant();

  KeyboardButton *b = static_cast<KeyboardButton*>(index.internalPointer());
  if (!b) return QVariant();

  switch (index.column()) {
    case 0:
      return b->getTriggerShown();
    case 1:
      return b->getTriggerReal();
    case 2:
    {
      Keyboard::ButtonType type = b->getValueType();
      return ((type == Keyboard::TextButton) ? i18n("Text") : i18n("Shortcut"));
    }
    case 3:
      return b->getValue();
  }

  return QVariant();
}


QVariant KeyboardTab::headerData(int column, Qt::Orientation orientation,
int role) const
{
  if (m_isNull) return QVariant();

  if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
    switch (column) {
      case 0:
        return i18nc("Name of the key", "Name");
      case 1:
        return i18nc("Trigger of the key", "Trigger");
      case 2:
        return i18nc("Type of the key", "Type");
      case 3:
        return i18nc("Value of the key", "Value");
    }
  }

  //default
  return QVariant();
}


Qt::ItemFlags KeyboardTab::flags(const QModelIndex &index) const
{
  if (m_isNull || !index.isValid())
    return 0;

  return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}


QModelIndex KeyboardTab::index(int row, int column,
const QModelIndex &parent) const
{
  if (m_isNull || !hasIndex(row, column, parent) || parent.isValid())
    return QModelIndex();

  return createIndex(row, column, (void*) buttonList.at(row));
}


QModelIndex KeyboardTab::parent(const QModelIndex &index) const
{
  Q_UNUSED(index)
    return QModelIndex();
}


int KeyboardTab::rowCount(const QModelIndex &parent) const
{
  if (m_isNull) return 0;

  Q_UNUSED(parent)
    return buttonList.count();
}


int KeyboardTab::columnCount(const QModelIndex &parent) const
{
  if (m_isNull) return 0;

  Q_UNUSED(parent)
    return 4;
}


QDomElement KeyboardTab::serialize(QDomDocument *doc)
{
  QDomElement tabElem = doc->createElement("tab");
  tabElem.setAttribute("name", tabName);

  foreach (KeyboardButton *button, buttonList) {
    tabElem.appendChild(button->serialize(doc));
  }

  return tabElem;
}

KeyboardTab::~KeyboardTab()
{
  qDeleteAll(buttonList);
}
