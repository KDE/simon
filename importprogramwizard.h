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


#include "simonwizard.h"

#include "selectprogrampage.h"
#include "configureprogrampage.h"
#include "importprogrampage.h"
#include "command.h"
#include "recwidget.h"
#include <QWizardPage>

/**
	@author Peter Grasch <bedahr@gmx.net>
*/
/**
 * \class $CLASSNAME
 * \author Peter Grasch
 * \date
 * \version 0.1
 * \brief
 */
class ImportProgramWizard : public SimonWizard
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
