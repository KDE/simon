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

#ifndef SIMON_SIMONVIEW_H_FAA1A01DFF1E4DA098606C3E951E43AD
#define SIMON_SIMONVIEW_H_FAA1A01DFF1E4DA098606C3E951E43AD

/**
 *	@class SimonView
 *	@brief The Main UI Class
 *
 *	This class is not only the "main" UI-Class but also the interface to the
 *	underlying main concept class.
 *
 *	@version 0.1
 *	@date 07.01.2006
 *	@author Peter Grasch
 */

#include "ui_simonview.h"
#include <KXmlGuiWindow>
#include <simoncontrol.h>
#include <simonscenarios/scenariodisplay.h>

#include <QList>
#include <QMutex>

#ifdef DEBUG_TEST_SYNCHRONIZATION
#include <QTimer>
#endif

#include <KHTMLPart>
// #include "operation.h"

class QPoint;
class QCloseEvent;
class QWidget;
class InlineWidgetView;
class QString;
class SimonControl;
class SimonInfo;
class TrainingView;
class AddWordView;
class TrayIconManager;
class RunCommandView;
class QAction;
class GrammarView;
class KCMultiDialog;
class KAction;
class Operation;
class QThread;
class VocabularyView;
class KComboBox;
class KHTMLPart;
class WelcomeHTMLPart;

class SimonView : public KXmlGuiWindow, public ScenarioDisplay
{

  Q_OBJECT

    private:
  #ifdef DEBUG_TEST_SYNCHRONIZATION
    QTimer t;
    int wordI;
  #endif
    WelcomeHTMLPart *welcomePart;
    QMutex guiUpdateMutex;

    bool settingsShown;
    int shownDialogs;
    QPoint currentPos;
    QPoint addWordDlgPos;

    KAction *disconnectAction;
    KAction *activateAction, *connectAction;

    Ui::MainWindow ui;                            //!< Mainwindow UI definition - made by uic from the QTDesigner .ui
    SimonControl *control;                        //!< Pointer to the main concept class
    TrayIconManager *trayManager;                 //!< Handles the TrayIcon
    VocabularyView *vocabularyView;
    GrammarView *grammarView;                     //!< Pointer on the Dialog "WordList"
    RunCommandView *runDialog;                    //!< Pointer on the Dialog "RunCommand"
    TrainingView *trainDialog;                    //!< Pointer on the Dialog "Training"
    KCMultiDialog *configDialog;

    KComboBox *cbCurrentScenario;

    void setupSignalSlots();
    void setupActions();

  protected:
    void displayAboutPage();
    void displayScenarioPrivate(Scenario *scenario);

  private slots:
    void manageScenarios();
    void updateScenarioDisplays();
    void updateActionList();
    void displayScenarios();
    void showVolumeCalibration();

  #ifdef DEBUG_TEST_SYNCHRONIZATION
    void testSlot();
  #endif

  public slots:
    void displayConnectionStatus(const QString &status);

    void closeSimon();

    void toggleConnection();
    void displayError(const QString& error);

    void toggleActivation();
    void representState(SimonControl::SystemStatus status);
    
    void showAddWordDialog();
    void showRunDialog();
    void showTrainDialog();
    void showWordListDialog();
    void showGrammarDialog();
    void showSystemDialog();

  public:
    explicit SimonView(QWidget *parent = 0, Qt::WFlags flags = 0);

    ~SimonView();

    void closeEvent ( QCloseEvent * event );

};

class WelcomeHTMLPart : public KHTMLPart
{
  public:
    WelcomeHTMLPart(QWidget *parentWidget, QObject *parent);
    bool urlSelected(const QString& url, int button, int state,
      const QString& _target, const KParts::OpenUrlArguments& args = KParts::OpenUrlArguments(),
      const KParts::BrowserArguments& browserArgs = KParts::BrowserArguments());

};
#endif
