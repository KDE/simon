/*
 *   Copyright (C) 2009 Peter Grasch <peter.grasch@bedahr.org>
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

#ifndef SIMON_SHADOWVOCABULARY_H_44E21ED777FA42D69E673E47531E8898
#define SIMON_SHADOWVOCABULARY_H_44E21ED777FA42D69E673E47531E8898

#include "simonmodelmanagement_export.h"
#include "vocabulary.h"
#include <QObject>
#include <QDateTime>

class MODELMANAGEMENT_EXPORT ShadowVocabulary : public Vocabulary
{
    Q_OBJECT

  signals:
    void changed();

  private:
    QDateTime lastModifiedDate;
    void touch();

  protected:
    bool loadFailed;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;

  public:
    bool isNull() { return loadFailed; }
    QDateTime lastModified() { return  lastModifiedDate; }

    ShadowVocabulary(QObject *parent = 0);
    bool save();
    bool addWord(Word* w);
    bool addWords(QList<Word*> w);
    bool reOrder(Word* w);
    bool removeWord(Word* w, bool deleteWord=true);
    bool renameCategory(const QString& from, const QString& to);

    bool reset(QIODevice* data);
    bool empty();
};
#endif
