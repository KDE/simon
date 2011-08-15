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
#include <QTextStream>
#include <QCoreApplication>
#include <QVariant>
#include <QTime>
#include <QRegExp>
#include <KLocalizedString>
#include <KStandardDirs>
#include <KDateTime>
#include <KLocale>
#include <math.h>

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
  PromptsTable *promptsTable = new PromptsTable(KStandardDirs::locate("appdata", "model/prompts"));
  if (promptsTable)
  {
    delete this->m_promptsTable;
    m_promptsTable = promptsTable;
    m_dirty=false;
    return true;
  }
  else
    return false;
}


void TrainingManager::trainingSettingsSaved()
{
  KConfig config( KStandardDirs::locateLocal("appdata", "model/modelsrcrc"), KConfig::SimpleConfig );
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

  return m_promptsTable->deleteWord(w);
}


bool TrainingManager::deleteWord(const QString& word)
{
  return m_promptsTable->deleteWord(word);
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

  if (!m_promptsTable->save()) return false;

  if (m_dirty)
  {
      m_wordRelevance.clear();                        // drop probability cache
      emit trainingDataChanged();
  }

  m_dirty=false;

  return true;
}


/**
 * \brief Saves any given promptstable to the given path. To save the main promptstable use savePrompts instead
 * \author Peter Grasch
 * \param prompts The promptstable to store
 * \param path Where to store the file
 * \return Success
 */
bool TrainingManager::writePromptsFile(PromptsTable* prompts, QString path)
{
  //FIXME: does trainingdate get written?

  prompts->setFileName(path);
  return prompts->save();
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
    return new PromptsTable(promptspath);
}


bool TrainingManager::refreshTraining(int sampleRate, const QByteArray& prompts)
{
  QMutexLocker lock(&m_promptsLock);

  SpeechModelManagementConfiguration::setModelSampleRate(sampleRate);

  QFile promptsF(KStandardDirs::locateLocal("appdata", "model/prompts"));
  if (!promptsF.open(QIODevice::WriteOnly))
    return false;

  promptsF.write(prompts);
  promptsF.close();

  // 	emit promptsChanged();
  init();

  return true;
}


/**
 * \brief checks if all words are in the dict and returns those that aren't
 * \author Susanne Tschernegg, Peter Grasch
 * @return Missing words
 */
QStringList TrainingManager::missingWords(const QStringList& prompts)
{
  QStringList strListAllWords;
  for ( int x=0; x<prompts.count(); x++ ) {
    QStringList strList = prompts[x].split ( ' ' );
    int strListSize = strList.size();
    for ( int y=0; y < strListSize; y++ ) {
      QString word = strList.at ( y );

      word.remove ( '.' );
      word.remove ( ',' );
      word.remove ( '(' );
      word.remove ( ')' );
      word.remove ( '?' );
      word.remove ( '!' );
      word.remove ( '"' );
      word.remove ( '/' );
      word.remove ( '\\' );
      word.remove ( '[' );
      word.remove ( ']' );
      word = word.trimmed();

      //if (!WordListManager::getInstance()->mainWordListContainsStr(word, Qt::CaseInsensitive))
      if (!ScenarioManager::getInstance()->getCurrentScenario()->containsWord(word)) {
        if (!strListAllWords.contains(word))
          strListAllWords.append ( word );
      }
    }
  }
  return strListAllWords;
}


/**
 * \brief Calculates the relevance of the given text with the given wordlist
 * \author Peter Grasch, Susanne Tschernegg
 * @param text The text to check
 * @param wlist The wordlist as reference
 * @return An index of how well simon would recognize it - the lower the worse
 */
float TrainingManager::calcRelevance ( const TrainingText *text )
{
  QString currPage;
  QStringList words;
  int wordCount=0;
  int probability=0;
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

      probability += getProbability ( currentWord );
    }
  }
  m_promptsLock.unlock();

  if ( wordCount > 0 )
    return qRound ( probability/wordCount );
  else return 0;
}


/**
 * \brief Returns the probability of the name (it is pulled out of the m_promptsTable)
 * \author Peter Grasch
 * \param QString wordname
 * Name of the word
 * \return int
 * Probability to recognize; The higher the more likely simon will recognize this word correctly
 */
int TrainingManager::getProbability ( QString wordname )
{
  if (!m_promptsTable) init();

  wordname = wordname.toUpper();
  if (m_wordRelevance.contains(wordname))
    return m_wordRelevance.value(wordname);

  QStringList prompts = m_promptsTable->words();
  int prob=0;
  int i=0;
  int promptscount=prompts.count();

  while ( i<promptscount ) {
    //prob += prompts.at ( i ).count(QRegExp(QString("( |^)%1( |$)").arg(wordname)));

    //work around some strange problem in my qt version...
    QString prompt = prompts.at(i);
    prob += prompt.count(' '+wordname+' ');
    if (prompt == wordname) prob++;
    else if (prompt.startsWith(wordname+' ')) prob++;
    else if (prompt.endsWith(' '+wordname)) prob++;

    i++;
  }

  m_wordRelevance.insert(wordname, prob);
  return prob;
}


bool TrainingManager::addSample (const QString &fileBaseName, const QString &sampleGroup, const QString &prompt)
{
  if (!m_promptsTable) init();

  if (m_promptsTable->contains(fileBaseName))
    return false;

  m_promptsTable->insert(fileBaseName, sampleGroup, prompt.toUpper());

  QStringList words = prompt.split(' ');
  foreach (const QString& word, words)
    m_promptsTable->deletePrompt(word);                   //removed cached recognition rates

  m_dirty=true;
  return true;
}


/*
bool TrainingManager::removeSample(const QString& fileBaseName)
{
  if (!m_promptsTable) init();

  if (m_promptsTable->remove(fileBaseName) > 0)
  {
    m_dirty=true;
    return true;
  } else return false;
}
*/

bool TrainingManager::clear()
{
  if (!m_promptsTable) init();

  return m_promptsTable->clear();
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
