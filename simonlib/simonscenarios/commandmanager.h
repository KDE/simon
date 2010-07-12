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

#ifndef SIMON_COMMANDMANAGER_H_324664B5A1614578808A52E438610EFB
#define SIMON_COMMANDMANAGER_H_324664B5A1614578808A52E438610EFB

/** \file commandmanager.h
 * \brief The file containing the CommandManager baseclass header.
 */

/**
 * \def DEFAULT_DESERIALIZE_COMMANDS_PRIVATE_H
 * \brief Default deserialization of subcommands
 *
 * This will try to deserialize every "command" child element with the static createInstance()
 * method of the given command class, adding every successfully deserialized command.
 *
 * Place this in the public section in the header file of the commandmanager subclass.
 */
#define DEFAULT_DESERIALIZE_COMMANDS_PRIVATE_H \
  bool deSerializeCommandsPrivate(const QDomElement& elem);

/**
 * \def DEFAULT_DESERIALIZE_COMMANDS_PRIVATE_C(x, y)
 * \brief Default deserialization of subcommands
 *
 * This will try to deserialize every "command" child element with the static createInstance()
 * method of the given command class, adding every successfully deserialized command.
 *
 * Place this in the source file of the commandmanager subclass replacing "x" with the classname
 * of the CommandManager derived class and "y" with the name of the Command derived class.
 */
#define DEFAULT_DESERIALIZE_COMMANDS_PRIVATE_C(x, y) \
  bool x::deSerializeCommandsPrivate(const QDomElement& elem) \
  { \
    if (elem.isNull()) return false; \
if (!commands) \
  commands = new CommandList(); \
  QDomElement commandElem = elem.firstChildElement("command"); \
  while(!commandElem.isNull()) \
{ \
  Command *c = y::createInstance(commandElem); \
  if (c) \
    commands->append(c); \
    commandElem = commandElem.nextSiblingElement("command"); \
} \
return true; \
}


#include "simonmodelmanagement_export.h"

#include "simoncommand.h"
#include "command.h"

#include <simonrecognitionresult/recognitionresult.h>
#include <simonscenariobase/scenarioobject.h>
#include <simonscenarios/commandlistelements.h>
#include <simonscenarios/actioncollection.h>
#include <QList>
#include <QObject>
#include <KIcon>
#include <QAbstractItemModel>
#include <QDomElement>
#include <QHash>

class CommandManager;
class CreateCommandWidget;
class KCModule;
class QAction;
class CommandConfiguration;
class QDomDocument;
class Scenario;
class VoiceInterfaceCommand;
class VoiceInterfaceCommandTemplate;
class ActionCollection;

/**
 *	@class CommandManager
 *	@brief Provides an abstract base-class for the command-managers
 *
 *	Every command plugin has to derive one class from CommandManager.
 *	This will be the entry point of the plugin and is the only class that can be seen from the outside.
 *
 *	Other elements (Commands, CommandConfiguration, etc.) are all exported by overriding the appropriate
 *	accessor methods defined in this base class.
 *
 *	The CommandManager also doubles as item model displaying its commands (#commands).
 *	Plugin developers are responsible for ensuring the model integrity by announcing changes in this
 *	members made after the initial call to deSerializeCommands() through appropriate QAbstractItemModel
 *	signals. The base class will handle all this automatically if you stick to the standard
 *	implementations (see addCommandPrivate(), appendCommand() and deleteCommand() for more details).
 *
 *	\sa QAbstractItemModel, Command, VoiceInterfaceCommand, CommandConfiguration
 */
class MODELMANAGEMENT_EXPORT CommandManager : public QAbstractItemModel, public ScenarioObject
{
  Q_OBJECT
  private:
    /**
     * \brief Templates of the available VoiceInterfaceCommands of the command manager
     *
     * This provides the base list of VoiceInterfaceCommandTemplates of the plugin. This member
     * is private and can not be directly modified by the base classes. Use installInterfaceCommand()
     * and installListInterfaceCommand() to modify its contents.
     *
     * You should set up these templates before deSerializeCommands() is called so install them
     * during deSerializeConfig() preferably.
     */
    QList<VoiceInterfaceCommandTemplate*> voiceInterfaceCommandTemplates;

  protected:
    /**
     * \brief Current state of the command manager
     *
     * This is a plain integer variable. Per default only the DefaultState and the GreedyState are defined
     * (see \ref SimonCommand::State) but you are free to use your own.
     *
     * The default implementation of GreedyReceiver will modify this state whenever \ref GreedyReceiver::startGreedy()
     * and \ref GreedyReceiver::stopGreedy() are called.
     */
    int m_currentState;

    /**
     * \brief The commands of the plugin
     *
     * This will contain all commands of this plugin. The easiest way to get your plugin to do something
     * is to create commands that you store in this member variable and let the default implementation
     * handle the rest.
     *
     * \warning If you access this list directly be sure to emit the appropriate QAbstractItemModel signals
     *
     * \sa addCommand(), appendCommand(), deleteCommand()
     */
    CommandList *commands;

    /**
     * \brief Plugin source id
     *
     * This contains the source id of the plugin. This identifies the plugin uniqely (in practice this is
     * the name of the .desktop file of the plugin)
     */
    QString m_source;

    /**
     * \brief GUI Menu options that your plugin supports
     *
     * If you want some part of your plugin showing up in the simon GUI (in the Commands submenu), just
     * add the appropriate actions to this list. During runtime simon will automatically retriev all
     * guiActions of all loaded command managers of the currently active scenario and display them in
     * the commands menu for easy access.
     *
     * For example, in the calculator plugin this contains an "Activate Calculator" action that displays
     * the calculator.
     */
    QList<QAction*> guiActions;

    /**
     * \brief Pointer to the parent action collection
     *
     * \warning This doesn't always need to be != 0. In fact, checking if this is 0 is a good
     * 		way to determine the plugin belongs to a scenario or not.
     *
     * Use this reference to retrieve the current scenario wide list interface command configuration if
     * you need it.
     */
    ActionCollection *m_actionCollection;

    /**
     * \brief Pointer to the command configuration
     *
     * Set this to your CommandConfiguration derived class if you need a configuration page.
     *
     * The base class will utilize the configuration (for example serialize it when needed)
     * but can of course handle a 0 value. If you, however, use a command configuration
     * make sure that this never is a dangling pointer as this will lead to unwanted results.
     *
     * \sa CommandConfiguration
     */
    CommandConfiguration *config;

    /**
     * \brief Item flags of the given model index
     *
     * Needed for a well behaved item model.
     *
     * \return Qt::ItemIsEnabled | Qt::ItemIsSelectable for valid indices
     */
    Qt::ItemFlags flags(const QModelIndex &index) const;

    /**
     * \brief Returns the header data of the given index and orientation.
     *
     * Needed for a well behaved item model.
     *
     * \return By default, simon will return the translated version of "Command" as
     * a (horizontal) header and not return any vertical headers.
     */
    QVariant headerData(int, Qt::Orientation orientation,
    int role = Qt::DisplayRole) const;

    /**
     * \brief Explicit call to QObject::parent()
     *
     * \return QObject::parent()
     */
    QObject* parent() { return QObject::parent(); }

    /**
     * \brief Parent of the given model index
     *
     * Needed for a well behaved item model.
     *
     * \param index The child index
     *
     * \return As the default implementation only allows for "flat" commands, this is always a null index
     */
    QModelIndex parent(const QModelIndex &index) const;

    /**
     * \brief How many rows does the model have
     *
     * Needed for a well behaved item model.
     *
     * \param parent The parent index whose amount of child rows should be returned
     *
     * \return The amount of commands if provided with a valid index
     */
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    /**
     * \brief Creates an index from the given values
     *
     * Needed for a well behaved item model.
     *
     * \return This function will return a null index if the given parameters are invalid or out of range.
     * 		Otherwise it will return an index which internal pointer will point to the command
     * 		at the given row in #commands
     */
    QModelIndex index(int row, int column,const QModelIndex &parent = QModelIndex()) const;

    /**
     * \brief The data of the model at the given index
     *
     * Needed for a well behaved item model.
     *
     * \return The icon / trigger of the command in #commands at the given index if the index isn't out of
     * 	range and the role is DecorationRole / DisplayRole (resp.)
     */
    virtual QVariant data(const QModelIndex &index, int role) const;

    /**
     * \brief Column count of the model
     *
     * Needed for a well behaved item model.
     *
     * \return The default implementation returns 1 (only a flat command structure is displayed)
     */
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;

    void adaptUi();

    virtual bool appendCommand(Command *com);

    /**
     * \brief Can this CommandManager handle the given command?
     *
     * Reimplement this in your derived class if you handle commands
     * of a custom Command derived type.
     *
     * The default implementation is false.
     *
     * \sa addCommandPrivate()
     *
     * \return True if the CommandManager can process the command
     */
    virtual bool shouldAcceptCommand(Command *com) {
      Q_UNUSED(com);
      return false;
    }

  public:
    /**
     * \brief Return the localized name of this plugin
     */
    virtual const QString name() const=0;

    /**
     * \brief Return the icon name of the plugin
     *
     * \note This should return something that KIcon() can understand
     * \return The naem of the icon of the plugin
     */
    virtual const QString iconSrc() const=0;
    virtual const KIcon icon() const;

    virtual const QString preferredTrigger() const;

    bool addCommand(Command *command);
    virtual bool addCommandPrivate(Command *command);

    virtual QList<QAction*> getGuiActions() const;

    /**
     * \brief Returns the #commands
     * \return List of commands of this plugin
     */
    virtual CommandList* getCommands() const
    {
      return commands;
    }

    /**
     * \return Returns true if the command manager has any commands
     */
    bool hasCommands() const
    {
      return (commands && !commands->isEmpty());
    }

    virtual void setFont(const QFont& font);

    virtual CommandConfiguration* getConfigurationPage();
    virtual CreateCommandWidget* getCreateCommandWidget(QWidget *parent);
    CreateCommandWidget* getCreateVoiceInterfaceCommandWidget(QWidget *parent);

    virtual bool processResult(const RecognitionResult& recognitionResult);

    virtual bool deleteCommand(Command *command);

    virtual bool deSerialize(const QDomElement&);
    virtual QDomElement serialize(QDomDocument *doc);

    virtual bool deSerializeConfig(const QDomElement& elem);

    virtual QDomElement serializeConfig(QDomDocument *doc);

    bool deSerializeCommands(const QDomElement& elem);
    virtual bool deSerializeCommandsPrivate(const QDomElement& elem);

    virtual QDomElement serializeCommands(QDomDocument *doc);

    virtual bool trigger(const QString& triggerName);

    virtual bool installInterfaceCommand(QObject* object, const QString& slot,
    const QString& actionName, const QString& iconSrc,
    const QString& description, bool announce, bool showIcon=false,
    int state=SimonCommand::DefaultState,
    int newState=SimonCommand::DefaultState,
    const QString& defaultVisibleTrigger=QString(),
    QString id=QString());

    virtual bool installListInterfaceCommand(CommandListElements::Element element,
    QObject* object, const QString& slot, QString id,
    QString description=QString(),
    int state=SimonCommand::DefaultState,
    int newState=SimonCommand::DefaultState,
    QString actionName=QString(), QString iconSrc=QString(),
    bool announce=false, bool showIcon=false,
    QString defaultVisibleTrigger=QString());

    /**
     * \return Returns the #voiceInterfaceCommandTemplates list of the plugin
     * \sa installInterfaceCommand(), installListInterfaceCommand()
     */
    QList<VoiceInterfaceCommandTemplate*> getVoiceInterfaceCommandTemplates()
    { return voiceInterfaceCommandTemplates; }

    /**
     * \brief Switches the CommandManager to the given new state
     * \param newState New state (see also \ref SimonCommand::State)
     */
    void switchToState(int newState)
    { m_currentState = newState; }

    /**
     * \brief Sets the parent action collection to the given pointer
     * \param aC The new parent collection
     *
     * This is just a simple setter method and there are only very few
     * cases where you really want access to the action collection anyways.
     *
     * \sa #m_actionCollection
     */
    void setActionCollection(ActionCollection *aC) {
      m_actionCollection =  aC;
    }

    /**
     * \brief Constructor
     *
     * Empty constructor. Initializes #m_currentState to the DefaultState and sets both #commands and #config
     * to 0 values.
     *
     * Please keep your derived constructors as lean as possible. Use \ref deSerializeConfig() or \ref deSerializeCommands()
     * to initialize the manager instead.
     */
    CommandManager(Scenario *parentScenario, const QVariantList& args) : QAbstractItemModel((QObject*) parentScenario),
    ScenarioObject(parentScenario),
    m_currentState(SimonCommand::DefaultState), commands(0), config(0) {
      Q_UNUSED(args);
    }

    void setGreedyStatus(bool isGreedy);
    VoiceInterfaceCommand* getVoiceInterfaceCommand(const QString& id);
    virtual ~CommandManager();

};
#endif
