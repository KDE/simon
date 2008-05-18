//
// C++ Interface: importprogramwizard
//
// Description:
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//




#ifndef IMPORTPROGRAMWIZARD_H
#define IMPORTPROGRAMWIZARD_H


#include <QWizard>

class Command;
class QWizardPage;
class SelectProgramPage;
class ConfigureProgramPage;
class ImportProgramPage;

/**
 * \class ImportProgramWizard
 * \author Peter Grasch
 * \date 16.08.2007
 * \version 0.1
 * \brief This wizard is used to add a existing program to the commands. The Wizard will find out the programs, which are installed on the computer.
  *         The programs are also devided into categories like "office", "games", resp.
 */
class ImportProgramWizard : public QWizard
{
	Q_OBJECT
		
private:
	QWizardPage* createIntroPage();
	SelectProgramPage* createSelectProgramPage();
        ConfigureProgramPage* createConfigureProgramPage();
        ImportProgramPage* createImportProgramPage();
        QWizardPage* createFinishedPage();

        int oldId;
    
public:
        ImportProgramWizard(QWidget* parent=0);

        ~ImportProgramWizard();

private slots:
    void idChanged(int newId);
    void changeName(bool change);

signals:
   void commandCreated(Command* command);

};

#endif
