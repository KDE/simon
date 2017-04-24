/*
 *  Copyright (C) 2010 Peter Grasch <peter.grasch@bedahr.org>
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

#ifndef SIMON_EXPORTTESTRESULTS_H_4002119636CC42C68FE07273F9000A73
#define SIMON_EXPORTTESTRESULTS_H_4002119636CC42C68FE07273F9000A73

#include "samui.h"
#include <QHash>
#include "corpusinformation.h"
#include "ui_exporttestresultsdlg.h"
#include <QDialog>

class ReportParameters;
class TestResultWidget;
class CorpusInformationWidget;
class TemplateValueList;
class ModelCompiler;

class ExportTestResults : public QDialog, public SamUi
{
  Q_OBJECT

  private:
    Ui::ExportTestDlg ui;
    QList<TestResultWidget*> testResults;
    QList<CorpusInformationWidget*> m_creationCorporaWidgets;
    QList<CorpusInformationWidget*> m_testCorporaWidgets;
    ModelCompiler *m_compiler;

    QHash<QString, QString> createTemplateValues();
    QList<TemplateValueList*> createTemplateValueLists();

    TemplateValueList* extractCorpusTemplateInformation(const QString& id, QList<CorpusInformationWidget*> w);
    

    QString getSelectedTemplate();

    void initTemplates();
    void clearCorpora();
    void createReport();
    QList<CorpusInformation*> getCorpusInformation(const QList<CorpusInformationWidget*>& widgets);
    QList<CorpusInformation*> getTestCorpusInformation();
    QList<CorpusInformation*> getTrainingCorpusInformation();

    void displayCorpora(QTabWidget* tableWidget, QList<CorpusInformationWidget*>& list,
        const QList<CorpusInformation*>& corpora);

    QString printPercentage(float rate);

  private slots:
    void initSystemDefinition();

  protected:
    int exec();

  public:
    ExportTestResults(QWidget *parent);
    ~ExportTestResults();
    bool exportTestResults(ReportParameters *reportParameters, QList<CorpusInformation*> creationCorpora,
                            QList<TestResultWidget*> testResults, ModelCompiler *compiler);
    void saveCorporaInformation();
    ReportParameters *getReportParameters();

};

#endif

