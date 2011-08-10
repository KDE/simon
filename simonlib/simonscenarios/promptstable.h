/*
 *   Copyright (C) 2011 Adam Nash <adam.t.nash@gmail.com>
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

#ifndef PROMPTSTABLE_H
#define PROMPTSTABLE_H

/** \file promptstable.h
 * \brief The file containing the PromptsTable baseclass header.
 */

#include <QObject>
#include <QHash>
#include <QFile>
#include "simonmodelmanagement_export.h"

/**
 *	@class PromptsTable
 *	@brief The PromptsTable class provides a convenient container for prompts file data.
 *
 *
 *	\sa TrainingManager
 *
 *	@version 0.1
 *	@date 8.9.2011
 *	@author Adam Nash
 */

class Word;

class MODELMANAGEMENT_EXPORT PromptsTable : public QObject
{
    Q_OBJECT
public:
    explicit PromptsTable(QString filePath, QObject *parent = 0);

    bool init();
    bool save();
    bool deletePrompt(QString key);
    bool deleteWord(Word *w);
    bool deleteWord(const QString & word);
    void insert(const QString& sample, const QString& sampleGroup, const QString& word);
    bool clear();
    bool contains(const QString& key);
    int remove(const QString& key);
    int count();
    QString value(const QString& key);
    QString sampleGroup(const QString& key);
    void setFileName(QString fileName);

    QList<QString> keys() { return m_wordBySample.keys(); }
    QList<QString> words() { return m_wordBySample.values(); }

private:
    QString m_filePath;
    QHash<QString,QString> m_wordBySample;
    QHash<QString,QString> m_groupBySample;

signals:

public slots:

};

#endif // PROMPTSTABLE_H
