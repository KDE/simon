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

#include "samui.h"
#include <QWidget>
#include <QProcess>
#include <QList>
#include <QDomElement>
#include <QModelIndex>
#include <kxmlguiwindow.h>
#include "testconfigurationwidget.h"

#include "ui_main.h"

class ModelCompiler;
class ModelCompilationAdapter;
class ModelCompilerHTK;
class ModelCompilationAdapterHTK;
class ModelCompilerSPHINX;
class ModelCompilationAdapterSPHINX;
class KProcess;
class KAction;
class ReportParameters;
class QCloseEvent;
class KCMultiDialog;
class ModelTest;
class QSortFilterProxyModel;
class TestConfigurationWidget;
class TestResultWidget;
class QwtBarsItem;
class QwtLegend;
class CorpusInformation;
class QDomDocument;

/**
 * @short Main view
 * @author Peter Grasch <grasch@simon-listens.org>
 * @version 0.1
 */

class SamView :  public KXmlGuiWindow, public SamUi
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

//    enum BackendType {SPHINX, JHTK};

  public slots:
    bool close();

  private slots:
    void setDirty();
    void setClean();

    void showConfig();

    void newProject();
    void load();
    void load(const QString& filename);
    void saveAs();
    void save();

    void updateWindowTitle();

    void parseFile();
    void storeFile();

    void compileModel();
    void getBuildPathsFromSimon();

    void testModel();

    void clearBuildLog();
    void slotModelCompilationFinished();
    void retrieveCompleteBuildLog();
    void storeBuildLog();

    QString getTargetDirectory();
    void switchToAdapt();
    void serializePrompts();
    void importRecognitionSamples();
    void serializeScenarios();

    void addTestConfiguration();
    void addTestConfiguration(TestConfigurationWidget* configuration);
    void testConfigurationRemoved();
    void testResultRemoved();

    void serializePromptsRun(const QString promptsPath, const QString& output);
    void serializeScenariosRun(const QStringList& scenarioIds, const QString& output);

    void slotModelAdaptionComplete();
    void slotModelAdaptionAborted();
    void slotModelAdaptionStatus(QString status, int progress, int max);
    void slotModelAdaptionError(QString errorMessage);

    void abortModelCompilation();
    void slotModelCompilationStatus(const QString& status, int now, int max);
    void slotModelCompilationError(const QString& error);
    void slotModelCompilationClassUndefined(const QString&);
    void slotModelCompilationWordUndefined(const QString&);
    void slotModelCompilationPhonemeUndefined(const QString&);

    void abortModelTest();
    void subTestStarted();
    void subTestAborted();
    void subTestComplete();
    void allTestsFinished();
    void switchToTestResults();
    void clearCurrentConfiguration();
    void clearTest();
    void testConfigTagChanged();

    void exportTestResults();
    void backendChanged(int id);

  private:
    /**
     * \brief Start the model test the next time the model finished compiling
     */
    bool m_startCompilationAfterAdaption;
    bool m_startTestAfterCompile;
    bool m_startTestAfterAdaption;
    bool m_exportAfterTest;

    bool m_dirty;

    TestConfigurationWidget::BackendType backendType;

    QString m_filename;
    CorpusInformation *m_creationCorpus;
    ReportParameters *m_reportParameters;
    Ui::MainWindow ui;

    ModelCompiler *modelCompiler;
    ModelCompiler *modelCompilerHTK;
    ModelCompiler *modelCompilerSPHINX;
    ModelCompilationAdapter *modelCompilationAdapter;
    ModelCompilationAdapter *modelCompilationAdapterHTK;
    ModelCompilationAdapter *modelCompilationAdapterSPHINX;
    //yah it isn't look good but we need to have saved state of the both versions of adapter & compiler

    QList<TestConfigurationWidget*> testConfigurations;
    QList<TestResultWidget*> testResults;

    QwtBarsItem *barGraph;
    QwtLegend *barGraphLegend;

    QStringList findScenarios(const QStringList& ids);
    int getModelType();

    void displayModelTestStatus();
    void startNextScheduledTest();

    void initGraph();

    QString m_User;

    CorpusInformation* createEmptyCorpusInformation();
    ReportParameters* createEmptyReportParameters();

    QList<CorpusInformation*> creationCorpusInformation();
    bool askIfQuit();
};

#endif

