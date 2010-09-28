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

#ifndef SIMON_RECORDINGSET_H_D92C3FEDF32B415094257DB891621383
#define SIMON_RECORDINGSET_H_D92C3FEDF32B415094257DB891621383

#include "simontts_export.h"
#include <QDomElement>
#include <QString>
#include <QHash>
#include <QAbstractItemModel>

class QDomDocument;
class QModelIndex;

/**
 * \class RecordingSetText
 * \since 0.4
 * The recording set text is used to create case insensitive hashes
 * so that we can do fast lookups, preserve the ease of use of QHash,
 * and still store the texts case sensitive for the user interface.
 */
class SIMONTTS_EXPORT RecordingSetText : public QString {
  public:
    RecordingSetText(const QString& t) : QString(t) {}
};

/**
 * \class RecordingSet
 *
 * \brief One set of pre recorded sound snippets (usually from one speaker)
 */
class SIMONTTS_EXPORT RecordingSet : public QAbstractItemModel
{
  private:
    /// Set to true if the initialization is incomplete
    bool m_isNull;

    /// Id of this set
    int m_id;
    
    /// Name of the set
    QString m_name;

    /// Hash table of recordings associated with this set
    QHash<RecordingSetText /*text*/, QString/*filename*/> m_recordings;

    /**
     * \brief Returns the absolute path for the given sample in this set
     * \param sampleId Internal id of the sample
     * \param absolute If false the path will be relative to @sa getBaseDirectory()
     * \param temporary If true returns the temporary path, else the persistent one
     * \return Path of sample with given id in this set
     */
    QString getSamplePath(int sampleId, bool absolute=true, bool temporary=true) const;

    /**
     * \brief Returns the base directory for this set
     * \return Absolute path
     */
    QString getBaseDirectory() const;

    /**
     * \brief Returns the temporary directory for this set
     * \return Absolute path
     */
    QString getTempDirectory() const;

    /**
     * \brief Removes any temporary sample of this text if there are any
     * \param text The text to look for
     * \return success
     */
    bool removeTemporarySample(const QString& text);

    /**
     * \brief Returns a free id for the next recording
     * \return Id guaranteed to be not yet in use in this set
     */
    int getNextId() const;

    /**
     * \brief Clears all temporary files
     *
     * Added recordings are stored in a temporary directory until calling serialize()
     * This removes these temporary files.
     *
     * \return success
     */
    bool clearTemp();

    /**
     * \brief Removes this filename from the list of files to remove
     * \param file The filename to keep
     * \return success
     */
    bool keepFile(const QString& file);


  public:
    RecordingSet();
    RecordingSet(int id, const QString& name);
    ~RecordingSet();

    /*
     * \brief Deserializes this set from a XML dom element
     * \param elem The configuration element to deserialize
     * \return Success
     */
    bool deserialize(const QDomElement& elem);
    /*
     * \briefSerializes this set into a XML dom element
     * \param doc Parent document
     * \return The created element
     */
    QDomElement serialize(QDomDocument* doc);

    /**
     * \brief Applies temporary changes
     *
     * Any added / edited / removed samples will be moved to / removed from the
     * persistent storage directory;
     *
     * \note Automatically called by \sa serialize()
     *
     * \return success
     */
    bool applyTemp();

    /**
     * \brief Returns the path of recording of the given text
     * \note Will return the temporary path if one exists;
     * \param text The text to look for
     * \param forceTemp If true, only a temporary path is returned (or none)
     * \return Full path to the file or QString() if not found
     */
    QString getPath(const QString& text, bool forceTemp=false) const;

    /*
     * Getter method for @sa m_isNull
     * \return m_isNull
     */
    bool isNull() const { return m_isNull; }

    /*
     * Getter method for @sa m_id
     * \return m_id
     */
    int id() const { return m_id; }

    /*
     * Getter method for @sa m_name
     * \return m_name
     */
    QString name() const { return m_name; }

    /**
     * \brief Setter method for @sa m_name
     * \return Success (in this implementation always true
     */
    bool rename(const QString& newName);

    /**
     * \brief Adds the given recording to this set
     * \param text The text said in the recording
     * \param path Path to the (temporary) recording file
     * \return success
     */
    bool addRecording(const QString& text, const QString& path);

    /**
     * \brief Edits the given recording of the set
     * \param text Identifying the recording to exchange
     * \param path Path to the (temporary) recording file
     * \return success
     */
    bool editRecording(const QString& text, const QString& path);

    /**
     * \brief Removes the given recording from the set
     * \param text The text said in the recording
     * \return success
     */
    bool removeRecording(const QString& text);

    /**
     * \brief Removes all associated samples
     * \return success
     */
    bool clear();

    int columnCount(const QModelIndex& = QModelIndex()) const { return 1; }
    int rowCount(const QModelIndex& = QModelIndex()) const { return m_recordings.count(); }
    QVariant data (const QModelIndex& index, int role=Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role=Qt::DisplayRole) const;
    QModelIndex index(int row, int column, const QModelIndex& parent=QModelIndex()) const;
    QModelIndex parent(const QModelIndex& index) const;
};

inline uint qHash(const RecordingSetText &key)
{
  return qHash(static_cast<const QString&>(key.toUpper()));
}

#endif

