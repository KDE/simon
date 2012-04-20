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

#ifndef SIMON_MODELCOMPILATIONADAPTER_H_166F0999AD87463EA4DE9A335364D7A6
#define SIMON_MODELCOMPILATIONADAPTER_H_166F0999AD87463EA4DE9A335364D7A6

#include <QObject>
#include <QProcess>
#include <QString>
#include <QHash>
#include "simonmodelcompilationmanagement_export.h"

#define ADAPT_CHECKPOINT if (!keepGoing) { emit adaptionAborted(); return false; }

/**
 *	@class ModelAdapter
 *	@brief Adapts the generic simon model to a more usable format for the underlying model compiler
 *
 *	@version 0.1
 *	@date 20.12.2009
 *	@author Peter Grasch
 */
class MODELCOMPILATIONMANAGEMENT_EXPORT ModelCompilationAdapter : public QObject
{
  Q_OBJECT
  
  signals:
    void status(QString, int progressNow, int progressMax);
    void error(QString);

    void adaptionComplete();
    void adaptionAborted();
    
  public:
    enum AdaptionType
    {
      None=0,
      AdaptLanguageModel=1,
      AdaptAcousticModel=2,
      AdaptIndependently=4
    };
    
    explicit ModelCompilationAdapter(const QString& userName, QObject *parent=0);

    QString getStatus() const { return currentStatus; }

    int wordCount() const { return m_wordCount; }
    int pronunciationCount() const { return m_pronunciationCount; }
    int sampleCount() const { return m_sampleCount; }

    void clearPoisonedPhonemes() { poisonedPhonemes.clear(); }
    void poisonPhoneme( const QString& phoneme ) { poisonedPhonemes << phoneme; }
    
    
    virtual bool startAdaption(AdaptionType adaptionType, const QStringList& scenarioPathsIn,
                               const QString& promptsIn, const QHash<QString, QString>& args)=0;

    void abort();

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

    QStringList poisonedPhonemes;
};
#endif
