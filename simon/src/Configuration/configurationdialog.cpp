//
// C++ Implementation: configurationdialog
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "configurationdialog.h"
#include "coreconfiguration.h"
#include "generalsettings.h"
#include "internetextensionsettings.h"
#include "externalprogrammanager.h"
#include "passwordsettings.h"

#include "../SimonLib/Sound/soundsettings.h"
#include "../ModelManagement/modelsettings.h"
#include "../ModelManagement/Grammar/grammarsettings.h"
#include "../Actions/Commands/commandsettings.h"
#include "../RecognitionControl/networksettings.h"
#include <KConfigDialog>


ConfigurationDialog::ConfigurationDialog(QWidget *parent) : QObject(parent)
{
}

void ConfigurationDialog::show()
{
	if ( KConfigDialog::showDialog( "coreconfiguration" ) )
		return;

	QWidget *parentWidget = dynamic_cast<QWidget*>(parent());
	if (!parentWidget) return;

	KConfigDialog* dialog = new KConfigDialog(parentWidget, "coreconfiguration", CoreConfiguration::self());

	dialog->addPage( new GeneralSettings( parentWidget ), i18n("Allgemein"), "computer" ); 
	dialog->addPage( new InternetExtensionSettings( parentWidget ), i18n("Interneterweiterungen"), "document-open-remote" ); 
	dialog->addPage( new PasswordSettings( parentWidget ), i18n("Passwortschutz"), "document-encrypt" ); 
	dialog->addPage( new ModelSettings( parentWidget ), i18n("Modell"), "applications-education-language" ); 

	dialog->addPage( new GrammarSettings(parentWidget), i18n("Grammatik"), "user-properties" ); 
	
	dialog->addPage( new SoundSettings( parentWidget ), i18n("Sound"), "preferences-desktop-sound" ); 
	dialog->addPage( new NetworkSettings( parentWidget ), i18n("Netzwerk"), "network-disconnect" ); 
	dialog->addPage( new CommandSettings( parentWidget ), i18n("Kommandos"), "system-run" ); 
 	dialog->addPage( new ExternalProgramManager( parentWidget ), i18n("Externe Programme"), "applications-other" ); 
	
	//User edited the configuration - update your local copies of the 
	//configuration data 
	// connect( dialog, SIGNAL(settingsChanged()), 
	// 	this, SLOT(updateConfiguration()) ); 
	
	dialog->show();
}


ConfigurationDialog::~ConfigurationDialog()
{

}
