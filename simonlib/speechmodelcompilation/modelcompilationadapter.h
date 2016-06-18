/*
 *   Copyright (C) 2008 Peter Grasch <peter.grasch@bedahr.org>
 *   Copyright (C) 2012 Vladislav Sitalo <root@stvad.org>
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

#ifndef SIMON_MODELCOMPILATIONADAPTER_H_166F0999AD87463EA4DE9A335364D7A6
#define SIMON_MODELCOMPILATIONADAPTER_H_166F0999AD87463EA4DE9A335364D7A6

#include "simonmodelcompilation_export.h"
#include "modelcompilation.h"
#include <simonscenarios/vocabulary.h>
#include <simonscenarios/grammar.h>

#include <QObject>
#include <QString>
#include <QHash>
#include <QSharedPointer>


#define ADAPT_CHECKPOINT if (!keepGoing) { emit adaptionAborted(ModelCompilation::Manual); return false; }

/**
 *	@class ModelAdapter
 *	@brief Adapts the generic simon model to a more usable format for the underlying model compiler
 *
 *	@version 0.1
 *	@date 20.12.2009
 *	@author Peter Grasch
 *
 *  General logic & data moved from modelcompilationadapterhtk.
 *  @version 0.2
 *  @date 14.08.2012
 *  @author Vladislav Sitalo
 */
class SIMONMODELCOMPILATION_EXPORT ModelCompilationAdapter : public QObject
{
  Q_OBJECT

signals:
  void status(QString, int progressNow, int progressMax);
  void error(QString);

  void adaptionComplete();
  void adaptionAborted(ModelCompilation::AbortionReason);

public:
  /*!
   * \brief The AdaptionType enum.
   *  Model adapter have different behavior depends on this type.
   *
   */
  enum AdaptionType
  {
    None=0,
    AdaptLanguageModel=1,
    AdaptAcousticModel=2,
    AdaptIndependently=4 /**< Used to set that we must adapt model "as is" (without checking for compliance with the acoustic model) */
  };

  explicit ModelCompilationAdapter(const QString& userName, QObject *parent=0);

  QString getStatus() const { return currentStatus; }

  int wordCount() const { return m_wordCount; }
  int pronunciationCount() const { return m_pronunciationCount; }
  int sampleCount() const { return m_sampleCount; }

  void clearPoisonedPhonemes() { m_poisonedPhonemes.clear(); }
  void poisonPhoneme( const QString& phoneme ) { m_poisonedPhonemes << phoneme; }


  /*!
   * \brief startAdaption Function used to adapt model with given parameters.
   * \param adaptionType Adaptation type.
   * \param scenarioPathsIn List of scenarios, used to create model.
   * \param promptsIn Path to prompts file.
   * \param args List of parameters specific for different specialization of this class.
   *
   * \return Succes status.
   */
  virtual bool startAdaption(AdaptionType adaptionType, const QStringList& scenarioPathsIn,
                             const QString& promptsIn, const QHash<QString, QString>& args)=0;

  void abort();

  QStringList getDroppedTranscriptions() const { return m_droppedTranscriptions; }

  QString promptsPath() const { return m_promptsPathOut; }


protected:
  bool keepGoing;
  AdaptionType m_adaptionType;
  QString currentStatus;
  QStringList m_scenarioPathsIn;
  QString m_promptsPathIn;
  QString m_userName;

  int m_wordCount;
  int m_pronunciationCount;
  int m_sampleCount;

  QStringList m_poisonedPhonemes;
  QStringList m_droppedTranscriptions;

  QString m_promptsPathOut;

  bool removeContextAdditions(AdaptionType adaptionType);

  /*!
   * \brief mergeInputData Forms vocabulary and grammar from given list of scenarios.
   * \param scenarioPaths Scenarios list.
   * \param mergedVocabulary Vocabulary.
   * \param mergedGrammar Grammar.
   */
  void mergeInputData(const QStringList& scenarioPaths, QSharedPointer<Vocabulary> mergedVocabulary,
                      QSharedPointer<Grammar> mergedGrammar);

  void removeWordsWithPoisonedPhonems(QSharedPointer<Vocabulary> vocabulary);

  bool containsPoisonedPhoneme(const QString &pronunciation);

  bool readPrompts(ModelCompilationAdapter::AdaptionType adaptionType, QSharedPointer<Vocabulary> vocabulary,
                   const QString &promptsPathIn, QStringList &trainedVocabulary);


};
#endif
