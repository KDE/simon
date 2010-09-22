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

#include "command.h"
#include <simoninfo/simoninfo.h>
#include <QVariant>
#include <QDomElement>
#include <QDomDocument>
#include <KDebug>

/**
 * \brief Should this command be executed?
 *
 * This method tells the CommandManager if the command matches the current conditions (state and recognition result).
 *
 * The default implementation checks if the commandManagerState is the same as the #boundState and if the recognized trigger
 * is the same as the commands trigger name.
 *
 * \param commandManagerState The state of the CommandManager
 * \param trigger The recognized sentence
 * \return True, if the Command should be executed.
 */
bool Command::matches(int commandManagerState, const QString& trigger)
{
  kDebug() << "Commandmanager state: " << commandManagerState << "Command bound to: " << boundState << trigger << getTrigger();
  if (commandManagerState != boundState)
    return false;

  return (trigger.compare(this->triggerName, Qt::CaseInsensitive) == 0);
}


/**
 * \brief Executes the command
 *
 * Depending on your #announce flag, this will display visual notification of the command that is being executed.
 *
 * This will switch the parent CommandManager to the state defined in #switchToState.
 *
 * This function should not be overwritten directly. It calls triggerPrivate() which is where you should
 * implement the actions of your specific subclass.
 *
 * \param state Reference parameter: The state of the parent CommandManager
 * \return If your work was done, return true. Otherwise return false (not correct state, etc.). Have a look at the trigger()
 * 	method of the CommandManager class for more details.
 *
 * \sa triggerPrivate()
 */
bool Command::trigger(int* state)
{
  if (announce) {
    KIcon commandIcon = getIcon();
    SimonInfo::showMessage(getTrigger(), 2500, &commandIcon);
  }
  if (state)
    *state = switchToState;
  return triggerPrivate(state);
}


/**
 * \brief Exports the user relevant parts of the command
 *
 * This is used to export user relevant information about the command.
 *
 * Do not overwrite this directly but use getValueMapPrivate() instead!
 *
 * \return Key -> Value pairs of "field" -> "value" information; For example: "Description" -> "Bla"
 * \sa getValueMapPrivate()
 */
const QMap<QString,QVariant> Command::getValueMap() const
{
  QMap<QString,QVariant> out = getValueMapPrivate();
  out.insert(i18n("Description"), getDescription());
  return out;
}


/**
 * \brief Serializes the internal state of the command to an XML element
 *
 * This serializes the shared information that each plugin has. The plugin specific stuff
 * is handled in \ref serializePrivate(). Overwrite \ref serializePrivate() instead of this
 * function.
 *
 * \param doc The parent document
 * \return The serialized XML element
 * \sa deSerialize(), serializePrivate()
 */
QDomElement Command::serialize(QDomDocument *doc)
{
  QDomElement commandElem = doc->createElement("command");

  QDomElement name = doc->createElement("name");
  name.appendChild(doc->createTextNode(triggerName));
  QDomElement icon = doc->createElement("icon");
  icon.appendChild(doc->createTextNode(iconSrc));
  QDomElement descriptionElem = doc->createElement("description");
  descriptionElem.appendChild(doc->createTextNode(description));
  QDomElement stateElem = doc->createElement("state");
  stateElem.appendChild(doc->createTextNode(QString::number(boundState)));
  QDomElement newStateElem = doc->createElement("newState");
  newStateElem.appendChild(doc->createTextNode(QString::number(switchToState)));
  QDomElement announceElem = doc->createElement("announce");
  announceElem.appendChild(doc->createTextNode(announce ? "1" : "0"));

  commandElem.appendChild(name);
  commandElem.appendChild(icon);
  commandElem.appendChild(descriptionElem);
  commandElem.appendChild(stateElem);
  commandElem.appendChild(newStateElem);
  commandElem.appendChild(announceElem);

  return serializePrivate(doc, commandElem);
}


/**
 * \brief Deserializes the internal state of the command from the given XML element
 *
 * Loads the attributes of the command from the given XML element. The element passed to
 * this function has once been created using \ref serialize().
 *
 * This function only handles the common options that all commands share. The plugin
 * specific attributes are dealt with in \ref deSerializePrivate(). Overwrite
 * deSerializePrivate() instead of this method.
 *
 * \param elem The element to deserialize
 * \return Success
 * \sa serialize(), deSerializePrivate()
 */
bool Command::deSerialize(const QDomElement& elem)
{
  QDomElement name = elem.firstChildElement();
  QDomElement icon = name.nextSiblingElement();
  QDomElement descriptionElem = icon.nextSiblingElement();
  QDomElement stateElem = descriptionElem.nextSiblingElement();
  QDomElement newStateElem = stateElem.nextSiblingElement();
  QDomElement announceElem = newStateElem.nextSiblingElement();
  triggerName = name.text();
  iconSrc = icon.text();
  description = descriptionElem.text();
  boundState = stateElem.text().toInt();
  switchToState = newStateElem.text().toInt();
  announce = (announceElem.text().toInt() == 1);

  return deSerializePrivate(elem);
}
