/*
 *   Copyright (C) 2011 Adam Nash <adam.t.nash@gmail.com>
 *   Copyright (C) 2012 Peter Grasch <peter.grasch@bedahr.org>
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

#ifndef SIMON_PROMPTSTABLE_H
#define SIMON_PROMPTSTABLE_H

/** \file promptstable.h
 * \brief The file containing the PromptsTable baseclass header.
 */

#include <QHash>
#include <QStringList>
#include "simonscenarios_export.h"
#include "promptstablemodel.h"

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

class SIMONSCENARIOS_EXPORT PromptsTable
{
friend class PromptsTableModel;
public:
  PromptsTable();
  bool init(const QString& path);
  bool save(const QString& path);
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

  bool merge(const PromptsTable& other);

  QHash<QString,QString> samples() const { return m_wordBySample; }
  QList<QString> keys() const { return m_samples; }
  QList<QString> words() const { return m_wordBySample.values(); }
  QStringList sampleGroups() const { return m_groupBySample.values(); }
  QString sample(int idx) const { return m_samples[idx]; }

  PromptsTableModel* getModel();

private:
  QStringList m_samples;
  QHash<QString,QString> m_wordBySample;
  QHash<QString,QString> m_groupBySample;

  PromptsTableModel *m_model;

  void updateModel();
};

#endif // PROMPTSTABLE_H
