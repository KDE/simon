/*
 *   Copyright (C) 2008 Phillip Goriup <goriup@simon-listens.org>
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

#include "guievents.h"
#include <simonlogging/logger.h>
#include <QRegExp>
#include <QStringList>
#include <KI18n/klocalizedstring.h>
#include <KLocale>

GuiEvents::GuiEvents(QObject *parent) : QObject(parent)
{
  guiItems = new GuiAction;
}


GuiEvents::~GuiEvents()
{
  qDeleteAll(*guiItems);
  delete(guiItems);
}


void GuiEvents::registerControl(QString trigger, QObject* receiver, const char* slot)
{
  actionValues *realAction = new actionValues;
  realAction->receiver = receiver;
  realAction->slot = slot;
  uniqueKey *key = new uniqueKey;

  key->trigger = trigger.trimmed();

  guiItems->insert(key, realAction);
}


void GuiEvents::doAction(QString action, QObject * parentItem)
{
  Q_UNUSED(parentItem);

  QRegExp expression(i18n("Calendar *"));
  expression.setPatternSyntax(QRegExp::Wildcard);

  QList<uniqueKey*> keyList = guiItems->keys();
  int i = 0;
  int index = 0;
  uniqueKey *key;
  while(i < keyList.count()-1) {
    keyList.at(i)->trigger = keyList.at(i)->trigger.trimmed();

    if((keyList.at(i)->trigger) == action) {
      key = keyList.at(i);
      i = keyList.count()-1;
      index = 1;
    }
    //this else contains the code to available the controlling of SimonCalendarWidget
    else if ((expression.exactMatch(action)) && (expression.exactMatch((keyList.at(i)->trigger)))) {
      QStringList list = action.split(' ');

      int count = list.at(list.count()-1).count();

      QString temp = action;
      temp = temp.remove(action.count()-count,count);
      temp = temp.trimmed();
      if(temp == keyList.at(i)->trigger) {
        key = keyList.at(i);
        i = keyList.count()-1;
        index = 1;
      }
    }
    i++;
  }

  if (index == 0) {
    return;
  }

  //connects to the slot that is saved in the QHash with the given parameters
  connect(this, SIGNAL(dummy(QString)), guiItems->value(key)->receiver, guiItems->value(key)->slot);
  emit this->dummy(action);
  disconnect(this, SIGNAL(dummy(QString)),0,0);

}
