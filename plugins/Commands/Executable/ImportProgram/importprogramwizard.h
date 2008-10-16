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

#ifndef IMPORTPROGRAMWIZARD_H
#define IMPORTPROGRAMWIZARD_H

#include <QWizard>

class Command;
class QWizardPage;
class SelectProgramPage;

/**
 * \class ImportProgramWizard
 * \author Peter Grasch
 * \date 16.08.2007
 * \version 0.1
 * \brief This wizard is used to add a existing program to the commands. The Wizard will find out the programs, which are installed on the computer.
  *         The programs are also divided into categories like "office", "games", resp.
 */
class ImportProgramWizard : public QWizard
{
	Q_OBJECT
		
private:
	SelectProgramPage* selector;
	QWizardPage* createIntroPage();
	SelectProgramPage* createSelectProgramPage();
        QWizardPage* createFinishedPage();
    
public:
        ImportProgramWizard(QWidget* parent=0);

        ~ImportProgramWizard();

private slots:
    void import(int status);

signals:
   void commandCreated(Command* command);

};

#endif
