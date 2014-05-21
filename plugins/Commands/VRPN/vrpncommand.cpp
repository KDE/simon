/*
 *   Copyright (C) 2014 Peter Grasch <peter.grasch@bedahr.org>
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

#include "vrpncommand.h"
#include <QObject>
#include <QDomDocument>
#include <QDomElement>
#include <QVariant>
#include <KIcon>
#include <KLocalizedString>
#include <KDebug>

const QString VRPNCommand::staticCategoryText()
{
  return i18n("VRPN");
}


const QString VRPNCommand::getCategoryText() const
{
  return VRPNCommand::staticCategoryText();
}


const KIcon VRPNCommand::staticCategoryIcon()
{
  return KIcon("network-connect");
}


const KIcon VRPNCommand::getCategoryIcon() const
{
  return VRPNCommand::staticCategoryIcon();
}


const QMap<QString,QVariant> VRPNCommand::getValueMapPrivate() const
{
  QMap<QString,QVariant> out;
  //out.insert(i18nc("The vrpn file to launch", "VRPN"), getVRPN());
  return out;
}


bool VRPNCommand::triggerPrivate(int *state)
{
  kDebug() << "Triggering...";
  return true;
}


QDomElement VRPNCommand::serializePrivate(QDomDocument *doc, QDomElement& commandElem)
{

  return commandElem;
}


bool VRPNCommand::deSerializePrivate(const QDomElement& commandElem)
{

  return true;
}


STATIC_CREATE_INSTANCE_C(VRPNCommand);
