/*
 *   Copyright (C) 2009 Peter Grasch <grasch@simon-listens.org>
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

#ifndef SIMON_GRAMMAR_H_8D43507AA9744D548F7B18569DCD0971
#define SIMON_GRAMMAR_H_8D43507AA9744D548F7B18569DCD0971
#include <QStringList>
#include <QMutex>
#include <QAbstractItemModel>

#include "simonmodelmanagement_export.h"
#include <simonscenariobase/scenarioobject.h>

class Scenario;

class MODELMANAGEMENT_EXPORT Grammar : public ScenarioObject, public QAbstractItemModel
{

  private:
    QMutex structuresLock;
    QStringList m_structures;

    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int, Qt::Orientation orientation,
      int role = Qt::DisplayRole) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QModelIndex index(int row, int column,const QModelIndex &parent = QModelIndex()) const;

  protected:
    Grammar(Scenario *parent);

    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;

  public:
    Grammar();
    static Grammar* createGrammar(Scenario *parent, const QDomElement&);
    bool deSerialize(const QDomElement&);
    QDomElement serialize(QDomDocument *doc);
    static QDomElement createEmpty(QDomDocument *doc);
    bool renameTerminal(QString terminal, const QString& newName);

    QString getExampleSentence(const QString& terminal);

    QString getStructure(int index);

    bool addStructures(const QStringList& newStructures, bool save=true);
    bool addStructure(const QString& newStructure, bool save=true);
    bool deleteStructure(int index);

    QStringList getTerminals();
    QStringList getStructures() { return m_structures; }

    int structureCount() { return m_structures.count(); }
};
#endif
