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

#include "command.h"
#include <simoninfo/simoninfo.h>
#include <QVariant>
#include <QDomElement>
#include <QDomDocument>
#include <KDebug>
#include <KLocalizedString>
#include <qvarlengtharray.h>


/**
 * \brief Parses the given arguments
 * 
 * Static function to parse command parameters; You can ues this from your commandmanager
 * implementation if you are dealing with arguments there
 * 
 * This is called from \sa matches(int, const QString&) to parse the arguments
 * 
 * \param input The recognized text
 * \param scheme The scheme of the commands (the general template)
 * \param arguments Output parameter: The found arguments
 * \return true if the input could be matched against the scheme; Returned arguments are invalid if this is false
 */
bool Command::parseArguments(const QString& input, const QString& scheme, QStringList& arguments)
{
  kDebug() << "Command trigger: " << scheme << " provided trigger: " << input;
  arguments.clear();
  QStringList splitList = scheme.split(QRegExp("%\\d+"));
  kDebug() << "Split list: " << splitList;
  QString callTrigger = input;
  for (int i=0; i < splitList.count()-1; i++)
  {
    int partLength = splitList[i].length();
    bool isString = (splitList[i].at(partLength-1) == '%');
    if (isString) partLength--;
    
    if (callTrigger.left(partLength) != splitList[i].left(partLength))
      return false;
    
    callTrigger.remove(0, partLength);
    
    int nextIndex;
    if (!isString)
      nextIndex = callTrigger.indexOf(QRegExp("( |$)"));
    else {
      QString nextString = splitList[i+1];
      if ((i == splitList.count()-2) && nextString.isEmpty()) // last run
        nextIndex = callTrigger.length();
      else
        nextIndex = callTrigger.indexOf(nextString);
    }
    
    if (nextIndex == -1)
      return false;
    QString thisParameter = callTrigger.left(nextIndex);
    callTrigger.remove(0, thisParameter.length());
    arguments << thisParameter;
  }
  kDebug() << "Got parameter: " << arguments;
  return (callTrigger.compare(splitList[splitList.count()-1]) == 0);
}


/**
 * \brief Should this command be executed?
 *
 * This method tells the CommandManager if the command matches the current conditions (state and recognition result).
 *
 * The default implementation checks if the commandManagerState is the same as the #boundStates and if the recognized trigger
 * is the same as the commands trigger name.
 *
 * \param commandManagerState The state of the CommandManager
 * \param trigger The recognized sentence
 * \return True, if the Command should be executed.
 */
bool Command::matches(int commandManagerState, const QString& trigger)
{
  if (!boundStates.contains(commandManagerState))
    return false;

  if (!triggerName.contains(QRegExp("%?%\\d+")))
    return (trigger.compare(this->triggerName, Qt::CaseInsensitive) == 0);
  
  bool succ = Command::parseArguments(trigger, getTrigger(), m_currentParameters);
  
  int filledArguments = 0;
  foreach (const QString& param, m_currentParameters)
    if (!param.isEmpty())
      filledArguments++;
    
  return (succ && (filledArguments >= neededParameterCount()));
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
 * \param silent Set this to true to hide the popup
 * \param state Reference parameter: The state of the parent CommandManager
 * \return If your work was done, return true. Otherwise return false (not correct state, etc.). Have a look at the trigger()
 * 	method of the CommandManager class for more details.
 *
 * \sa triggerPrivate()
 */
bool Command::trigger(int* state, bool silent)
{
  if (announce) {
    KIcon commandIcon = getIcon();
    if (!silent)
      SimonInfo::showMessage(getParsedTrigger(), 2500, &commandIcon);
  }
  if (state)
    *state = switchToState;
  return triggerPrivate(state);
}

/**
 * \brief Returns the trigger name of this command with parsed arguments
 * \return The parsed (current) trigger name
 */
QString Command::getParsedTrigger() const
{
  QString out = getTrigger();
  for (int i=0; i < m_currentParameters.count(); i++) {
    QString regExp = QString("%?%")+QString::number(i+1);
    kDebug() << regExp;
    out.replace(QRegExp(regExp), m_currentParameters[i]);
  }
  kDebug() << "Out: " << out;
  return out;
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
  
  QString states;
  foreach (int elem, boundStates)
    states += QString::number(elem)+',';
  states = states.left(states.length()-1); //remove last ,
  
  stateElem.appendChild(doc->createTextNode(states));
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
  
  boundStates.clear();
  QString states = stateElem.text();
  foreach (const QString& s, states.split(','))
    boundStates << s.toInt();
  
  switchToState = newStateElem.text().toInt();
  announce = (announceElem.text().toInt() == 1);

  return deSerializePrivate(elem);
}

/**
 * \brief Accesser method for @sa m_currentParameters
 * \return The parameters of the last matches() call
 */
QStringList Command::currentArguments() const
{
  return m_currentParameters;
}
