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

class QDomDocument;

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
class SIMONTTS_EXPORT RecordingSet
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

  public:
    RecordingSet();
    RecordingSet(int id, const QString& name);

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
     * \brief Returns the path of recording of the given text
     * \return Full path to the file or QString() if not found
     */
    QString getPath(const QString& text);

    /*
     * Getter method for @sa m_isNull
     * \return m_isNull
     */
    bool isNull() { return m_isNull; }

    /*
     * Getter method for @sa m_id
     * \return m_id
     */
    int id() { return m_id; }

    /*
     * Getter method for @sa m_name
     * \return m_name
     */
    QString name()  { return m_name; }

    /**
     * \brief Setter method for @sa m_name
     * \return Success (in this implementation always true
     */
    bool rename(const QString& newName);
};

inline uint qHash(const RecordingSetText &key)
{
  return qHash(static_cast<const QString&>(key.toUpper()));
}

#endif
