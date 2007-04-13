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
     QString name="Ich bin eine Soundkarte";
     QString device="1.Soundkarte";
     SoundDeviceList *sdl= new SoundDeviceList();
     sdl->append(SoundDevice(device,name));
     qDebug()<<"Ich bin da "<<sdl->count();
     return sdl;           
}
bool SettingsManager::getMixing()
{
     return false;
}

bool SettingsManager::getSaveAllRecordings()
{
     return false;
}

QString SettingsManager::getPathToSaveRecordings()
{
     return ("genau hier");
}
int SettingsManager::getVolume()
{
    return (45);   
}
int SettingsManager::getChannel()
{
    return (2);
}

int SettingsManager::getSamplerate()
{
   return 564234;   
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
    return false;
}

bool SettingsManager::getJuliusdAutoStart()
{
    return false;
}

bool SettingsManager::getJuliusdRequired()
{
    return false;
}

bool SettingsManager::getAskBeforeExit()
{
     return false;
}
  
QString SettingsManager::getPathToConfig()
{
     return "BLABLA";
}

QString SettingsManager::getPathToLexicon()
{
     return "BLABLA";
}

QString SettingsManager::getPathToGrammar()
{
     return "BLABLA";
}

QString SettingsManager::getPathToCommando()
{
     return "BLABLA";
}

QString SettingsManager::getPathToVocabul()
{
     return "BLABLA";
}

QString SettingsManager::getPathToPrompts()
{
     return "BLABLA";
}



/**
 * \brief Destructor
 */
SettingsManager::~SettingsManager()
{
                                   
}




