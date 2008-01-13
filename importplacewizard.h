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
#include "introplacepage.h"
#include "localplacepage.h"
#include "remoteplacepage.h"
#include "configureplacepage.h"
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
 * \date 03.09.2007
 * \version 0.1
 * \brief With this wizard the user can add a local or a remote place to the commands.
 */
class ImportPlaceWizard : public QWizard
{
    Q_OBJECT
		
	private:
        IntroPlacePage* introPlacePage;
        LocalPlacePage* localPlacePage;
        RemotePlacePage* remotePlacePage;
        ConfigurePlacePage* configurePlacePage;
        ImportPlacePage* importPlacePage; 
    
		IntroPlacePage* createIntroPlacePage();
        LocalPlacePage* createLocalPlacePage();
        RemotePlacePage* createRemotePlacePage();
        ConfigurePlacePage* createConfigurePlacePage();
        ImportPlacePage* createImportPlacePage();
        QWizardPage* createFinishedPage();
    
        int oldId;
    
public:
        ImportPlaceWizard(QWidget* parent=0);

        ~ImportPlaceWizard();

private slots:
    void idChanged(int newId);
    void placePreselectionChanged();
    void changeName(bool change);

signals:
   void commandCreated(Command* command);

};

#endif
