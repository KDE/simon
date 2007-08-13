//
// C++ Implementation: generalsettings
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "generalsettings.h"
#include <QInputDialog>
#include <QVariant>
#include "settings.h"

/**
 * \brief Constructs a new GeneralSettings object
 * \author Peter Grasch
 * @param parent The parent of the widget
 */
GeneralSettings::GeneralSettings(QWidget* parent): SystemWidget(tr("Allgemeine Einstellungen"), QIcon(":/images/icons/computer.svg"), tr("Grundlegende Einstellungen rund um Simon"), parent)
{
	ui.setupUi(this);
	hide();
	connect ( ui.pbAddAdress, SIGNAL ( clicked() ), this, SLOT ( addAddress() ) );
	connect ( ui.pbDeleteAdress, SIGNAL ( clicked() ), this, SLOT ( deleteAddress() ) );
	
	//set help
	help = tr("Hier können Sie grundlegende Einstellungen rund um Simon verändern.\n\nDazu zählen: Pfade, Sicherheitsabfragen und Verbindungseinstellungen um zu Juliusd zu verbinden.\n\nEinstellungen in diesem Modul können kritisch für die Verwendung von simon sein.\n\nsimon verwendet für die Erkennung selbst die Open Source Spracherkennungs-engine \"Julius\".\nDie Verbindung zu Julius wird über das Netzwerk aufgebaut.\nDie Addresse zum Juliusd wird entweder mit einem Hostnamen oder einer direkten \"IP Adresse\" angegeben (4x3 stellige Netzwerksidentifikationsnummer - xxx.xxx.xxx.xxx)\nDie Port Nummer muss beim Juliusd und bei simon gleich sein.\nWenn das Häckchen \"Starte juliusd wenn nötig\" aktiviert ist, wird versucht juliusd lokal zu starten, sollte zu keinem laufenden Dämon verbunden werden können.");
}



bool GeneralSettings::apply()
{
	//general
	Settings::set("SimonAutostart", ui.cbStartSimonOnBoot->isChecked());
	Settings::set("StartJuliusdWhenRequired", ui.cbStartJuliusAsNeeded->isChecked());
	Settings::set("AskBeforeExit", ui.cbAskBeforeExit->isChecked());

	//juliusd
	Settings::set("Network/JuliusdAddress", ui.cbAddress->currentText());

	//paths
	Settings::set("PathToCommands", ui.leCommands->text());
	Settings::set("PathToLexicon", ui.leLexicon->text());
	Settings::set("PathToGrammar", ui.leGrammar->text());
	Settings::set("PathToPrompts", ui.lePrompts->text());
	Settings::set("PathToVocab", ui.leVocab->text());

	return true;
}

bool GeneralSettings::reset()
{
	return init();
}

bool GeneralSettings::init()
{
	//general
	ui.cbStartSimonOnBoot->setChecked(Settings::get("SimonAutostart").toBool());
	ui.cbStartJuliusAsNeeded->setChecked(Settings::get("StartJuliusdWhenRequired").toBool());
	ui.cbAskBeforeExit->setChecked(Settings::get("AskBeforeExit").toBool());

	//juliusd
	ui.cbAddress->clear();
	ui.cbAddress->addItem(Settings::get("Network/JuliusdAddress").toString());

	//paths
	ui.leCommands->setText(Settings::get("PathToCommands").toString());
	ui.leLexicon->setText(Settings::get("PathToLexicon").toString());
	ui.leGrammar->setText(Settings::get("PathToGrammar").toString());
	ui.lePrompts->setText(Settings::get("PathToPrompts").toString());
	ui.leVocab->setText(Settings::get("PathToVocab").toString());
	
	return true;
}


void GeneralSettings::deleteAddress()
{
	ui.cbAddress->removeItem ( ui.cbAddress->currentIndex() );
}

void GeneralSettings::addAddress()
{
	QString host="";
	QString port="";
	bool ok=false;
	host=QInputDialog::getText ( this, tr("Neue Adresse Hinzufügen"),tr("Host:"), QLineEdit::Normal,"localhost", &ok );
	if ( ok && !host.isEmpty() )
	{
		ok=false;
		port=QInputDialog::getText ( this, tr("Neue Adresse Hinzufügen"),tr("Portnummer:"), QLineEdit::Normal, "4444",&ok );
		if ( ok )
		{
			if ( !port.isEmpty() ) ui.cbAddress->addItem ( host+":"+port );
			else ui.cbAddress->addItem ( host+":4444" );
		}
	}
}

GeneralSettings::~GeneralSettings()
{
}


