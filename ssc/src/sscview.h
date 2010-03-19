/*
 *  Copyright (C) 2008 Peter Grasch <grasch@simon-listens.org>
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


#ifndef SIMON_SSCVIEW_H_07C486DEC8C343548F4D205AB7A8C741
#define SIMON_SSCVIEW_H_07C486DEC8C343548F4D205AB7A8C741

#include <KXmlGuiWindow>
#include <QProcess>
#include "ui_main.h"

class User;
class KAction;
class QCloseEvent;
class KCMultiDialog;
class TrainingsWizard;
/**
 * @short Main view
 * @author Peter Grasch <grasch@simon-listens.org>
 * @version 0.1
 */

class SSCView : public KXmlGuiWindow
{
    Q_OBJECT
public:
    /**
     * Default constructor
     */
    SSCView(QWidget* parent=0);

    /**
     * Destructor
     */
    virtual ~SSCView();

    void setupActions();

    User* retrieveUser();
    void displayUser(User*);

private slots:
    void showConfigurationDialog();
    void connectToServer();
    void disconnectFromServer();
    void displayConnectionStatus(const QString &status);

    void connected();
    void disconnected();
    void displayError(const QString& error);
    void displayWarning(const QString& warning);

    void getUser();

    void addUser();
    void deleteUser();
    void userDetails();

    void listUsers();
    void listInstitutions();

    void findUser();

    void repeat();
    void training();
    void interview();

private:
    Ui::SampleCollectorUi ui;
    //TrainingsWizard *trainingsWizard;

    User* getInstituteSpecificUser();
};

#endif // ksimondVIEW_H
