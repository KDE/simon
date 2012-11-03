/*
 *   Copyright (C) 2009 Grasch Peter <peter.grasch@bedahr.org>
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

#include "keyboardsetcontainer.h"
#include <QString>
#include <QList>
#include <QFile>
#include <KDebug>
#include <KStandardDirs>

KeyboardSetContainer::KeyboardSetContainer()
{
}


QStringList KeyboardSetContainer::getAvailableSets()
{
  QStringList sets;
  foreach (KeyboardSet *set, setList)
    sets << set->getSetName();
  return sets;
}


KeyboardSet* KeyboardSetContainer::findSet(const QString& setName)
{
  foreach (KeyboardSet *set, setList) {
    if (set->getSetName() == setName)
      //found set
      return set;
  }
  return 0;
}


QStringList KeyboardSetContainer::getAvailableTabs(const QString& setName)
{
  KeyboardSet *set = findSet(setName);
  if (!set) return QStringList();

  return set->getAvailableTabs();
}


KeyboardTab* KeyboardSetContainer::getTab(const QString& setName, const QString& tab)
{
  KeyboardSet *set = findSet(setName);
  if (!set) return 0;

  return set->getTab(tab);
}


void KeyboardSetContainer::clear()
{
  qDeleteAll(setList);
  setList.clear();
}


bool KeyboardSetContainer::loadFromFile(const QString& path)
{
  QDomDocument doc("keyboardsets");
  QFile file(path);
  if (!file.open(QIODevice::ReadOnly))
    return false;
  if (!doc.setContent(&file)) {
    file.close();
    return false;
  }
  file.close();
  return parseElement(doc.documentElement());
}


bool KeyboardSetContainer::deSerialize(const QDomElement& setsElem)
{
  if (setsElem.isNull()) {
    kDebug() << "Should load defaults";
    return loadFromFile(KStandardDirs::locate("appdata", "conf/keyboardsets.xml"));
  }

  return parseElement(setsElem);
}


bool KeyboardSetContainer::parseElement(const QDomElement& setsElem)
{
  if (setsElem.isNull())
    return false;

  QDomElement setElem = setsElem.firstChildElement();
  while (!setElem.isNull()) {
    KeyboardSet *set = new KeyboardSet(setElem);
    if (set->isNull())
      delete set;
    else
      setList << set;
    setElem = setElem.nextSiblingElement();
  }
  return true;

}


QDomElement KeyboardSetContainer::serialize(QDomDocument *doc)
{
  QDomElement setsElem = doc->createElement("keyboardSets");

  foreach (KeyboardSet *set, setList) {
    QDomElement setElem = set->serialize(doc);
    setsElem.appendChild(setElem);
  }

  return setsElem;
}


bool KeyboardSetContainer::createSet(const QString& name)
{
  if (findSet(name)) return false;                //not unique

  setList.append(new KeyboardSet(name));
  return true;
}


bool KeyboardSetContainer::editSet(const QString& curName, const QString& newName)
{
  if (findSet(newName)) return false;             //not unique

  KeyboardSet *set = findSet(curName);
  if (!set) return false;
  set->setSetName(newName);
  return true;
}


bool KeyboardSetContainer::deleteSet(const QString& name)
{
  KeyboardSet *set = findSet(name);
  if (!set) return false;

  setList.removeAll(set);
  delete set;

  return true;
}


bool KeyboardSetContainer::createTab(const QString& setName, const QString& name)
{
  KeyboardSet *set = findSet(setName);
  if (!set) return false;

  return set->createTab(name);
}


bool KeyboardSetContainer::editTab(const QString& setName, const QString& curName, const QString& newName)
{
  KeyboardSet *set = findSet(setName);
  if (!set) return false;

  return set->editTab(curName, newName);
}


bool KeyboardSetContainer::deleteTab(const QString& setName, const QString& name)
{
  KeyboardSet *set = findSet(setName);
  if (!set) return false;

  return set->deleteTab(name);
}


bool KeyboardSetContainer::moveTabUp(const QString& setName, const QString& tabName)
{
  KeyboardSet *set = findSet(setName);
  if (!set) return false;

  return set->moveTabUp(tabName);
}


bool KeyboardSetContainer::moveTabDown(const QString& setName, const QString& tabName)
{
  KeyboardSet *set = findSet(setName);
  if (!set) return false;

  return set->moveTabDown(tabName);
}


bool KeyboardSetContainer::addButton(const QString& setName, const QString& tab, KeyboardButton *button)
{
  KeyboardSet *set = findSet(setName);
  if (!set) return false;

  return set->addButton(tab, button);
}


bool KeyboardSetContainer::deleteButton(const QString& setName, const QString& tab, KeyboardButton *button)
{
  KeyboardSet *set = findSet(setName);
  if (!set) return false;

  return set->deleteButton(tab, button);

}


bool KeyboardSetContainer::moveButtonUp(const QString& setName, const QString& tab, KeyboardButton *button)
{
  KeyboardSet *set = findSet(setName);
  if (!set) return false;

  return set->moveButtonUp(tab, button);
}


bool KeyboardSetContainer::moveButtonDown(const QString& setName, const QString& tab, KeyboardButton *button)
{
  KeyboardSet *set = findSet(setName);
  if (!set) return false;

  return set->moveButtonDown(tab, button);
}


KeyboardSetContainer::~KeyboardSetContainer()
{
  kDebug() << "Deleting set container!";
  qDeleteAll(setList);
}
