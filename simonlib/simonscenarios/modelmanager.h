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

#ifndef SIMON_MODELMANAGER_H_7D8BF260D23E4A83BD8404866C3FFDA1
#define SIMON_MODELMANAGER_H_7D8BF260D23E4A83BD8404866C3FFDA1

#include "simonmodelmanagement_export.h"
#include <QObject>
#include <QDateTime>
#include <QStringList>

class Model;
class WordListContainer;
class GrammarContainer;
class LanguageDescriptionContainer;
class TrainingContainer;

class MODELMANAGEMENT_EXPORT ModelManager : public QObject
{
  Q_OBJECT

    signals:
  void modelChanged();

  private:
    bool inGroup;
    bool modelChangedFlag;

    QStringList missingFiles;
    bool hasTraining();
    bool hasLanguageDescription();
    bool hasActiveContainer();

  public slots:
    void modelHasChanged();

  public:
    ModelManager(QObject *parent=0);
    void startGroup();
    void commitGroup(bool silent=false);

    Model* createBaseModelContainer();
    QDateTime getBaseModelDate();
    virtual bool storeBaseModel(const QDateTime& changedTime, int baseModelType,
      const QByteArray& hmmDefs, const QByteArray& tiedList,
      const QByteArray& macros, const QByteArray& stats);

    LanguageDescriptionContainer* getLanguageDescriptionContainer();
    QDateTime getLanguageDescriptionModifiedTime();
    void touchLanguageDescription();
    virtual bool storeLanguageDescription(const QDateTime& changedTime, QByteArray& shadowVocab,
      const QByteArray& treeHed, const QByteArray& languageProfile=QByteArray());

    TrainingContainer* getTrainingContainer();
    QDateTime getTrainingModifiedTime();
    virtual bool storeTraining(const QDateTime& changedTime, qint32 sampleRate, const QByteArray& wavConfig,
      const QByteArray& prompts);

    Model* createActiveContainer();
    qint32 getActiveModelSampleRate();
    QDateTime getActiveContainerModifiedTime();
    virtual bool storeActiveModel(const QDateTime& changedTime, qint32 sampleRate, const QByteArray& hmmDefs,
      const QByteArray& tiedList, const QByteArray& dict, const QByteArray& dfa);

    void buildMissingSamplesList();
    QByteArray getSample(const QString& sampleName);
    QString missingSample();
    virtual bool storeSample(const QByteArray& sample);

    virtual ~ModelManager() {}

};
#endif
