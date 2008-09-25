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
