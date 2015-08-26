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

#ifndef SIMON_LISTCOMMAND_H_B003A824E4CE4D008341F2E92534890A
#define SIMON_LISTCOMMAND_H_B003A824E4CE4D008341F2E92534890A

#include <simonscenarios/command.h>
#include <simonscenarios/voiceinterfacecommand.h>
#include "greedyreceiver.h"
#include "commandlistwidget.h"
#include "actionlib_export.h"
#include <QList>
#include <QObject>
#include <QHash>
class CommandListWidget;
class VoiceInterfaceCommand;

/**
 * @class ListCommand
 * @brief Describes a list command; Consists of several other commands
 *
 * This is a command wrapper around a \ref CommandListWidget. You should use the ListCommand
 * for when all you want to execute are other commands that are managed by other command managers.
 *
 * The ListCommand has no tight binding with its sub commands, it only stores their type and
 * trigger. When The ListCommand triggers one of its sub options it merely handes this information
 * to ActionManager and hopes that the referenced command still exists.
 *
 * This loose binding of course has its upsides: ListCommands are very cheap and when one of the
 * referenced Commands changes, the ListCommand points to the changed command. However, the association
 * is based on the trigger name as well in which case the list command can't find the command
 * it is supposed to point to anymore.
 *
 * The ListCommand uses a CommandListWidget internally to present the user with the results and
 * adapts it automatically to the prevailing list configuration (scenario wide if the list is
 * part of a scenario or system wide if it is not).
 *
 * When the list command gets triggered, it becomes greedy (see \ref GreedyReceiver for details).
 */
class SIMONACTIONS_EXPORT ListCommand : public QObject, public Command, public GreedyReceiver
{
  Q_OBJECT

    signals:
  /// An entry has been selected (triggered)
  void entrySelected();

  /// The list was canceled
  void canceled();

  private:
    /**
     * \brief This contains the voice interface commands used to interact with the command list widget
     */
    QList<VoiceInterfaceCommand*> m_subCommands;

    /// The parent command manager
    CommandManager *m_parentManager;

    /// The command list widget used to show the options
    CommandListWidget *clw;

    /**
     * \brief If your list needs Back / Next buttons (> 8 subcommands) this will hold the current start index.
     *
     * The selected command trigger can be determined by the current index in the list + startIndex
     * (+ 1 if no back button is displayed)
     */
    int startIndex;

    /// Icon names of the subcommands
    QStringList iconsrcs;

    /// Trigger names of the subcommands
    QStringList commands;

    /// Command plugin category names of the subcommands
    QStringList commandTypes;

    /**
     * \brief Displays the current options in the command list widget
     *
     * This displays the currently active options. This will list the current 8 piece subset.
     * If you have more than 8 child commands, Back and next buttons will be displayed; This
     * method will always display the current subset.
     */
    void listCurrentCommandSection();

    /**
     * \brief Retrieves the currently configured VoiceInterfaceCommands for processing lists
     *
     * This will get the default configuration from either the parent scenario or the
     * global ActionManager if the ListCommand is not in any scenario
     *
     * \return Hash which contains (possibly multiple, possibly none) VoiceInterfaceCommand
     * 		associated to each Element of the list
     */
    QHash<CommandListElements::Element, VoiceInterfaceCommand*> getAdaption();

  protected:
    /**
     * \brief Returns information about the contained Elements
     * \return "Commands" -> List of subcommand trigger names
     */
    const QMap<QString,QVariant> getValueMapPrivate() const;

    /**
     * \brief Triggers the list. This will show the command list widget and make the command greedy
     * \param state Reference parameter: The state of the command manager
     */
    bool triggerPrivate(int *state);

    /**
     * \brief Loads the command state from the given XML Element
     * \param commandElem The XML element to parse
     * \return True if the List has been deserialized correctly
     */
    virtual bool deSerializePrivate(const QDomElement& commandElem);

    /**
     * \brief Constructor
     * \param parentManager The parent command manager. This can be 0 if the list is not in any scenario.
     */
    ListCommand(CommandManager *parentManager);

    /**
     * \brief Set up signal / slots
     */
    void init();

  private slots:
    /**
     * \brief Executes the child command at the given index
     *
     * This will use #startIndex to determine the actual position of the
     * current index (might be fudged by the back / next logic).
     *
     * \sa startIndex
     * \param index The index of the command in the current display to execute
     * \return True, if the ListWidget executed a command successfully
     */
    bool processRequest(int index);

  public slots:
    /**
     * \brief Triggers the command associated with the list element associated with the input text
     *
     * This will get the adapted voice commands from the ScenarioManager or the ActionManager
     * (depending on the parent of the ListCommand) and try to match the received sentence
     * to one of the list commands.
     *
     * If a match is found, the \ref processRequest() function will be called with the retrieved index
     * (or the list widget hidden if the user wants to cancel)
     *
     * \param inputText The recognition result sentence
     * \return True if a matching command was found / executed
     * \sa processRequest()
     */
    bool greedyTrigger(const QString& inputText);

    /**
     * \brief Cancels the selection process
     *
     * Hides the command list widget and stops being greedy
     */
    void cancel();

  public:
    /**
     * \brief Serializes the list to an XML element
     *
     * This will serialize the subcommands as shallow references. Each command will be referenced
     * by a combination of this type and trigger.
     *
     * \param doc Parent document
     * \param commandElem The element already containing basic information common to all commands
     */
    QDomElement serializePrivate(QDomDocument *doc, QDomElement& commandElem);

    /// Returns the list category description (translated version of "List")
    static const QString staticCategoryText();

    /// Returns the list icon
    static const QIcon staticCategoryIcon();

    /// Returns the list icon
    const QIcon getCategoryIcon() const;
    /// Returns the list category description (translated version of "List")
    const QString getCategoryText() const;

    /**
     * @brief Constructor
     * \param parentManager The parent command manager
     * \param name The name (trigger) of the command
     * \param iconSrc The icon name of the list command itself
     * \param description The description of the command
     * \param commands List of triggers of the subcommands
     * \param iconsrcs List of icon names for the subcommands
     * \param commandTypes Types of the subcommands (plugin category names)
     */
    ListCommand(CommandManager *parentManager, const QString& name, const QString& iconSrc, const QString& description, const QStringList& commands, const QStringList& iconSrcs, const QStringList& commandTypes);

    /**
     * @brief Returns the list of subcommands (#commands)
     * \return QStringList List of triggers of the subcommands
     */
    QStringList getCommands() const { return this->commands; }

    /**
     * @brief Returns the list of icon names of the subcommands (#iconsrcs)
     * \return QStringList List of icon names of the subcommands
     */
    QStringList getIconSrcs() const { return this->iconsrcs; }

    /**
     * @brief Returns the types of sub commands
     * \return The types (plugin category names) of the subcommands
     */
    QStringList getCommandTypes() const { return this->commandTypes; }

    /**
     * \brief set The font of the plugin to given font; Adjusts the displayed list
     * \param font The font to use
     */
    void setFont(const QFont& font);

    /**
     * \brief Static createInstance Method
     * \param manager The parent command manager (can be 0 if outside a manager)
     * \param element The XML element to deserialize from
     * \return The created ListCommand or 0 if creation failed
     */
    static ListCommand* createInstance(CommandManager *manager, const QDomElement& element);

    /// Destructor
    ~ListCommand();

};
#endif
