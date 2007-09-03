//
// C++ Interface: importplacewizard
//
// Description:
//
//
// Author: Susanne Tschernegg
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef IMPORTPLACEWIZARD_H
#define IMPORTPLACEWIZARD_H

#include <QWizard>
#include "chooseplacepage.h"
#include "localplacepage.h"
#include "remoteplacepage.h"
#include "importplacepage.h"
#include "command.h"
#include "recwidget.h"
#include <QWizardPage>

/**
	@author Susanne Tschernegg
*/
/**
 * \class $CLASSNAME
 * \author Susanne Tschernegg
 * \date
 * \version 0.1
 * \brief
 */
class ImportPlaceWizard : public QWizard
{
    Q_OBJECT
		
	private:
		QWizardPage* createIntroPage();
		ChoosePlacePage* createChoosePlacePage();
        LocalPlacePage* createLocalPlacePage();
        RemotePlacePage* createRemotePlacePage();
        ImportPlacePage* createImportPlacePage();
        QWizardPage* createFinishedPage();
    
        int oldId;
    
public:
        ImportPlaceWizard(QWidget* parent=0);

        ~ImportPlaceWizard();

private slots:
    void idChanged(int newId);

signals:
   void commandCreated(Command* command);

};

#endif
