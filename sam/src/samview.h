/*
 *  Copyright (C) 2009 Peter Grasch <grasch@simon-listens.org>
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

#ifndef SIMON_SAMVIEW_H_4002119636CC42C68FE07273F9000A73
#define SIMON_SAMVIEW_H_4002119636CC42C68FE07273F9000A73

#include <QWidget>
#include <QProcess>
#include <QModelIndex>
#include <kxmlguiwindow.h>

#include "ui_main.h"

class KProcess;
class KAction;
class QCloseEvent;
class KCMultiDialog;
class ModelCompilationManager;
class ModelCompilationAdapter ;
class ModelTest;
class QSortFilterProxyModel;

/**
 * @short Main view
 * @author Peter Grasch <grasch@simon-listens.org>
 * @version 0.1
 */

class SamView :  public KXmlGuiWindow
{
  Q_OBJECT
    public:
    /**
     * Default constructor
     */
    SamView(QWidget *parent, Qt::WFlags flags);

    /**
     * Destructor
     */
    virtual ~SamView();

  private slots:
    void showConfig();

    void newProject();
    void load();
    void saveAs();
    void save();

    void updateWindowTitle();

    void parseFile();
    void storeFile();

    void compileModel();
    void getBuildPathsFromSimon();

    void testModel();

    void retrieveCompleteBuildLog();
    void retrieveCompleteTestLog();

    QString getTargetDirectory();
    void switchToAdapt();
    void serializePrompts();
    void serializeScenarios();

    void serializePromptsRun(const QString promptsPath, const QString& output);
    void serializeScenariosRun(const QStringList& scenarioIds, const QString& output);

    void slotModelAdaptionComplete();
    void slotModelAdaptionAborted();
    void slotModelAdaptionStatus(QString status, int progress);
    void slotModelAdaptionError(QString errorMessage);

    void abortModelCompilation();
    void slotModelCompilationStatus(const QString& status, int now, int max);
    void slotModelCompilationError(const QString& error);
    void slotModelCompilationClassUndefined(const QString&);
    void slotModelCompilationWordUndefined(const QString&);
    void slotModelCompilationPhonemeUndefined(const QString&);

    void abortModelTest();
    void slotModelTestStatus(const QString& status, int now, int max);
    void slotModelTestRecognitionInfo(const QString& status);
    void slotModelTestError(const QString& error, const QByteArray&);
    void switchToTestResults();

    void analyzeTestOutput();

    void slotFileResultSelected(QModelIndex index);
    void slotEditSelectedSample();

    void exportTestResults();

  private:
    QString m_filename;
    Ui::MainWindow ui;
    ModelCompilationManager *modelCompilationManager;
    ModelCompilationAdapter *modelCompilationAdapter;
    ModelTest *modelTest;
    QSortFilterProxyModel *fileResultModelProxy;

    QStringList findScenarios(const QStringList& ids);
    int getModelType();
};
#endif                                            // samVIEW_H
