//
// C++ Implementation: settingsmanager
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "settingsmanager.h"


/**
 * \brief Constructor
 */
SettingsManager::SettingsManager()
{
                                  
}

SoundDeviceList* SettingsManager::getDevices()
{
     SoundControl *sc= new SoundControl();
     return sc->getDevices() ;           
}

int SettingsManager::getDefaultDevice()
{
    return settings->value("device").toInt(); 
}

bool SettingsManager::getMixing()
{
     int temp=settings->value("mixing").toInt();
     if (temp==0) return false;
     else if (temp==1) return true;
          else return false;
     
}


bool SettingsManager::getSaveAllRecordings()
{
     int temp=settings->value("saveallrecordings").toInt();
     if (temp==0) return false;
     else if (temp==1) return true;
          else return false;
}

QString SettingsManager::getPathToSaveRecordings()
{
     return settings->value("pathtosaverecordings");
}
int SettingsManager::getVolume()
{
    return settings->value("volume").toInt();  
}

int SettingsManager::getChannel()
{
    return settings->value("channel").toInt();  
}

int SettingsManager::getSamplerate()
{
   return settings->value("samplerate").toInt();  
}
void SettingsManager::loadFile()
{
    XMLSetting *xmls= new XMLSetting("conf/settings.xml");
    if(!xmls) return;
    
    xmls->load();
    settings=xmls->getSettings();
}
 
int SettingsManager::getPortNum()
{
    return settings->value("portnumber").toInt();
}

bool SettingsManager::getSimonAutoStart()
{
     int temp=settings->value("simonautostart").toInt();
     if (temp==0) return false;
     else if (temp==1) return true;
          else return false;
}

QString SettingsManager::getIpAdress()
{
    return settings->value("juliusdip");
}

bool SettingsManager::getJuliusdAutoStart()
{
     int temp=settings->value("juliusdautostart").toInt();
     if (temp==0) return false;
     else if (temp==1) return true;
          else return false;
}

bool SettingsManager::getJuliusdRequired()
{
     int temp=settings->value("juliusdrequired").toInt();
     if (temp==0) return false;
     else if (temp==1) return true;
          else return false;
}

bool SettingsManager::getAskBeforeExit()
{
     int temp=settings->value("askbeforeexit").toInt();
     if (temp==0) return false;
     else if (temp==1) return true;
          else return false;
}
  
QString SettingsManager::getPathToConfig()
{
     return settings->value("pathtoconfig");
}

QString SettingsManager::getPathToLexicon()
{
     return settings->value("pathtolexicon");
}

QString SettingsManager::getPathToGrammar()
{
     return settings->value("pathtogrammar");
}

QString SettingsManager::getPathToCommando()
{
     return settings->value("pathtocommando");
}

QString SettingsManager::getPathToVocabul()
{
     return settings->value("pathtovocabul");
}

QString SettingsManager::getPathToPrompts()
{
     return settings->value("pathtoprompts");
}



/**
 * \brief Destructor
 */
SettingsManager::~SettingsManager()
{
                                   
}




