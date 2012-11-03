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

#ifndef SIMON_COMMAND_H_651AF6CF619F4C8EA4E38A3F99962A60
#define SIMON_COMMAND_H_651AF6CF619F4C8EA4E38A3F99962A60

/** \file command.h
 * \brief The file containing the Command baseclass header.
 */

/**
 * \def STATIC_CREATE_INSTANCE_H(x)
 * \brief Static createInstance() method.
 *
 * Place this in the public section of the header file of the command subclass
 * if you want to use the DEFAULT_DESERIALIZE_COMMANDS_PRIVATE
 * macros in the CommandManager of your plugin (see the \ref CommandManager documentation for
 * more details).
 *
 * Replace x with the classname of your command subclass.
 */
#define STATIC_CREATE_INSTANCE_H(x) \
  static x* createInstance(const QDomElement& element);

/**
 * \def STATIC_CREATE_INSTANCE_C(x)
 * \brief Static createInstance() method.
 *
 * Place this in the source file of the command subclass
 * if you want to use the DEFAULT_DESERIALIZE_COMMANDS_PRIVATE
 * macros in the CommandManager of your plugin (see the \ref CommandManager documentation for
 * more details).
 *
 * Replace x with the classname of your command subclass.
 */
#define STATIC_CREATE_INSTANCE_C(x) \
  x* x::createInstance(const QDomElement& element) \
  { \
    x *command = new x(); \
    if (!command->deSerialize(element)) \
    { \
      delete command; \
      return 0; \
    } \
    return command; \
  }

#include "simonmodelmanagement_export.h"

#include <QList>
#include <QString>
#include <KIcon>
#include <QMap>
class QDomElement;
class QDomDocument;

#include "simoncommand.h"
class CommandManager;

/**
 * @class Command
 * @brief This class represents one command with all its attributes
 *
 * Extend this base class if you want your command manager to have subcommands.
 *
 * If your commands should be user configurable, you also have to extend
 * CreateCommandWidget.
 *
 * If you want to use the DEFAULT_DESERIALIZE_COMMANDS_PRIVATE macros in your
 * CommandManager, you need to add the following code to your subclass of Command:
 *
 * Header file (replace "x" with the classname of your command subclass):
 * \code
#define STATIC_CREATE_INSTANCE_H(x)
\endcode
* Implementation (replace "x" with the classname of your command subclass):
* \code
#define STATIC_CREATE_INSTANCE_C(x)
\endcode
*
* \sa CreateCommandWidget
*/
class MODELMANAGEMENT_EXPORT  Command
{

  private:
    /// The name / trigger of the command
    QString triggerName;

    /// Icon name of the command
    QString iconSrc;

    /// Detailed, user visible description of the command
    QString description;

    /// If this is true, the base class will provide graphical feedback when the command is triggered (passive popup)
    bool announce;

    /// If this is true, the command will NOT be displayed in the command list of the plugin
    bool hidden;

    /// The parent CommandManager
    CommandManager *m_parent;
    
    /// Parameters of the last triggering
    QStringList m_currentParameters;

  protected:
    /// \brief The command is bound to this state.
    /// \sa matches()
    QList<int> boundStates;

    /// Whenever the command is executed, the commandmanager should switch into this state
    int switchToState;

    /// Empty private constructor
    Command() { hidden = false; }

    /**
     * \brief Setter method for triggerName
     * \param trigger The new triggerName
     */
    void setTriggerName(const QString& trigger) { triggerName = trigger; }

    /**
     * \brief Setter method for iconSrc
     * \param iconSrc The new iconSrc
     */
    void setIconSrc(const QString& iconSrc) { this->iconSrc = iconSrc; }

    /**
     * \brief Setter method for description
     * \param description The new description
     */
    void setDescription(const QString& description) { this->description = description; }

    /**
     * \brief Execute the plugin specific action
     *
     * This method does the actual work. Whatever a command of your plugin should do, it should do it in this method
     * You can access the arguments passed to the command by calling \sa currentArguments()
     *
     * \param state Reference parameter; The state of the command manager.
     *
     * \return If your work was done, return true. Otherwise return false (not correct state, etc.). Have a look at the trigger()
     * 	method of the CommandManager class for more details.
     */
    virtual bool triggerPrivate(int* state)=0;
    
    QStringList currentArguments() const;
    
    QString getParsedTrigger() const;

    /**
     * \brief Export the user relevant parts of your Command subclass
     *
     * Use this to export user relevant information about your command. An executable command would export the name
     * of the executable it starts when it is triggered, for example.
     *
     * The map of key -> value pairs is then displayed in the command view in simon.
     *
     * This is only information for the user and isn't used internally for anything so go nuts :)
     *
     * Shared information that is common for all plugins will be added automatically (the description for example).
     *
     * \return Key -> Value pairs of "field" -> "value" information; For example: "Executable" -> "/usr/bin/firefox"
     * \sa getValueMap()
     */
    virtual const QMap<QString,QVariant> getValueMapPrivate() const=0;

    /**
     * \brief Serializes the current command state to an XML element
     *
     * You only need to take care of the special attributes that your Command subclass introduces,
     * everything else will be handled by the base class implementation.
     *
     * \param doc Parent document
     * \param commandElem Command elem. This already contains the base class attributes, just add the additional
     *        attributes that your subclass introduces.
     */
    virtual QDomElement serializePrivate(QDomDocument *doc, QDomElement& commandElem)=0;

    /**
     * \brief Restore the internal Command state to the given XML element
     *
     * This method is used to restore the state of your command that was serialized during the last
     * call of serializePrivate().
     *
     * You only need to take care of the special attributes that your Command subclass introduces,
     * everything else will be handeled by the base class implementation.
     *
     * The passed parameter will be the complete XML element that describes the command, NOT
     * only the parts that you added during serializePrivate (so use the tagname based accessors
     * and do not rely on the order you used when serializing)
     *
     * \param commandElem the XML Element to parse
     * \return Success; Return false if the input is corrupt.
     */
    virtual bool deSerializePrivate(const QDomElement& commandElem)=0;

  public:
    /**
     * \brief Sets the parent command manager to the given value.
     * \param man The parent command manager
     */
    void setParent(CommandManager* man) { m_parent = man; }

    /**
     * \brief Returns the parent command manager
     * \returns Parent command manager
     */
    CommandManager* parent() const { return m_parent; }

    /**
     * \brief Returns the category name of the command
     *
     * Returns a translated category name of the command. For commands that start external
     * programs this could for example be:
     * \code
       return i18n("Program");
       \endcode
     *
     * \return Category name
     */
    virtual const QString getCategoryText() const=0;

    /**
     * \brief Returns a category icon
     * Returns an icon for the command category. For commands that start external programs this could
     * for example be:
     * \code
       return KIcon("applications-system");
       \endcode
     * \return Category icon
     */
    virtual const KIcon getCategoryIcon() const=0;
    
    virtual bool matches(int commandManagerState, const QString& trigger);

    virtual bool trigger(int* state, bool silent=false);

    virtual const QMap<QString,QVariant> getValueMap() const;

    /**
     * @brief Constructor
     *
     * Initializes the command
     *
     *	@param name The trigger of the command
     *	@param icon The icon name of the command
     *	\param description_ Detailed, user readable description of the command
     *	\param boundState_ The state the CommandManager should be in for this Command to be relevant
     *	\param newState_ The state to switch to when the command was executed
     *	\param announce_ Should we announce it when we execute this command?
     */
    Command(const QString& name, const QString& icon, const QString& description_, int boundState_ = SimonCommand::DefaultState,
      int newState_ = SimonCommand::DefaultState, bool announce_ = true)
      : triggerName(name),
      iconSrc(icon),
      description(description_),
      announce(announce_),
      hidden(false),
      boundStates(QList<int>() << boundState_),
      switchToState(newState_)
    {}
    
    
    /**
     * @brief Constructor
     *
     * Initializes the command
     *
     *  @param name The trigger of the command
     *  @param icon The icon name of the command
     *  \param description_ Detailed, user readable description of the command
     *  \param boundStates_ The states the CommandManager should be in for this Command to be relevant
     *  \param newState_ The state to switch to when the command was executed
     *  \param announce_ Should we announce it when we execute this command?
     */
    Command(const QString& name, const QString& icon, const QString& description_, QList<int> boundStates_,
      int newState_ = SimonCommand::DefaultState, bool announce_ = true)
      : triggerName(name),
      iconSrc(icon),
      description(description_),
      announce(announce_),
      hidden(false),
      boundStates(boundStates_),
      switchToState(newState_)
    {Q_ASSERT(boundStates.count());}

    /**
     * @brief Returns the trigger of the command
     * @return Returns the name / trigger of the command (#triggerName)
     */
    QString getTrigger() const { return triggerName; }

    /**
     * \brief Returns the bound state of the command
     * \return The state the command manager should be in for this command to be relevant
     */
    QList<int> getBoundStates() const { return boundStates; }

    /**
     * \brief Re-binds the command to a different state
     * \param state The state the command manager should be in for this command to be relevant
     */
    void setBoundState(int state) { 
      boundStates.clear();
      boundStates << state;
    }
    
    /**
     * \brief Re-binds the command to different states
     * \param states The states the command manager should be in for this command to be relevant
     */
    void setBoundState(QList<int> states) { 
      boundStates = states;
    }

    /**
     * \brief Returns the state the command manager should switch to when the command is executed
     * \return Switch to this state when the command is triggered
     */
    int getTargetState() const { return this->switchToState; }

    /**
     * \brief Sets the state the command manager should switch to when the command is executed
     * \param targetState Switch to this state when the command is triggered
     */
    void setTargetState(int targetState) { this->switchToState = targetState; }

    /**
     * \brief Returns if the base class should notify the user when the command is run
     * \return True if a graphical popup should be shown when the command is executed
     */
    bool getAnnounce() const { return announce; }

    /**
     * \brief Returns the user readable description of the command
     * \return Description of the command
     */
    QString getDescription() const { return this->description; }

    /**
     * @brief Returns the icon of this command.
     * If no icon is set, the category icon is returned.
     * @return The icon of the command.
     * \sa getCategoryIcon()
     */
    const KIcon getIcon() const
    {
      KIcon thisIcon = KIcon(iconSrc);
      if (!thisIcon.isNull())
        return thisIcon;
      else return getCategoryIcon();
    }

    QDomElement serialize(QDomDocument *doc);

    bool deSerialize(const QDomElement& commandElem);

    /**
     * @brief Returns the icon name of this command.
     * @return The name of the icon. This should be something that KIcon can interpret.
     */
    const QString getIconSrc() const
    {
      return iconSrc;
    }

    /**
     * @brief Sets the command "hidden"
     * Hidden commands will not be shown in the commands list and are therefore not 
     * configurable through the default interface. Use this carefully!
     * @param hide Should the command be hidden
     */
    void setHidden(bool hide)
    {
      this->hidden = hide;
    }

    /**
     * @brief Returns true if the command should be hidden in the list
     * Hidden commands will not be shown in the commands list and are therefore not 
     * configurable through the default interface. Use this carefully!
     * @return True if the command should not be shown in the command list
     */
    bool getHidden() const
    {
      return this->hidden;
    }

    /**
     * \brief Returns the amount of parameter _needed_ from this type of command
     * The default \see matches() function will require at least the amount of 
     * parameters present here. "Needed" parameters are not allowed to be empty
     *
     * The default implementation returns 0
     *
     * \return The number of required arguments
     */
    virtual int neededParameterCount() const
    {
      return 0;
    }

    /**
     * \brief Empty destructor
     */
    virtual ~Command() {}
    
    static bool parseArguments(const QString& input, const QString& scheme, QStringList& arguments);
};

/**
 *	@typedef CommandList
 *	QList of Commands
 */
typedef QList<Command*> CommandList;

#endif
