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

#include "trainingmanager.h"

#include <simonlogging/logger.h>
#include "speechmodelmanagementconfiguration.h"
#include "scenariomanager.h"
#include "scenario.h"

#include <QFile>
#include <QDir>
#include <QStringList>
#include <QString>
#include <QObject>
#include <QCoreApplication>
#include <KI18n/klocalizedstring.h>

#include <KDateTime>
#include <math.h>
#include <QStandardPaths>
#include <KConfigCore/KConfigGroup>

TrainingManager* TrainingManager::m_instance;

/**
 * @brief Constructor
 *
 *	@author Peter Grasch
 */
TrainingManager::TrainingManager(QObject* parent) : QObject(parent),
m_dirty(false),
m_trainingTexts(new TrainingList()),
m_promptsLock(QMutex::Recursive),
m_promptsTable(0)
{
}


bool TrainingManager::init()
{
  //init prompts
  QMutexLocker lock(&m_promptsLock);
  PromptsTable *promptsTable = new PromptsTable();
  if (promptsTable->init(QStandardPaths::locate(QStandardPaths::DataLocation, "model/prompts"))) {
    delete this->m_promptsTable;
    m_promptsTable = promptsTable;
    m_dirty = false;
    return true;
  }

  delete promptsTable;
  return false;
}


void TrainingManager::trainingSettingsSaved()
{
    KConfig config( QStandardPaths::writableLocation(QStandardPaths::DataLocation) + QLatin1Char('/') + "model/modelsrcrc", KConfig::SimpleConfig) ;
  KConfigGroup cGroup(&config, "");
  cGroup.writeEntry("TrainingDate", KDateTime::currentUtcDateTime().dateTime());
  config.sync();

  emit trainingSettingsChanged();
}


TrainingManager* TrainingManager::getInstance()
{
  if (!m_instance) {
    m_instance = new TrainingManager();
    connect(qApp, SIGNAL(aboutToQuit()), m_instance, SLOT(deleteLater()));
  }
  return m_instance;
}


/**
 * \brief Deletes the samples containing the given word
 * \author Peter Grasch
 * @param w The word to remove
 * @return Success
 */
bool TrainingManager::deleteWord ( Word *w )
{
  if (!m_promptsTable) init();

  return (m_promptsTable->deleteWord(w) && savePrompts());
}


bool TrainingManager::deleteWord(const QString& word)
{
  return (m_promptsTable->deleteWord(word) && savePrompts());
}


/**
 * \brief Deletes the prompt corresponding to the key and the sample
 * \author Peter Grasch
 * @param key The key to delete
 * @return success
 */
bool TrainingManager::deletePrompt ( QString key )
{
  if (!m_promptsTable) init();

  if (m_promptsTable->deletePrompt(key))
  {
      m_dirty = true;
      return true;
  }

  return false;
}


QString TrainingManager::getTrainingDir()
{
  QString dir = SpeechModelManagementConfiguration::modelTrainingsDataPath().toLocalFile();
  if (!dir.endsWith('/')
    #ifdef Q_OS_WINDOWS
    && !dir.endsWith('\\')
    #endif
    )
    dir += QDir::separator();

  QDir d(dir);
  if (!d.exists()) d.mkpath(dir);

  return dir;
}


/**
 * \brief Saves the current promptstable
 * \author Peter Grasch
 * @return Success
 */
bool TrainingManager::savePrompts()
{
  QMutexLocker lock(&m_promptsLock);

  if (!m_promptsTable) init();

  if (!m_promptsTable->save(QStandardPaths::writableLocation(QStandardPaths::DataLocation) + QLatin1Char('/') + "model/prompts")) return false;

  if (m_dirty)
  {
      m_wordRelevance.clear();                        // drop probability cache

      //Update the training date and signal a change in the data
      KConfig config( QStandardPaths::writableLocation(QStandardPaths::DataLocation) + QLatin1Char('/') + "model/modelsrcrc", KConfig::SimpleConfig) ;
      KConfigGroup cGroup(&config, "");
      cGroup.writeEntry("TrainingDate", KDateTime::currentUtcDateTime().dateTime());
      config.sync();
      emit trainingDataChanged();
  }

  m_dirty=false;

  return true;
}

PromptsTable* TrainingManager::getPrompts()
{
  if (!m_promptsTable) init();
  return this->m_promptsTable;
}


/**
 * \brief Builds and returns the promptstable by parsing the file at the given path
 * \author Peter Grasch
 * \param QString promptspath
 * The path to the prompts file
 * \return PromptsTable*
 * The table
 */
PromptsTable* TrainingManager::readPrompts ( QString promptspath )
{
  PromptsTable *t =  new PromptsTable();
  if (t->init(promptspath))
    return t;
  delete t;
  return 0;
}


bool TrainingManager::refreshTraining(int sampleRate, const QByteArray& prompts)
{
  QMutexLocker lock(&m_promptsLock);

  SpeechModelManagementConfiguration::setModelSampleRate(sampleRate);

  QFile promptsF(QStandardPaths::writableLocation(QStandardPaths::DataLocation) + QLatin1Char('/') + "model/prompts");
  if (!promptsF.open(QIODevice::WriteOnly))
    return false;

  promptsF.write(prompts);
  promptsF.close();

  // 	emit promptsChanged();
  init();

  return true;
}

/**
 * \brief Calculates the relevance of the given text
 * \author Peter Grasch, Susanne Tschernegg
 * @param text The text to check
 * @return An index of how well simon would recognize it - the lower the worse
 */
float TrainingManager::calcRelevance ( const TrainingText *text )
{
  QString currPage;
  QStringList words;
  int wordCount=0;
  int frequency=0;
  int curWordCount=0;

  m_promptsLock.lock();
  int textWordCount=text->getPageCount();
  for ( int i=0; i<textWordCount; i++ ) {
    currPage = text->getPage ( i );
    currPage.remove ( '.' );
    currPage.remove ( ',' );
    currPage.remove ( '?' );
    currPage.remove ( '!' );
    currPage.remove ( '"' );
    currPage.remove ( '/' );
    currPage.remove ( '[' );
    currPage.remove ( ']' );

    words = currPage.split ( ' ' );

    curWordCount = words.count();

    wordCount += curWordCount;

    for ( int j=0; j<curWordCount; j++ ) {
      QString currentWord = words[j];

      frequency += getFrequency ( currentWord );
    }
  }
  m_promptsLock.unlock();

  if ( wordCount > 0 )
      return qRound ( ((qreal)frequency)/((qreal)wordCount) );
  else return 0;
}


/**
 * \brief Returns the frequency of the name (it is pulled out of the m_promptsTable)
 * \author Peter Grasch
 * \param QString wordname
 * Name of the word
 * \return int
 * Frequency to recognize; The higher the more likely simon will recognize this word correctly
 */
int TrainingManager::getFrequency ( QString wordname )
{
  if (!m_promptsTable) init();

  wordname = wordname.toUpper();
  if (m_wordRelevance.contains(wordname))
    return m_wordRelevance.value(wordname);

  QStringList prompts = m_promptsTable->words();
  int freq=0;
  int i=0;
  int promptscount=prompts.count();

  while ( i<promptscount ) {
    //work around some strange problem in my qt version...
    QString prompt = prompts.at(i);
    freq += prompt.count(' '+wordname+' ');
    if (prompt == wordname) freq++;
    else if (prompt.startsWith(wordname+' ')) freq++;
    else if (prompt.endsWith(' '+wordname)) freq++;

    i++;
  }

  m_wordRelevance.insert(wordname, freq);
  return freq;
}


bool TrainingManager::addSample (const QString &fileBaseName, const QString &sampleGroup, const QString &prompt)
{
  if (!m_promptsTable) init();

  if (m_promptsTable->contains(fileBaseName))
    return false;

  m_promptsTable->insert(fileBaseName, sampleGroup, prompt.toUpper());

  QStringList words = prompt.split(' ');
  foreach (const QString& word, words)
    m_wordRelevance.remove(word.toUpper());                   //removed cached recognition rates

  m_dirty=true;
  return true;
}

bool TrainingManager::mergePrompts(const PromptsTable& table)
{
  if (!m_promptsTable) init();

  m_dirty = true;

  bool succ = m_promptsTable->merge(table);

  foreach (const QString& word, table.words())
    m_wordRelevance.remove(word);                   //removed cached recognition rates
  return succ;
}

bool TrainingManager::clear()
{
  if (!m_promptsTable) init();

  return (m_promptsTable->clear() && savePrompts());
}


/**
 * @brief Destructor
 *
 *	@author Peter Grasch
 */
TrainingManager::~TrainingManager()
{
  qDeleteAll(*m_trainingTexts);
  delete m_trainingTexts;
}
