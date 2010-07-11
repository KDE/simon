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

#include "commandmanager.h"
#include "voiceinterfacecommand.h"
#include "voiceinterfacecommandtemplate.h"
#include "createvoiceinterfacecommandwidget.h"
#include "commandconfiguration.h"
#include <KLocalizedString>
#include <simonscenarios/scenario.h>
#include <QAction>
#include <QDomElement>
#include <QDomDocument>

/**
 * \brief Returns the icon of the plugin
 *
 * The default implementation creates a KIcon() from using the
 * icon name returned by iconSrc().
 *
 * \sa iconSrc() Overwrite iconSrc() instead of this  function
 * \return Icon of the plugin
 */
const KIcon CommandManager::icon() const
{
  QString iconS = iconSrc();

  if (iconS.isNull())
    return KIcon();

  return KIcon(iconS);
}


/**
 * \brief Triggers the commands with the given trigger
 *
 * Whenever simon recognizes some input, it will call the trigger() method of the
 * configured commandmanagers in the order in which they are sorted within they are
 * sorted within the scenario.
 *
 * Each CommandManager will then process the recognition result, execute associated
 * commands and return a boolean value that tells the action collection if it should
 * ask the next CommandManager.
 *
 * As long as the CommandManagers return false, the result will be passed on to the
 * next manager.
 *
 * For example: If you want your CommandManager to be "transparent" (let everything through)
 * let it always return false. In contrast, a CommandManager whose trigger() method
 * always returns true, will "eat" any input it gets.
 *
 * The default implementation will run through all the commands in the #commands list
 * and execute them if they match the current state and the given trigger (determined
 * by calling Command::matches() on them). The first command that reports that it
 * matches these parameters will get executed (Command::trigger()) and the function
 * will return true.
 *
 * If you want to overwrite this function for any reason keep in mind that the
 * voice interface commands installed through \ref installInterfaceCommand and
 * \ref installListInterfaceCommand are also processed as command instances internally
 * so if you need to change the trigger process and want to use voice interface commands
 * at the same time you should probably call the base class implementation from your
 * own trigger method.
 *
 * \param triggerName The recognition result (minus the plugin trigger)
 *
 * \return True if the CommandManager accepts the result
 */
bool CommandManager::trigger(const QString& triggerName)
{
  if (!commands) return false;

  foreach (Command* c, *commands) {
    if (c->matches(m_currentState, triggerName))
      if (c->trigger(&m_currentState))
        return true;
  }

  return false;
}


/**
 * \brief Low level access to the selected recognition result
 *
 * Re-implement this if you need access to the raw recognition result instead of
 * just the recognized sentence as a string.
 *
 * The default implementation just calls \ref trigger() with the recognized sentence.
 *
 * \param recognitionResult The recognition result recieved from the recognizer
 * \return True if the CommandManager accepts the result
 *
 * \sa trigger() for details on what happens when the recognizer recognizes something
 */
bool CommandManager::processResult(const RecognitionResult& recognitionResult)
{
  return trigger(recognitionResult.sentence());
}


/**
 * \brief Adds the given command to the plugins #commands
 *
 * This will handle VoiceInterfaceCommand instances appropriately and
 * pass everything else on to addCommandPrivate()
 *
 * \param command The command to add
 * \return True if the command was processed (added)
 */
bool CommandManager::addCommand(Command *command)
{
  VoiceInterfaceCommand *c = dynamic_cast<VoiceInterfaceCommand*>(command);
  if (c) {
    if (!commands)
      commands = new CommandList();
    bool accept = appendCommand(command);
    adaptUi();
    return accept;
  }

  return addCommandPrivate(command);
}


/**
 * \brief Adds the command to the list if the CommandManager reports that it can handle it
 *
 * Uses \ref shouldAcceptCommand() to determine if it should add the command
 *
 * \param command The command to add
 * \return True if the command was added
 */
bool CommandManager::addCommandPrivate(Command *command)
{
  if (shouldAcceptCommand(command))
    return appendCommand(command);

  return false;
}


/**
 * \brief Appends the command to the #commands list
 * \param c The command to add
 * \return True if the commands was added
 *
 * Use this instead of directly adding the command to the list! This will take care
 * of the QAbstractItemModel housekeeping.
 */
bool CommandManager::appendCommand(Command *c)
{
  if (!commands)
    commands = new CommandList();

  beginInsertRows(QModelIndex(), commands->count(), commands->count());
  c->setParent(this);                             //assign parent
  *commands << c;
  endInsertRows();
  return parentScenario->save();
}


/**
 * \brief Creates and adds a new VoiceInterfaceCommandTemplate with the given parameters
 *
 * Using this and the \ref installListInterfaceCommand() function you can create a connection from
 * the users voice to a SLOT of any QObject. simon will handle the configuration and display the
 * commands to the user.
 *
 * This will install a template describing the desired action. When loading the default values, each
 * template will be turned into one instance of VoiceInterfaceCommand (defaulting to the values described
 * in the template). The user can add new versions (with different triggers for example) of these commands,
 * edit them and remove them just like any other commands.
 *
 * \param object The object that has the slot that will be called when the command is triggered
 * \param slot The slot to execute
 * \param actionName Translated, descriptive name of what will happen if the voice command is triggered
 * \param iconSrc Icon name of the template
 * \param description Detailed description of the template
 * \param announce If this is true simon will pop up a information whenever the trigger is triggered
 * \param showIcon If this is true simon will try to display the icon (created with the given iconSrc)
 * 			on the receiving object
 * \param state The state in which the template (and its resulting commands) should be considered for execution.
 * 		Default: SimonCommand::DefaultState.
 * \param newState When the command is triggered, the CommandManager should switch to this state. Default:
 * 			SimonCommand::DefaultState.
 * \param defaultVisibleTrigger This is the text that will end up on the receiving object. Per default
 * 		this will be created from the actionName.
 * \param id Unique identifier of this template. Will be used to commands with their templates during
 * 		deserializsation. Per default, the method will try to resolve this from the objectName
 * 		of the given receiving object. If the id is already used, the method will make sure
 * 		that it is unique so do not worry. However, the method will not accept empty ids so
 * 		make sure you either provide one or set an object name for your receiver.
 *
 * \return True if the template was installed
 * \sa adaptUi() installListInterfaceCommand()
 */
bool CommandManager::installInterfaceCommand(QObject* object, const QString& slot,
const QString& actionName, const QString& iconSrc,
const QString& description, bool announce, bool showIcon,
int state, int newState, const QString& defaultVisibleTrigger,
QString id)
{
  if (id.isEmpty() && object)
    id = object->objectName();

  if (id.isEmpty()) {
    kDebug() << "id is empty. panicking!";
    return false;
  }

  //make id unique
  bool unique;
  do {
    unique = true;
    foreach (VoiceInterfaceCommandTemplate *t, voiceInterfaceCommandTemplates) {
      if (t->id() == id) {
        unique = false;
        break;
      }
    }
    if (!unique)
      id += '_';
  } while (!unique);

  VoiceInterfaceCommandTemplate *templ = new VoiceInterfaceCommandTemplate(id, actionName, iconSrc, description,
    state, newState, announce, showIcon, defaultVisibleTrigger);

  templ->assignAction(object, slot);
  voiceInterfaceCommandTemplates.append(templ);

  if (commands) {
    foreach (Command *c, *commands) {
      VoiceInterfaceCommand *iC = dynamic_cast<VoiceInterfaceCommand*>(c);
      if (!iC) continue;
      if (iC->id() == id)
        iC->assignAction(this, object, slot);
    }
  }
  return true;
}


/**
 * \brief Creates and adds a new VoiceInterfaceCommandTemplate for a list option with the given parameters
 *
 * Using this and the \ref installInterfaceCommand() function you can create a connection from
 * the users voice to a SLOT of any QObject. simon will handle the configuration and display the
 * commands to the user.
 *
 * This will install a template describing the desired action. When loading the default values, each
 * template will be turned into one instance of VoiceInterfaceCommand (defaulting to the values described
 * in the template). The user can add new versions (with different triggers for example) of these commands,
 * edit them and remove them just like any other commands.
 *
 * This is a specialized method that should only be used for controlling lists (like the ones provided
 * by a CommandListWidget). For lists, you do not need (and actually shouldn't without a good reason)
 * to specify all parameters because the global action configuration (and subsequently the scenario wide
 * list configuration) will "fill in the blanks".
 *
 * \param element Element identifier. Describes which element in the list this refers to.
 * \param object The object that has the slot that will be called when the command is triggered
 * \param slot The slot to execute
 * \param id Unique identifier of this template. Will be used to commands with their templates during
 * 		deserializsation. Per default, the method will try to resolve this from the objectName
 * 		of the given receiving object. If the id is already used, the method will make sure
 * 		that it is unique so do not worry. However, the method will not accept empty ids so
 * 		make sure you either provide one or set an object name for your receiver.
 * \param description Detailed description of the template. Default: Fetch from scenario configuration
 * \param state The state in which the template (and its resulting commands) should be considered for execution.
 * 		Default: SimonCommand::DefaultState.
 * \param newState When the command is triggered, the CommandManager should switch to this state. Default:
 * 			SimonCommand::DefaultState.
 * \param actionName Translated, descriptive name of what will happen if the voice command is triggered
 * 			Default: Fetch from scenario configuration.
 * \param iconSrc Icon name of the template. Default: Fetch from scenario configuration.
 * \param announce If this is true simon will pop up a information whenever the trigger is triggered.
 * 		   Default: False.
 * \param showIcon If this is true simon will try to display the icon (created with the given iconSrc)
 * 			on the receiving object. Default: false; Will be fetched from the scenario
 * 			configuration if iconSrc is null (default).
 * \param defaultVisibleTrigger This is the text that will end up on the receiving object. Per default
 * 		this will be fetched from the scenario configuration.
 *
 * \return True if the template was installed
 * \sa adaptUi() installListInterfaceCommand()
 */
bool CommandManager::installListInterfaceCommand(CommandListElements::Element element,
QObject* object, const QString& slot, QString id,
QString description, int state,
int newState, QString actionName, QString iconSrc,
bool announce, bool showIcon,
QString defaultVisibleTrigger)
{
  if (m_actionCollection && (actionName.isNull() || iconSrc.isNull() || description.isNull() || defaultVisibleTrigger.isNull())) {
    //get from scenario global listconfiguration
    QHash<CommandListElements::Element, VoiceInterfaceCommand*> standardAdaption = m_actionCollection->getListInterfaceCommands();

    QList<VoiceInterfaceCommand*> adaptionCommands = standardAdaption.values(element);

    QStringList triggers;
    foreach (VoiceInterfaceCommand* c, adaptionCommands)
      triggers << c->getTrigger();

    if (iconSrc.isNull()) {
      iconSrc = adaptionCommands[0]->getIconSrc();
      showIcon = adaptionCommands[0]->showIcon();
    }
    if (description.isNull())
      description = adaptionCommands[0]->getDescription();

    if (defaultVisibleTrigger.isNull())
      defaultVisibleTrigger = adaptionCommands[0]->visibleTrigger();

    if (actionName.isNull()) {
      bool returns = true;
      foreach (const QString& trigger, triggers)
        returns &= installInterfaceCommand(object, slot, trigger, iconSrc, description,
        announce, showIcon, state, newState, defaultVisibleTrigger, id);
      return returns;
    }
  }

  return installInterfaceCommand(object, slot, actionName, iconSrc, description,
    announce, showIcon, state, newState, defaultVisibleTrigger, id);
}


/**
 * \brief Returns the CreateCommandWidget for the VoiceInterfaceCommands
 *
 * If the plugin has VoiceInterfaceCommandTemplates (#voiceInterfaceCommandTemplates), this creates and
 * returns the CreateVoiceInterfaceCommandWidget. Otherwise, a 0 pointer is returned.
 *
 * \warning The caller is responsible for deleting the widget when it is no longer needed.
 *
 * \return A pointer to a new CreateVoiceInterfaceCommandWidget
 */
CreateCommandWidget* CommandManager::getCreateVoiceInterfaceCommandWidget(QWidget *parent)
{
  if (voiceInterfaceCommandTemplates.isEmpty())
    return 0;                                     //no voice interface actions

  return new CreateVoiceInterfaceCommandWidget(this, parent);
}


/**
 * \brief Adhere to the given base font
 * Reimplement this method in your derived classes if you implement a graphical plugin and
 * want to respect the global command plugin font.
 *
 * The default implementation does nothing.
 *
 * \param font The configured default font
 */
void CommandManager::setFont(const QFont& font)
{
  //reimplement this when you use graphical widgets in your
  //command manager
  Q_UNUSED(font);
}


/**
 * \brief Restores the managers internal state from the given XML Element
 *
 * Calls deSerializeConfig() on the "config" child element and
 * deSerializeCommands() on the "commands" child element and adapts the
 * user interface afterwards.
 *
 * \param elem The input element. Might be null if the manager was never loaded before
 * \return Success
 *
 * \sa deSerializeConfig()
 * \sa deSerializeCommands()
 * \sa adaptUi()
 */
bool CommandManager::deSerialize(const QDomElement& elem)
{
  QDomElement configElem = elem.firstChildElement("config");
  if (!deSerializeConfig(configElem)) {
    kDebug() << "Could not load config of plugin";
    return false;
  }
  QDomElement commandsElem = elem.firstChildElement("commands");
  if (!deSerializeCommands(commandsElem)) {
    kDebug() << "Could not load commands of plugin";
    return false;
  }

  adaptUi();
  return true;
}


/**
 * \brief Serializes the manager state to a XML Element.
 *
 * \param doc Parent document
 * \return XML element (tagname: "plugin")
 *
 * \sa serializeConfig()
 * \sa serializeCommands()
 */
QDomElement CommandManager::serialize(QDomDocument *doc)
{
  QDomElement pluginElem = doc->createElement("plugin");

  pluginElem.appendChild(serializeConfig(doc));
  pluginElem.appendChild(serializeCommands(doc));

  return pluginElem;
}


/**
 * \brief Load config from given XML Element
 *
 * As you constructor should be as lean as possible (cheap), you can set up the command manager
 * in deSerializeConfig(). It is also a good idea to install your voice interface templates as this
 * will be called before \ref deSerializeCommands().
 *
 * The given XML element will be null (isNull() returns true) when the command manager is loaded
 * the first time so set up any default values you might need when it is.
 *
 * The default implementation does nothing! It does NOT pass the element back to #config because
 * in a perfect scenario #config should be created in this method (not in the constructor!).
 *
 * \param elem Configuration element. (Elements returned from serializeConfig() will be passed back
 * 		to deSerializeConfig() when the CommandManager is loaded the next time.)
 *
 * \return True if the config was successfully deserialized.
 */
bool CommandManager::deSerializeConfig(const QDomElement& elem)
{
  Q_UNUSED(elem);
  return true;
}


/**
 * \brief Serializes the configuration of the plugin into a XML Element
 *
 * \warning The tag name of the returned element should always be "config"
 *
 * The default configuration will return the element returned by the serialize() method
 * of your CommandConfiguration object stored in #config if #config is not a 0 pointer.
 *
 * If #config is null, this returns an empty Element.
 *
 * \param doc Parent document
 * \return The XML element
 */
QDomElement CommandManager::serializeConfig(QDomDocument *doc)
{
  if (config)
    return config->serialize(doc);

  return doc->createElement("config");
}


/**
 * \brief Serializes the commands into a XML Element
 *
 * This will serialize the #commands into one XML Element with the tag name
 * "commands".
 *
 * The default implementation will handle arbitrary commands so you shouldn't
 * need to overwrite this method. If you do, keep in mind that this method
 * also handles voice interface commands. If you store them yourself (without
 * using this function, just call their serialize() command just like with any
 * other command, but set their tag name to "voiceInterfaceCommand":
 * \code
QDomElement commandElem = c->serialize(doc);
commandElem.setTagName("voiceInterfaceCommand");
* \endcode
*
* \param doc The parent document
* \return XML Element
*/
QDomElement CommandManager::serializeCommands(QDomDocument *doc)
{
  QDomElement commandsElem = doc->createElement("commands");
  if (commands) {
    foreach (Command *c, *commands) {
      QDomElement commandElem = c->serialize(doc);
      if (dynamic_cast<VoiceInterfaceCommand*>(c))
        commandElem.setTagName("voiceInterfaceCommand");
      commandsElem.appendChild(commandElem);
    }
  }

  return commandsElem;
}


/**
 * \brief Load the command configuration from the given XML Element
 *
 * This will load the voice interface commands and pass the element on to
 * deSerializeCommandsPrivate() to handle all the other commands that the plugin might store.
 *
 * If this is the first time the plugin is loaded (the provided elem is null), the
 * default set of voiceinterface commands will be created from the templates in #voiceInterfaceCommandTemplates.
 *
 * Make sure you install all templates before this function is called or they will not have a command
 * associated with them by default. (Install the interface commands during deSerializeConfig() - it is called
 * before this method).
 *
 * \param elem The XML Element containing the command information. Might be null (isNull() returns true)
 * 	if the plugin has no configuration.
 *
 * \note If you are considering to overwrite this method: You most likely want to overwrite \ref deSerializeCommandsPrivate()
 *
 * \sa deSerializeCommandsPrivate(), installInterfaceCommand(), installListInterfaceCommand()
 * \return True if the commands could be correctly deserialized
 */
bool CommandManager::deSerializeCommands(const QDomElement& elem)
{
  if (elem.isNull()) {
    //load defaults
    bool childSucc = deSerializeCommandsPrivate(elem);

    if (!voiceInterfaceCommandTemplates.isEmpty())
      if (!commands)
        commands = new CommandList();

    foreach (VoiceInterfaceCommandTemplate *tem, voiceInterfaceCommandTemplates) {
      VoiceInterfaceCommand *com = new VoiceInterfaceCommand(this, tem);
      com->assignAction(this, tem->receiver(), tem->slot());
      com->setParent(this);
      *commands << com;
    }

    return childSucc;
  }

  ///////////// end defaults

  QDomElement command = elem.firstChildElement("voiceInterfaceCommand");

  if (commands)
    qDeleteAll(*commands);

  commands = 0;

  if (!command.isNull())                          //we need commands
    commands = new CommandList();

  while (!command.isNull()) {
    VoiceInterfaceCommand *com = VoiceInterfaceCommand::createInstance(command);

    command = command.nextSiblingElement("voiceInterfaceCommand");
    if (!com) continue;

    foreach (VoiceInterfaceCommandTemplate *tem, voiceInterfaceCommandTemplates) {
      if (tem->id() == com->id()) {
        com->assignAction(this, tem->receiver(), tem->slot());
        *commands << com;
        break;
      }
    }
  }

  if (commands)
    kDebug() << "Loaded commands: " << commands->count();

  bool succ = deSerializeCommandsPrivate(elem);

  if (!commands) return succ;

  foreach (Command *c, *commands)
    c->setParent(this);                           //assign parent

  return succ;
}


/**
 * \brief Returns the voice interface command with the given id
 * \param id Unique identifier of the command.
 * \return 0 if the command was not found, otherwise a pointer to the command
 */
VoiceInterfaceCommand* CommandManager::getVoiceInterfaceCommand(const QString& id)
{
  foreach (Command *c, *commands) {
    VoiceInterfaceCommand *iC = dynamic_cast<VoiceInterfaceCommand*>(c);
    if (!iC) continue;
    if (iC->id() == id)
      return iC;
  }
  return 0;
}


/**
 * \brief Deserializes the commands introduced by this plugin
 *
 * Overwrite this if your class uses any commands besides VoiceInterfaceCommands.
 *
 * The given XML element will contain the serialized command elements as
 * children with the tagname "command".
 *
 * The given element might contain voice interface commands as well but they do not
 * share the same "command" tagname but rather use "voiceInterfaceCommand" so you can
 * safely parse all "command" children.
 *
 * A default implementation that only needs the classnames is provided as macros:
 * Add the following to your header file in the public section:
 * \code
DEFAULT_DESERIALIZE_COMMANDS_PRIVATE_H()
\endcode
* and add the following somewhere in your implementation (replacing "x" with the classname
* of the CommandManager derived class and "y" with the name of the Command derived class).
* \code
DEFAULT_DESERIALIZE_COMMANDS_PRIVATE_C(x, y);
\endcode
*
* This will try to deserialize every "command" child element with the static createInstance()
* method of the given command class, adding every successfully deserialized command.
*
* \param elem The command XML node
* \return Success
* \sa deSerializeCommands()
*/
bool CommandManager::deSerializeCommandsPrivate(const QDomElement& elem)
{
  Q_UNUSED(elem);
  return true;
}


/**
 * \return List of gui actions (#guiActions).
 * \sa guiActions
 */
QList<QAction*> CommandManager::getGuiActions() const
{
  return guiActions;
}


/**
 * \brief Returns the CreateCommandWidget used for configuring a new/existing command
 * \author Peter Grasch
 *
 * If you want your command to be add-able (the user can add a new command of the the type of your plugin)
 * you must override this method and provide your own CreateCommandWidget.
 *
 * See the CreateCommandWidget documentation for details.
 *
 * The default implementation returns 0.
 */
CreateCommandWidget* CommandManager::getCreateCommandWidget(QWidget *parent)
{
  Q_UNUSED(parent);

  return 0;
}


/**
 * \brief Returns the configuration page of the plugin
 * \author Peter Grasch
 */
CommandConfiguration* CommandManager::getConfigurationPage()
{
  return config;
}


/**
 * \brief Preferred plugin trigger
 *
 * This returns the preferred plugin trigger of the plugin.
 *
 * Per default this returns "Computer", which means that given a command
 * "Internet" the user has to say "Computer Internet" to activate the command.
 *
 * \return Preferred trigger of the plugin
 */
const QString CommandManager::preferredTrigger() const
{
  return i18n("Computer");
}


/**
 * \brief Deletes the command from the command list (#commands)
 *
 * This will automatically save the parent scenario.
 *
 * If the deleted command was a voice interface command, the UI will be adapted.
 *
 * Use this method instead of removing commands from the list (#commands) manually
 * as this will also take care of notifying model views of the update (QAbstractItemModel
 * housekeeping).
 *
 * \param command The command to delete
 * \return True if a command was actually removed
 *
 * \sa adaptUi()
 */
bool CommandManager::deleteCommand(Command *command)
{
  if (!commands) return false;

  for (int i=0; i < commands->count(); i++) {
    if (commands->at(i) == command) {
      beginRemoveRows(QModelIndex(), i, i);
      commands->removeAt(i);
      endRemoveRows();

      if (dynamic_cast<VoiceInterfaceCommand*>(command))
        adaptUi();

      delete command;
      return parentScenario->save();
    }
  }

  return false;
}


/**
 * \brief Adapts the graphical user interface to the voice interface commands
 *
 * This will adapt the receivers - if they are graphical objects - to the voice interface
 * commands that point to them.
 *
 * Receivers that have at least one VoiceInterfaceCommandTemplate pointing to them
 * but no associated commands, will be hidden. (Widgets with neither voice commands nor templates
 * are of course not affected)
 */
void CommandManager::adaptUi()
{
  if (!commands) return;

  QHash<QObject* /*receiver*/, QStringList /*visible triggers*/> voiceCommands;
  QHash<QObject* /*receiver*/, QStringList /*triggers*/> voiceCommandsTrigger;

  foreach (Command *c, *commands) {
    VoiceInterfaceCommand *com = dynamic_cast<VoiceInterfaceCommand*>(c);
    if (!com) continue;
    if (!com->receiver()) continue;

    if (!voiceCommands.contains(com->receiver())) {
      if (com->showIcon())
        com->receiver()->setProperty("icon",  com->getIcon());
      else
        com->receiver()->setProperty("icon", QIcon());
    }

    QStringList currentCommands = voiceCommands.value(com->receiver());
    currentCommands.append(com->visibleTrigger());
    voiceCommands.insert(com->receiver(), currentCommands);

    currentCommands = voiceCommandsTrigger.value(com->receiver());
    currentCommands.append(com->getTrigger());
    voiceCommandsTrigger.insert(com->receiver(), currentCommands);
  }

  QList<QObject*> voiceCommandsKey = voiceCommands.keys();
  foreach (QObject *object, voiceCommandsKey) {
    QStringList triggers = voiceCommandsTrigger.value(object);
    QStringList visibleTriggers = voiceCommands.value(object);
    object->setProperty("toolTip", triggers.join(", "));
    // if it didn't have at least one entry it wouldn't be here
    object->setProperty("text", visibleTriggers.at(0));
  }

  //hide unused widgets
  foreach (VoiceInterfaceCommandTemplate *tem, voiceInterfaceCommandTemplates) {
    QObject *receiver = tem->receiver();
    bool shouldBeShown = voiceCommands.contains(receiver);

    QWidget *widget = dynamic_cast<QWidget*>(receiver);
    if (!widget) continue;

    widget->setVisible(shouldBeShown);
  }
}


QVariant CommandManager::data(const QModelIndex &index, int role) const
{
  if (!index.isValid() || !commands) return QVariant();

  if (role == Qt::DisplayRole)
    return commands->at(index.row())->getTrigger();

  if (role == Qt::DecorationRole)
    return commands->at(index.row())->getIcon();

  return QVariant();
}


Qt::ItemFlags CommandManager::flags(const QModelIndex &index) const
{
  if (!index.isValid())
    return 0;

  return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}


QVariant CommandManager::headerData(int column, Qt::Orientation orientation,
int role) const
{
  if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
    switch (column) {
      case 0:
        return i18n("Trigger");
    }
  }

  //default
  return QVariant();
}


QModelIndex CommandManager::parent(const QModelIndex &index) const
{
  Q_UNUSED(index);
  return QModelIndex();
}


int CommandManager::rowCount(const QModelIndex &parent) const
{
  if (!parent.isValid() && commands)
    return commands->count();
  else return 0;
}


int CommandManager::columnCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent);
  return 1;
}


QModelIndex CommandManager::index(int row, int column, const QModelIndex &parent) const
{
  if (!hasIndex(row, column, parent) || parent.isValid() || !commands)
    return QModelIndex();

  return createIndex(row, column, commands->at(row));
}


/**
 * \brief Adapts to the current greedy state
 * \param isGreedy True, if the CommandManager should be greedy, false otherwise
 *
 * This will elevate the current state in the greedy range (state = state | GreedyState)
 * or deascalate back to non-greedy states (state = state & ~GreedyState)
 *
 * \warning This is NOT equivalent to calling switchToState(DefaultState) or switchToState(GreedyState) resp.
 */
void CommandManager::setGreedyStatus(bool isGreedy)
{
  if (isGreedy)
    m_currentState |= SimonCommand::GreedyState;
  else
    m_currentState &= ~(SimonCommand::GreedyState);
}


/**
 * \brief Destructor
 *
 * - Will delete all commands if the list pointer (#commands) is not 0.
 * - Will delete the config if the #config pointer is not 0.
 * - Will delete all gui actions (#guiActions).
 * - This will also delete all VoiceInterfaceCommandTemplates (#voiceInterfaceCommandTemplates).
 *
 * \warning Beware of dangling pointers!
 */
CommandManager::~CommandManager()
{
  if (commands)
    qDeleteAll(*commands);

  if (config)
    config->deleteLater();

  foreach (QAction* action, guiActions) {
    action->deleteLater();
  }

  qDeleteAll(voiceInterfaceCommandTemplates);
}
