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

#ifndef SIMON_MODELMANAGERUIPROXY_H_F5A390FE23024F8488DE6F8D5FD90C70
#define SIMON_MODELMANAGERUIPROXY_H_F5A390FE23024F8488DE6F8D5FD90C70

#include <simonscenarios/modelmanager.h>

#include "simonmodelmanagementui_export.h"

/**
 *	@class ModelManagementUiProxy
 *	@brief Provides a "proxy" for the underlying modelmanager-concept-class which adds ui elements
 *
 *	@version 0.1
 *	@date 18.11.2008
 *	@author Peter Grasch
 */

class SIMONMODELMANAGEMENTUI_EXPORT ModelManagerUiProxy : public ModelManager
{
  Q_OBJECT

  signals:
    void recompileModel();

  private slots:
    void slotModelChanged();

  private:
    static ModelManagerUiProxy *instance;
    ModelManagerUiProxy(QObject *parent=0);

  public:
    static ModelManagerUiProxy* getInstance();

    ~ModelManagerUiProxy();

    bool storeBaseModel(const QDateTime& changedTime, int baseModelType,
      const QByteArray& container);
    bool storeLanguageDescription(const QDateTime& changedTime, QByteArray& shadowVocab,
      const QByteArray& treeHed, const QByteArray& languageProfile=QByteArray());
    bool storeTraining(const QDateTime& changedTime, qint32 sampleRate, const QByteArray& wavConfig,
      const QByteArray& prompts);
    bool storeActiveModel(const QDateTime& changedTime, qint32 sampleRate, const QByteArray& container);
    bool storeSample(const QByteArray& sample);

};
#endif
