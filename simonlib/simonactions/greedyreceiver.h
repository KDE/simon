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

#ifndef SIMON_GREEDYRECEIVER_H_561D6EF525114CB7B4D84C003137CBE9
#define SIMON_GREEDYRECEIVER_H_561D6EF525114CB7B4D84C003137CBE9

#include "actionlib_export.h"
#include <simonrecognitionresult/recognitionresult.h>
#include <QString>

class CommandManager;

/**
 * \class GreedyReceiver
 * \brief Enables the deriving class to become "greedy"
 *
 * Subclass from greedy receiver if your CommandManager (or any other object really) needs
 * to take complete control of the recognition results at some point.
 *
 * For example, it makes sense for the list command to become greedy when the list is displayed.
 * From the point the list command is triggered, it should intercept all results before they
 * reach the other plugins for as long as no selection is made or the window is closed.
 *
 * You can have more than one greedy receiver at a time wheras they behave like a stack: The
 * last loaded greedy receiver, receives all results as long as it is greedy. As soon as it
 * stops being greedy, the previously loaded greedy receiver takes control again, etc.
 *
 * Greedy receivers can intercept not only the finished sentence or the selected raw recognition
 * rate, but can even take full control of the whole list of recognition results preselected
 * by the recognition process.
 *
 * Normally, simon receives a list of recognition results from simond who contain confidence
 * scores. During normal operation, simon selects the best match from this list of results
 * (best guess) altough it can also ask the user if the correct result can not be determined
 * and if it is configured to do so.
 *
 * Greedy receivers can process the whole list of recongition results bypassing this selection
 * process. However, simon will still prune recognition results below a configurable confidence
 * threshold ensuring that the greedy receiver only gets useful results.
 *
 * Depending on how much control you want in your subclass, you can subclass one (or none) of
 * the following methods:
 * - \ref greedyTriggerRawList(): Complete list of raw recognition results
 * - \ref greedyTriggerRaw(): Selected raw recognition result (best match)
 * - \ref greedyTrigger(): Sentence of the selected recognition result
 *
 * If you choose not to overwrite any of these function, the "normal" trigger() method of the
 * command manager will be called as usual. This is very useful if you set up special commands
 * for your GreedyState (see the CommandManager and Command classes for details).
 */
class SIMONACTIONS_EXPORT GreedyReceiver
{

  private:
    /**
     * \brief The parent command manager
     */
    CommandManager *m_manager;

  public:
    /**
     * \brief Greedy trigger of the raw list of recognition results
     *
     * This list contains all recognition results from the server, minus
     * the ones that scored below the configurable confidence threshold.
     *
     * The default behaviour will ask the user for his opinion if more
     * than one result matches (exactly the same way simon would, if no
     * greedy receiver was running).
     *
     * \param results The received list of recognition results
     * \return True, if the command was accepted
     */
    virtual bool greedyTriggerRawList(const RecognitionResultList &results);

    /**
     * \brief Trigger with the raw recognition result
     *
     * The greedy equivalent of CommandManager::processResult()
     *
     * The default implementation simply extracts the sentence and
     * calls \ref greedyTrigger().
     *
     * \param result The selected raw recognition result
     * \return True, if the command was accepted
     */
    virtual bool greedyTriggerRaw(const RecognitionResult& result);

    /**
     * \brief Trigger based on the recognized sentence
     *
     * The greedy equivalent of CommandManager::trigger()
     *
     * The default implementation simply calls CommandManager::trigger()
     * with the extracted sentence
     *
     * \param sentence The recognized sentence
     * \return True, if the command was accepted
     */
    virtual bool greedyTrigger(const QString& sentence);

    /**
     * \brief Starts being greedy
     *
     * This registers the greedy receiver as such in simons command system
     * and switches the parent command manager in a greedy state (see
     * \ref CommandManager::setGreedyStatus() for details).
     */
    virtual void startGreedy();

    /**
     * \brief Stops being greedy
     *
     * This removes the greedy receiver from simons command system
     * and switches the parent command manager in a non greedy state (see
     * \ref CommandManager::setGreedyStatus() for details).
     */
    virtual void stopGreedy();

    /**
     * \brief Constructor
     * \param manager The parent command manager
     */
    GreedyReceiver(CommandManager *manager) : m_manager(manager)
      { }

    /**
     * \brief Destructor
     */
    virtual ~GreedyReceiver();

};
#endif
