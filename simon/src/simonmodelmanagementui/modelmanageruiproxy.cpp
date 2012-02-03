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

#include "modelmanageruiproxy.h"
#include "AddWord/addwordview.h"
#include <QCoreApplication>
#include <KMessageBox>
#include <KDebug>
#include <KStandardDirs>
#include <KLocalizedString>
#include <simonscenarios/trainingmanager.h>

ModelManagerUiProxy* ModelManagerUiProxy::instance;

ModelManagerUiProxy::ModelManagerUiProxy(QObject *parent) : ModelManager(parent)
{
  connect (this, SIGNAL(modelChanged()), this, SLOT(slotModelChanged()));
}

ModelManagerUiProxy* ModelManagerUiProxy::getInstance()
{
  if (!instance) {
    instance = new ModelManagerUiProxy();
    connect(qApp, SIGNAL(aboutToQuit()), instance, SLOT(deleteLater()));
  }
  return instance;
}

void ModelManagerUiProxy::slotModelChanged()
{
  emit recompileModel();
}


bool ModelManagerUiProxy::storeBaseModel(const QDateTime& changedTime, int baseModelType,
const QByteArray& hmmDefs, const QByteArray& tiedList,
const QByteArray& macros, const QByteArray& stats)
{
  bool succ = ModelManager::storeBaseModel(changedTime, baseModelType, hmmDefs, tiedList, macros, stats);
  if (!succ) {
    KMessageBox::sorry(0, i18nc("%1 is path", "Could not store the base model received from the server."
      "\n\nPlease check the permissions on the model folder: %1",
      KStandardDirs::locateLocal("appdata", "model")));
  }
  return succ;
}


bool ModelManagerUiProxy::storeLanguageDescription(const QDateTime& changedTime, QByteArray& shadowVocab,
const QByteArray& treeHed, const QByteArray& languageProfile)
{
  bool succ = ModelManager::storeLanguageDescription(changedTime, shadowVocab, treeHed, languageProfile);
  if (!succ) {
    KMessageBox::sorry(0, i18nc("%1 is path", "Could not store the language description received from the server."
      "\n\nPlease check the permissions on the model folder: %1",
      KStandardDirs::locateLocal("appdata", "model")));
  }
  return succ;
}


bool ModelManagerUiProxy::storeTraining(const QDateTime& changedTime, qint32 sampleRate, const QByteArray& wavConfig,
const QByteArray& prompts)
{
  bool succ = ModelManager::storeTraining(changedTime, sampleRate, wavConfig, prompts);
  if (!succ) {
    KMessageBox::sorry(0, i18nc("%1 is path", "Could not store the training corpus received from the server."
      "\n\nPlease check the permissions on the model folder: %1",
      KStandardDirs::locateLocal("appdata", "model")));
  }
  return succ;
}


bool ModelManagerUiProxy::storeActiveModel(const QDateTime& changedTime, qint32 sampleRate, const QByteArray& hmmDefs,
const QByteArray& tiedList, const QByteArray& dict, const QByteArray& dfa)
{
  bool succ = ModelManager::storeActiveModel(changedTime, sampleRate, hmmDefs, tiedList, dict, dfa);
  if (!succ) {
    KMessageBox::sorry(0, i18nc("%1 is path", "Could not store the active model received from the server."
      "\n\nPlease check the permissions on the model folder: %1",
      KStandardDirs::locateLocal("appdata", "model")));
  }
  return succ;
}


bool ModelManagerUiProxy::storeSample(const QByteArray& sample)
{
  bool succ = ModelManager::storeSample(sample);
  if (!succ) {
    KMessageBox::sorry(0, i18nc("%1 is sample path, %2 is training data folder",
      "Could not store the sample %1 received from the server."
      "\n\nPlease check the permissions on the sample folder: %2",
      missingSample(), TrainingManager::getInstance()->getTrainingDir()));
  }
  return succ;
}


ModelManagerUiProxy::~ModelManagerUiProxy()
{
  instance = 0;
}
