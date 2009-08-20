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


#ifndef SAMVIEW_H
#define SAMVIEW_H

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
class ModelTest;
/**
 * This is the main view class for sam.  Most of the non-menu,
 * non-toolbar, and non-statusbar (e.g., non frame) GUI code should go
 * here.
 *
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

    void slotModelCompilationStatus(const QString& status, int now, int max);
    void slotModelCompilationError(const QString& error);
    void slotModelCompilationClassUndefined(const QString&);
    void slotModelCompilationWordUndefined(const QString&);
    void slotModelCompilationPhonemeUndefined(const QString&);

    void slotModelTestStatus(const QString& status, int now, int max);
    void slotModelTestRecognitionInfo(const QString& status);
    void slotModelTestError(const QString& error);
    void switchToTestResults();

    void analyzeTestOutput();

    void slotFileResultSelected(QModelIndex index);
    void slotEditSelectedSample();

private:
    QString m_filename;
    Ui::MainWindow ui;
    ModelCompilationManager *modelCompilationManager;
    ModelTest *modelTest;
};

#endif // samVIEW_H
