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

#ifndef SIMON_CREATECOMMANDWIDGET_H_953FB9DDF3FA4DC5965BF6C9797CAA8A
#define SIMON_CREATECOMMANDWIDGET_H_953FB9DDF3FA4DC5965BF6C9797CAA8A

#include "simonmodelmanagement_export.h"

#include <QWidget>

class Command;
class CommandManager;
class KIcon;

/**
 *	@class CreateCommandWidget
 *	@brief Provides an abstract base-class for the widget that is going to be displayed in the New/Modify-Command-Dialog
 *
 *	If you want your commands to be user configurable, you most likely want to derive a class from CreateCommandWidget.
 *	CreateCommandWidgets are the graphical "factories" for Commands where users can define and edit their commands.
 *
 *	To enable the user to add and edit your Command subclass, subclass this class and return an instance of it in the
 *	CommandManager::getCreateCommandWidget() method.
 */
class MODELMANAGEMENT_EXPORT CreateCommandWidget : public QWidget
{
  Q_OBJECT

    signals:
  /**
   * \brief The dialog should re-check if the command was completely specified
   * \sa isComplete();
   */
  void completeChanged();

  /**
   * \brief Suggests a command to the rest of the dialog
   *
   * The CreateCommandWidget only manages the additional attribute your subclass has over the
   * Command base class. For the other, common attributes, a static interface is used that
   * is not controlled by this class. However, your CreateCommandWidget might want to suggest
   * a trigger or a specific icon to the user. If you want to do this, construct your command
   * and emit it with this signal.
   *
   * \param command The suggested command
   * \sa propagateCreatedCommand()
   */
  void commandSuggested(Command* command);

  protected:
    /// The parent command manager
    CommandManager *m_manager;

  protected slots:
    /**
     * \brief Assigns parent manager and passes command on to \ref commandSuggested()
     * Conveniance method: If your CreateCommandWidget subclass uses additional means to
     * configure commands (for example an import wizard) which returns the found command
     * by emitting a signal, you can connect it to this slot. The base class will then
     * assign the parent manager as the commands parent before suggesting it using
     * \ref commandSuggested()
     * \sa commandSuggested()
     */
    virtual void propagateCreatedCommand(Command *c);

  public:
    /**
     * @brief Creates the command
     * @author Peter Grasch
     *
     * This function gets the common parameters for all commands (name, iconSrc and description).
     *
     * A plugin implementing this should add the additional parameters of its Command-derived
     * class (like the path to the executable in an ExecutableCommand) and create an instance
     * of the derived class.
     *
     * If you do not have enough information to create the command (i.e. the user didn't
     * enter all the required fields), return a nullpointer.
     *
     * For an executable command this could look something like this:
     * \code
    if (!isComplete())
    return 0;

    return new ExecutableCommand(name, iconSrc, description, ui.urExecutable->url().path(),
    ui.urWorkingDirectory->url());
    \endcode
    * \param name The trigger of the command
    * \param iconSrc The icon name of the command
    * \param description The description of the command
    */
    virtual Command* createCommand(const QString& name, const QString& iconSrc, const QString& description)=0;

    /**
     * \brief Creates the command and adds it to the parent manager
     * Calls createCommand() and adds the result to the manager by calling CommandManager::addCommand().
     * \param name The trigger of the command
     * \param iconSrc The icon name of the command
     * \param description The description of the command
     * \sa createCommand()
     */
    virtual bool addCommand(const QString& name, const QString& iconSrc, const QString& description);

    /**
     * \brief When editing a command this method will be called to initialize the display with the old values
     *
     * This method should adapt the current configuration to the values of the given command.
     *
     * \param command The command to display
     * \return False, if for some reason the input fields could not be set to the values configured in the given command
     */
    virtual bool init(Command* command)=0;

    /**
     * \brief Have all needed attributes been specified?
     * \return True if all necessary fields have been filled out
     */
    virtual bool isComplete()=0;

    /**
     * \brief Tests if the command was created from the same manager
     *
     * Calls \sa isInstanceOfSameManager(CommandManager*)
     *
     * \param command The command to check
     * \return True if the parent manager (#m_manager) is the same as the parent manager of the provided command
     */
    virtual bool isInstanceOfSameManager(Command *command);

    /**
     * \brief Tests the creator refers to the same manager
     *
     * \param manager The manager to compare
     * \return True if the parent manager (#m_manager) is the same as the provided manager
     */
    virtual bool isInstanceOfSameManager(CommandManager *manager);


    /**
     * @brief Constructor
     * \param manager The parent command manager
     * \param parent The parent widget
     */
    explicit CreateCommandWidget(CommandManager *manager, QWidget *parent=0) : QWidget(parent),
      m_manager(manager)
      {}

    /**
     * \brief Destructor
     */
    virtual ~CreateCommandWidget() {}


    /**
     * \brief Returns a list of supported parameters of commands of this type
     *
     * This list will be displayed in the creation dialog below the trigger
     *
     * \return the list of supported parameters
     */
    virtual QList<CommandParameter>& supportedParameters() const;
};
#endif
