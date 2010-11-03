/*
 *  Copyright (C) 2010 Peter Grasch <grasch@simon-listens.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2,
 *  or (at your option) any later version, as published by the Free
 *  Software Foundation
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details
 *
 *  You should have received a copy of the GNU General Public
 *  License along with this program; if not, write to the
 *  Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef SIMON_TESTRESULTwIDGET_H_4002119636CC42C68FE07273F9000A73
#define SIMON_TESTRESULTwIDGET_H_4002119636CC42C68FE07273F9000A73

#include "ui_testresult.h"
#include <QWidget>

class TestConfigurationWidget;
class QModelIndex;
class QSortFilterProxyModel;
class ModelTest;

class TestResultWidget : public QWidget
{
  Q_OBJECT
  signals:
    void status(const QString&, int progressNow, int progressTotal=100);
    void testStarted();
    void testComplete();
    void testAborted();


  public:
    enum TestState {
      Idle=0,
      Waiting=1,
      Running=2,
      Done=3
    };

  private:
    Ui::TestResultsView ui;
    TestState currentState;
    TestConfigurationWidget *config;
    ModelTest *modelTest;
    QSortFilterProxyModel *fileResultModelProxy;
    QVariant getSentenceResultValue(int row, int column);

  private slots:
    void slotFileResultSelected(const QModelIndex& index);
    void slotEditSelectedSample();
    void retrieveCompleteTestLog();
    void slotModelTestStatus(const QString& status, int now, int max);
    void slotModelTestRecognitionInfo(const QString& status);
    void slotModelTestError(const QString& error, const QByteArray&);
    void analyzeTestOutput();
    void displayRate(QProgressBar *pbRate, float rate);

    void slotModelTestAborted();
    void slotModelTestCompleted();

  public:
    TestResultWidget(TestConfigurationWidget *configuration, QWidget *parent=0);
    ~TestResultWidget();

    TestState getState() { return currentState; }

    QString getTag();

    float getAccuracy();
    float getWordErrorRate();
    float getConfidence();

    int getSubstitutionErrors();
    int getInsertionErrors();
    int getDeletionErrors();
    int getSentenceCount();

    void schedule();
    TestConfigurationWidget* getConfiguration() { return config; }


    QString getSentencePrompt(int i);
    int getSentenceCount(int i);
    QString getSentenceWER(int i);
    QString getSentenceAccuracy(int i);
    int getSentenceSubstitutionErrors(int i);
    int getSentenceInsertionErrors(int i);
    int getSentenceDeletionErrors(int i);

  public slots:
    void abort();
    void startTest();
};

#endif

