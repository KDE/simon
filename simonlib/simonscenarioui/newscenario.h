/*
 *   Copyright (C) 2008 Peter Grasch <peter.grasch@bedahr.org>
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

#ifndef SIMON_NEWSCENARIO_H_CFC94622A0054FCBB56958AF2C9BF205
#define SIMON_NEWSCENARIO_H_CFC94622A0054FCBB56958AF2C9BF205

#include <KDialog>
#include <QList>
#include "ui_modifyscenario.h"

class Scenario;
class Author;
class VersionNumber;

class NewScenario : protected KDialog
{

  Q_OBJECT

  private:
    Ui::ModifyScenarioDlg ui;
    QList<Author*> m_authors;

    QString createId();
    void updateAuthorDisplay();

    int exec();
    void displayScenario(Scenario *s);
    VersionNumber* getMinimumVersion();

  private slots:
    void setWindowTitleToScenarioName(QString name);
    void checkIfComplete();
    void addAuthor();
    void removeAuthor();

  public:
    NewScenario(QWidget *parent=0);
    ~NewScenario();

    Scenario* newScenario();
    Scenario* editScenario(Scenario *s);
    bool editScenario(const QString& id);

};
#endif
