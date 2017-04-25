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
#include "vrpncommandmanager.h"
#include <QDomDocument>
#include <QDomElement>
#include <QVariant>
#include <QIcon>
#include <KLocalizedString>
#include <QDebug>

const QString VRPNCommand::staticCategoryText()
{
  return i18n("VRPN");
}


const QString VRPNCommand::getCategoryText() const
{
  return VRPNCommand::staticCategoryText();
}


const QIcon VRPNCommand::staticCategoryIcon()
{
  return QIcon::fromTheme("network-connect");
}


const QIcon VRPNCommand::getCategoryIcon() const
{
  return VRPNCommand::staticCategoryIcon();
}


const QMap<QString,QVariant> VRPNCommand::getValueMapPrivate() const
{
  QMap<QString,QVariant> out;
  out.insert(i18n("Button name"), getButton());
  QString clickModeStr;
  switch (clickMode) {
    case Press:
      clickModeStr = i18nc("Click mode description: Pressing the VRPN button", "Press");
      break;
    case Release:
      clickModeStr = i18nc("Click mode description: Releasing the VRPN button", "Release");
      break;
    case PressAndRelease:
      clickModeStr = i18nc("Click mode description: Pressing and releasing the VRPN button", "Press And Release");
      break;
    case Toggle:
      clickModeStr = i18nc("Click mode description: Toggling the VRPN button", "Toggle");
      break;
  }
  out.insert(i18nc("Describes how a button will be clicked", "Click Mode"), clickModeStr);
  return out;
}


bool VRPNCommand::triggerPrivate(int *state)
{
  Q_UNUSED(state);
  qDebug() << "Triggering..." << button;
  return static_cast<VRPNCommandManager*>(parent())->activateButton(button, clickMode);
}


QDomElement VRPNCommand::serializePrivate(QDomDocument *doc, QDomElement& commandElem)
{
  QDomElement buttonElem = doc->createElement("button");
  buttonElem.appendChild(doc->createTextNode(button));
  buttonElem.setAttribute("mode", static_cast<int>(clickMode));
  commandElem.appendChild(buttonElem);

  return commandElem;
}


bool VRPNCommand::deSerializePrivate(const QDomElement& commandElem)
{
  QDomElement buttonElem = commandElem.firstChildElement("button");
  bool okay = true;
  int clickModeInt = buttonElem.attribute("mode").toInt(&okay);
  if (!okay)
    clickModeInt = 0;
  clickMode = static_cast<ClickMode>(clickModeInt);
  button = buttonElem.text();
  return true;
}


STATIC_CREATE_INSTANCE_C(VRPNCommand);
