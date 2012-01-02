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

#ifndef SIMON_TRAININGMANAGER_H_0648DFA07DE1482CBF6ECD50E4B89EDD
#define SIMON_TRAININGMANAGER_H_0648DFA07DE1482CBF6ECD50E4B89EDD

#include "word.h"
#include "trainingtext.h"
#include <QHash>
#include <QMutex>
#include <QList>
#include "simonmodelmanagement_export.h"
#include "promptstable.h"

/**
 *	@class TrainingManager
 *	@brief Class to handle the process of training the language model
 *
 *	@version 0.1
 *	@date 23.01.2006
 *	@author Peter Grasch
 */

typedef  QList<TrainingText*> TrainingList;

class TrainingManager;

class MODELMANAGEMENT_EXPORT TrainingManager : public QObject
{
  Q_OBJECT
    private:
    bool m_dirty;

    static TrainingManager *m_instance;
    TrainingList *m_trainingTexts;

    QHash<QString,int> m_wordRelevance;             /// Stores the relevance of words (caching for getProbability)

    QMutex m_promptsLock;
    PromptsTable *m_promptsTable;

    signals:
    void addMissingWords(QStringList words);
    void trainingFinished();
    void trainingDataChanged();
    void trainingSettingsChanged();

  protected:
    TrainingManager(QObject *parent=0);

  public:
    PromptsTable* getPrompts();

    static TrainingManager* getInstance();

    bool init();

    bool deletePrompt ( QString key );

    int getProbability ( QString name );
    PromptsTable* readPrompts (QString pathToPrompts);

    bool deleteWord(const QString& word);
    bool deleteWord ( Word *w );
    bool savePrompts();

    bool refreshTraining(int sampleRate, const QByteArray& prompts);

    bool addSample(const QString& fileBaseName, const QString& sampleGroup, const QString& prompt);
    //		bool removeSample(const QString& fileBaseName);
    bool clear();

    QStringList missingWords(const QStringList& prompts);

    QString getPage ( int i );

    float calcRelevance ( const TrainingText *text );

    QString getTrainingDir();

    void trainingSettingsSaved();

    ~TrainingManager();

};
#endif
