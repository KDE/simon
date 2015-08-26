/*
 *   Copyright (C) 2008 Peter Grasch <peter.grasch@bedahr.org>
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

#include "dialogcommand.h"
#include <QDomDocument>
#include <QDomElement>
#include <QVariant>
#include <QIcon>
#include <KI18n/klocalizedstring.h>
#include <QDebug>

const QString DialogCommand::staticCategoryText()
{
  return i18n("Dialog");
}


const QString DialogCommand::getCategoryText() const
{
  return DialogCommand::staticCategoryText();
}


const QIcon DialogCommand::staticCategoryIcon()
{
  return QIcon::fromTheme("im-user");
}


const QIcon DialogCommand::getCategoryIcon() const
{
  return DialogCommand::staticCategoryIcon();
}


const QMap<QString,QVariant> DialogCommand::getValueMapPrivate() const
{
  QMap<QString,QVariant> out;
  //out.insert(i18nc("The dialog file to launch", "Dialog"), getDialog());
  return out;
}


bool DialogCommand::triggerPrivate(int *state)
{
  qDebug() << "Triggering...";
  return true;
}


QDomElement DialogCommand::serializePrivate(QDomDocument *doc, QDomElement& commandElem)
{

  return commandElem;
}


bool DialogCommand::deSerializePrivate(const QDomElement& commandElem)
{

  return true;
}


STATIC_CREATE_INSTANCE_C(DialogCommand);
