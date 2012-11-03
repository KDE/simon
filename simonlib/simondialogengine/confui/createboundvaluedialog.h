/*
 *   Copyright (C) 2010 Peter Grasch <peter.grasch@bedahr.org>
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


#ifndef SIMON_CREATEBOUNDVALUEDIALOG_H_4B4956DCAE204C49977297D20CB81F09
#define SIMON_CREATEBOUNDVALUEDIALOG_H_4B4956DCAE204C49977297D20CB81F09

#include <KDialog>

//#ifdef USE_PLASMA
//#include <KService>
//#endif

namespace Ui
{
  class CreateBoundValueDialog;
}

#ifdef USE_PLASMA
namespace Plasma
{
  class DataEngineManager;
  //class DataEngine::Data;
}

#include <Plasma/DataEngine>
#endif

class BoundValue;

class CreateBoundValueDialog : protected KDialog
{
  Q_OBJECT
  private:
    Ui::CreateBoundValueDialog *ui;

    BoundValue* createBoundValueInstance();
#ifdef USE_PLASMA
    bool m_requestingSource;
    QString m_currentEngineName;
    Plasma::DataEngine *m_currentEngine;
    Plasma::DataEngineManager *m_engineManager;
    //QList<KService::Ptr> m_dataEngines;

    void initPlasma();
#endif

    void initToBoundValue(BoundValue *init);

  private slots:
    void test();
#ifdef USE_PLASMA
    void initDataEngine();
    void sourceAdded(const QString& source);
    void sourceRemoved(const QString& source);
    void requestSource();
    void addPlasmaSource(const QString source);

    void dataUpdated(const QString& source, const Plasma::DataEngine::Data& data);
#endif

  public:
    CreateBoundValueDialog(QWidget *parent=0);
    virtual ~CreateBoundValueDialog();


    BoundValue *createBoundValue(BoundValue *init=0);
};

#endif


