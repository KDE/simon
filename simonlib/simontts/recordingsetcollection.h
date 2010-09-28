/*
 *   Copyright (C) 2010 Peter Grasch <grasch@simon-listens.org>
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

#ifndef SIMON_RECORDINGSETCOLLECTION_H_D92C3FEDF32B415094257DB891621383
#define SIMON_RECORDINGSETCOLLECTION_H_D92C3FEDF32B415094257DB891621383

#include "simontts_export.h"
#include <QString>
#include <QList>

class RecordingSet;


/**
 * \class RecordingSetCollection
 * \since 0.4
 * \brief Organizes multiple instances of @sa RecordingSet
 */
class SIMONTTS_EXPORT RecordingSetCollection
{
  private:
    /// List of managed sets
    QList<RecordingSet*> m_sets;

    /// Sets that should be removed the next time we save
    QList<RecordingSet*> m_setsScheduledForDeletion;

    /**
     * \brief Clears the @sa m_sets and @sa m_setsScheduledForDeletion lists
     */
    void deleteAll();

    /**
     * \brief Deletes all sets scheduled for deletion
     * \see m_setsScheduledForDeletion
     * \return Success
     */
    bool purgeSelectedSets();

    /**
     * \brief Gets all sets that are going to be used to resolve samples
     * \return List of active recording sets (in order)
     */
    QList<RecordingSet*> getActiveSets();

    /**
     * \brief Returns a free id
     * \return id guaranteed to be unique within this set
     */
    int getFreeId();

  public:
    /**
     * \brief Constructor
     */
    RecordingSetCollection();

    /**
     * \brief Initializes the collection from the given path
     * \param path The path to the xml configuration file
     * \return Success of loading
     */
    bool init(const QString& path);

    /**
     * \brief Saves the collection to the given path
     * \param path The path to the xml configuration file
     * \return Success of loading
     */
    bool save(const QString& path);

    /**
     * \brief Returns the set with the given id
     * \param id The id of the set to get
     * \return The acquired set
     */
    RecordingSet* getSet(int id) const;

    /**
     * \brief Returns true if there is a recording for the given text in the active sets
     * \param text The text to find
     * \return True if there is a recording for the text
     */
    bool canSay(const QString& text);

    /**
     * \brief Gets the path of the recording of the given text
     * \param text The text to search for
     * \return Full path to the recording or null string if there is none
     */
    QString getPath(const QString& text);

    /**
     * \brief Returns ids of all available sets
     * \return List of set ids
     * \see getSet(int)
     */
    QList<int> getSets();

    /**
     * \brief Returns the set with the given id
     * \param id The id to look for
     * \return The found set
     */
    RecordingSet* getSet(int id);

    /**
     * \brief Adds new set
     * \param name The name of the new set
     * \return success
     */
    bool addSet(const QString& name);

    /**
     * \brief Renames the given set to the also given new name
     * \param id The id of the set
     * \param name The new name of the set
     * \return success
     */
    bool renameSet(int id, const QString& name);

    /**
     * \brief Removes the set with the given id
     * \param id The set to remove
     * \return true if the set was removed (false if it wasn't even in there)
     */
    bool removeSet(int id);

    /**
     * \brief Adds the given recording to the set identified by id
     * \param id The id of the set to add the recording to
     * \param text The text said in the recording
     * \param path Path to the (temporary) recording file
     * \return success
     */
    bool addRecording(int id, const QString& text, const QString& path);

    /**
     * \brief Edits the given recording of the set identified by id
     * \param id The id of the set to edit the recording of
     * \param text The text said in the recording
     * \param path Path to the (temporary) recording file
     * \return success
     */
    bool editRecording(int id, const QString& text, const QString& path);

    /**
     * \brief Removes the given recording from the set identified by id
     * \param id The id of the set to remove the recording from
     * \param text The text said in the recording
     * \return success
     */
    bool removeRecording(int id, const QString& text);

    /**
     * \brief Destructor
     * \note Calls @sa deleteAll() - beware of using sets retreived @sa getSet()!
     */
    ~RecordingSetCollection();
};

#endif

