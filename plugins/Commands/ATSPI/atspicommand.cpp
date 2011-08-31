/*
 *   Copyright (C) 2008 Peter Grasch <grasch@simon-listens.org>
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

#include "atspicommand.h"
#include <QObject>
#include <QDomDocument>
#include <QDomElement>
#include <QVariant>
#include <KIcon>
#include <KLocalizedString>
#include <KDebug>

const QString ATSPICommand::staticCategoryText()
{
  return i18n("ATSPI");
}


const QString ATSPICommand::getCategoryText() const
{
  return ATSPICommand::staticCategoryText();
}


const KIcon ATSPICommand::staticCategoryIcon()
{
  return KIcon("help-hint");
}


const KIcon ATSPICommand::getCategoryIcon() const
{
  return ATSPICommand::staticCategoryIcon();
}


const QMap<QString,QVariant> ATSPICommand::getValueMapPrivate() const
{
  QMap<QString,QVariant> out;
  //out.insert(i18nc("The ATSPI file to launch", "ATSPI"), getATSPI());
  return out;
}


bool ATSPICommand::triggerPrivate(int *state)
{
  kDebug() << "Triggering...";
  return true;
}


QDomElement ATSPICommand::serializePrivate(QDomDocument *doc, QDomElement& commandElem)
{

  return commandElem;
}


bool ATSPICommand::deSerializePrivate(const QDomElement& commandElem)
{

  return true;
}


STATIC_CREATE_INSTANCE_C(ATSPICommand);
