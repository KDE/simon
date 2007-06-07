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
     
void SettingsManager::setPortNum(int portnum)
{
     settings.setValue("portnum",portnum);
}
     
void SettingsManager::setSimonAutoStart(bool simonautostart)
{
     settings.setValue("simonautostart",simonautostart); 
}
     
void SettingsManager::setJuliusdAutostart(bool juliusdautostart)
{
     settings.setValue("juliusdautostart",juliusdautostart); 
}
     
void SettingsManager::setJuliusdRequired(bool juliusdrequired)
{
     settings.setValue("juliusdrequired",juliusdrequired); 
}
     
void SettingsManager::setAskBeforeExit(bool askbeforeexit)
{
     settings.setValue("askbeforeexit",askbeforeexit); 
}
     
void SettingsManager::setPathToLexicon(QString pathtolexicon)
{
     settings.setValue("pathtolexicon",pathtolexicon); 
}
     
void SettingsManager::setPathToGrammar(QString pathtogrammar)
{
     settings.setValue("pathtogrammar",pathtogrammar); 
}
     
void SettingsManager::setPathToCommando(QString pathtocommando)
{
     settings.setValue("pathtocommando",pathtocommando); 
}
     
void SettingsManager::setPathToVocabul(QString pathtovocabul)
{
     settings.setValue("pathtovocabul",pathtovocabul); 
}
     
void SettingsManager::setPathToPrompts(QString pathtoprompts)
{
     settings.setValue("pathtoptompts",pathtoprompts); 
}
     
void SettingsManager::setIpAdress(QString ipadress)
{
     settings.setValue("juliusdip",ipadress); 
}
     
void SettingsManager::setMixing(bool mixing)
{
     settings.setValue("mixing",mixing); 
}
     
void SettingsManager::setSaveAllRecordings(bool saveallrecordings)
{
     settings.setValue("saveallrecordings",saveallrecordings); 
}
     
void SettingsManager::setChannel(QString channel)
{
     settings.setValue("channel",channel); 
}
     
void SettingsManager::setPathToSaveRecordings(QString pathtosaverecordings)
{
     settings.setValue("pathtosaverecordings",pathtosaverecordings);      
}

void SettingsManager::setSamplerate(int samplerate)
{
     settings.setValue("samplerate",samplerate); 
}
     
void SettingsManager::setVolume(int volume)
{
     settings.setValue("volume",volume); 
}
     
void SettingsManager::setDefaultDevice(QString defaultdeviceid)
{
     settings.setValue("device",defaultdeviceid); 
}

SoundDeviceList* SettingsManager::getInputDevices()
{
     SoundControl *sc= new SoundControl();
     return sc->getInputDevices() ;
}

SoundDeviceList* SettingsManager::getOutputDevices()
{
     SoundControl *sc= new SoundControl();
     return sc->getOutputDevices() ;
}

int SettingsManager::getDefaultDevice()
{
    return settings.value("device").toInt(); 
}

bool SettingsManager::getMixing()
{
     return settings.value("mixing").toBool();    
}

bool SettingsManager::getSaveAllRecordings()
{
     return settings.value("saveallrecordings").toBool();
 }

QString SettingsManager::getPathToSaveRecordings()
{
     return settings.value("pathtosaverecordings").toString();
}

int SettingsManager::getVolume()
{
    return settings.value("volume").toInt();  
}

int SettingsManager::getChannel()
{
    return settings.value("channel").toInt();  
}

int SettingsManager::getSamplerate()
{
   return settings.value("samplerate").toInt();  
}

void SettingsManager::loadFile()
{
    settings.setPath(QSettings::IniFormat,QSettings::UserScope,"settings.ini");
}

int SettingsManager::saveSettings()
{
    settings.sync();
     return 0;
}

int SettingsManager::getPortNum()
{
    return settings.value("portnumber").toInt();
}

bool SettingsManager::getSimonAutoStart()
{
     return settings.value("simonautostart").toBool();
     
}

QString SettingsManager::getIpAdress()
{
    return settings.value("juliusdip").toString();
}

bool SettingsManager::getJuliusdAutoStart()
{
     return settings.value("juliusdautostart").toBool();
    
}

bool SettingsManager::getJuliusdRequired()
{
     return settings.value("juliusdrequired").toBool();
  
}

bool SettingsManager::getAskBeforeExit()
{
     return settings.value("askbeforeexit").toBool();
}

QString SettingsManager::getPathToLexicon()
{
     return settings.value("pathtolexicon").toString();
}

QString SettingsManager::getPathToGrammar()
{
     return settings.value("pathtogrammar").toString();
}

QString SettingsManager::getPathToCommando()
{
     return settings.value("pathtocommando").toString();
}

QString SettingsManager::getPathToVocabul()
{
     return settings.value("pathtovocabul").toString();
}

QString SettingsManager::getPathToPrompts()
{
     return settings.value("pathtoprompts").toString();
}



/**
 * \brief Destructor
 */
SettingsManager::~SettingsManager()
{
                                   
}




