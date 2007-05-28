//
// C++ Implementation: settingsview
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "settingsview.h"
#include "simoninfo.h"
/**
 * \brief Constructor
 * 
 * This instances a new dialog and connects the signals/slots
 * It also enables/disables the plattform dependant options (alsa/dsound)
 * 
 * \author Peter Grasch
 * 
 * \param QWidget *parent
 * The parent of the dialog
 */

SettingsView::SettingsView(QWidget *parent)
 : QDialog(parent)
{
	ui.setupUi(this);
	
	connect(ui.pbSystemSettings, SIGNAL(clicked()), this, SLOT(switchToSystem()));
	connect(ui.pbSoundSettings, SIGNAL(clicked()), this, SLOT(switchToSound()));
	connect(ui.pbCommandSettings, SIGNAL(clicked()), this, SLOT(switchToCommands()));
	connect(ui.pbProtocolSettings, SIGNAL(clicked ()), this, SLOT(switchToProtocols()));
	connect(ui.pbRevert, SIGNAL(clicked()), this, SLOT(switchToHistory()));
	connect(ui.pbApply, SIGNAL(clicked()), this, SLOT(apply()));
	connect(ui.pbConfirm, SIGNAL(clicked()), this, SLOT(apply()));
	sm=new SettingsManager();
    sm->loadFile();
	
    ui.cbChannels->clear();
    ui.cbChannels->addItem("Mono",1);
    ui.cbChannels->addItem("Stereo",2);
	this->readConfig();
#ifdef linux
    ui.lbDirectX->setVisible(false);
    ui.lbALSA->setVisible(true);
#endif
#ifdef __WIN32
	ui.lbDirectX->setVisible(true);
	ui.lbALSA->setVisible(false);
#endif
    this->switchToSystem();
}





/**********************************************/
/******************Tab Stuff*******************/
/**********************************************/

/**
 * \brief Untoggles all the "tab" buttons on top
 * 
 * \author Peter Grasch
 */
void SettingsView::unsetAllTabs()
{
	ui.pbSystemSettings->setChecked(false);
	ui.pbSoundSettings->setChecked(false);
	ui.pbCommandSettings->setChecked(false);
	ui.pbProtocolSettings->setChecked(false);
	ui.pbRevert->setChecked(false);
	
}

/**
 * \brief Switches to the "system" tab
 * 
 * \author Peter Grasch
 */
void SettingsView::readConfig()
{
	//ui.sbPort->setValue(sm->getPortNum()); 
    ui.cbAskBeforeExit->setCheckState((sm->getAskBeforeExit()) ? Qt::Checked : Qt::Unchecked);
    ui.cbStartSimonOnBoot->setCheckState((sm->getSimonAutoStart()) ? Qt::Checked : Qt::Unchecked);
    ui.cbStartJuliusdOnBoot->setCheckState((sm->getJuliusdAutoStart()) ? Qt::Checked : Qt::Unchecked);
    ui.cbStartJuliusAsNeeded->setCheckState((sm->getJuliusdRequired()) ? Qt::Checked : Qt::Unchecked);
    ui.leLexicon->setText(sm->getPathToLexicon());
    ui.leGrammar->setText(sm->getPathToGrammar());
    ui.leCommands->setText(sm->getPathToCommando());
    ui.leVocab->setText(sm->getPathToVocabul());
    ui.lePrompts->setText(sm->getPathToPrompts());
//     ui.leAddress->setText(sm->getIpAdress()); 
    
    
    	SoundDeviceList *sd=sm->getDevices();
    ui.cbDevice->clear();
    int defaultdevice=sm->getDefaultDevice();
    for (int i=0; i<sd->count(); i++)
    {
        QString deviceid= ((SoundDevice)sd->at(i)).getDeviceID();
        ui.cbDevice->addItem(((SoundDevice) sd->at(i)).getName(),deviceid);
        if (deviceid.toInt()==defaultdevice) ui.cbDevice->setCurrentIndex(ui.cbDevice->count());
        
    }
    
    ui.cbMixing->setCheckState((sm->getMixing()) ? Qt::Checked : Qt::Unchecked);
    ui.cbSaveAllRecordings->setCheckState((sm->getSaveAllRecordings()) ? Qt::Checked : Qt::Unchecked);
    ui.leSaveRecordingsTo->setText(sm->getPathToSaveRecordings());
    int channel=sm->getChannel();
    int j=0;
    while ((j<ui.cbChannels->count()) && (ui.cbChannels->itemData(j)!=channel))
    {
            j++;
    }
    if (j==ui.cbChannels->count())
    {
       QMessageBox::critical(this,"Lesen der Kanäle fehlgeschlagen","Beim Auslesen der Kanäle aus der Konfigurationsdatei ist ein Fehler aufgetreten");
       return;
    }
    ui.cbChannels->setCurrentIndex(j);
    ui.sbSamplerate->setValue(sm->getSamplerate());
    ui.hsMic->setValue(sm->getVolume());
   	

                    
}

void SettingsView::apply()
{
     bool simonautostart=false;
     bool juliusdautostart=false;
     bool juliusdrequired=false;
     bool askbeforeexit=false;
     bool mixing=false;
     bool saveallrecordings=false;
     if (ui.cbStartSimonOnBoot->checkState()==Qt::Checked) simonautostart=true;
     if (ui.cbStartJuliusdOnBoot->checkState()==Qt::Checked) juliusdautostart=true;
     if (ui.cbStartJuliusAsNeeded->checkState()==Qt::Checked) juliusdrequired=true;
     if (ui.cbAskBeforeExit->checkState()==Qt::Checked) askbeforeexit=true;
     if (ui.cbMixing->checkState()==Qt::Checked) mixing=true;
     if (ui.cbSaveAllRecordings->checkState()==Qt::Checked) saveallrecordings=true;
     
     sm->setSimonAutoStart(simonautostart);
     sm->setJuliusdAutostart(juliusdautostart);
     sm->setJuliusdRequired(juliusdrequired);
     sm->setAskBeforeExit(askbeforeexit);
     sm->setMixing(mixing);
     sm->setSaveAllRecordings(saveallrecordings);
     //sm->setPortNum(ui.sbPort->value());
     sm->setPathToLexicon(ui.leLexicon->text());
     sm->setPathToGrammar(ui.leGrammar->text());
     sm->setPathToVocabul(ui.leVocab->text());
     sm->setPathToPrompts(ui.lePrompts->text());
//      sm->setIpAdress(ui.leAddress->text());
     sm->setChannel(ui.cbChannels->itemData(ui.cbChannels->currentIndex(),Qt::UserRole).toString()); 
     sm->setPathToSaveRecordings(ui.leSaveRecordingsTo->text());
     sm->setSamplerate(ui.sbSamplerate->value());
     sm->setVolume(ui.hsMic->sliderPosition());
     sm->setDefaultDevice((ui.cbDevice->itemData(ui.cbDevice->currentIndex(),Qt::UserRole)).toString());
     
     int result=sm->saveSettings();
     if (result==0) QMessageBox::information(this,"Einstellungen gespeichert","Die geänderten Einstellungen wurden gespeichert");
     else QMessageBox::critical(this,"Fehler beim Speichern der Einstellungen","Beim Speichern der Einstellungen ist ein Fehler aufgetreten! Kontrollieren sie ob der Datenträger weder voll noch schreibgeschützt ist.");
    
     
     
     
     
}
void SettingsView::switchToSystem()
{
	unsetAllTabs();
	ui.pbSystemSettings->setChecked(true);
	ui.swSettings->setCurrentIndex( 0 );
     
}

/**
 * \brief Switches to the "sound" tab
 * 
 * \author Peter Grasch
 */
void SettingsView::switchToSound()
{
	unsetAllTabs();
    ui.pbSoundSettings->setChecked(true);
	ui.swSettings->setCurrentIndex( 1 );
	
}

/**
 * \brief Switches to the "command" tab
 * 
 * \author Peter Grasch
 */
void SettingsView::switchToCommands()
{
	unsetAllTabs();
	
	ui.pbCommandSettings->setChecked(true);
	ui.swSettings->setCurrentIndex( 2 );
}

/**
 * \brief Switches to the "command" tab
 * 
 * \author Peter Grasch
 */
void SettingsView::switchToProtocols()
{
	unsetAllTabs();
	
	ui.pbProtocolSettings->setChecked(true);
	ui.swSettings->setCurrentIndex( 3 );
}

/**
 * \brief Switches to the "history" tab
 * 
 * \author Peter Grasch
 */
void SettingsView::switchToHistory()
{
	unsetAllTabs();
	
	ui.pbRevert->setChecked(true);
	ui.swSettings->setCurrentIndex( 4 );
}


/**
 * \brief Destructor
 * 
 * \author Peter Grasch
 */
SettingsView::~SettingsView()
{
}


