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
     settings->insert("portnum",QString::number(portnum));
}
     
void SettingsManager::setSimonAutoStart(bool simonautostart)
{
     settings->insert("simonautostart",simonautostart ? "1" : "0"); 
}
     
void SettingsManager::setJuliusdAutostart(bool juliusdautostart)
{
     settings->insert("juliusdautostart",juliusdautostart ? "1" : "0"); 
}
     
void SettingsManager::setJuliusdRequired(bool juliusdrequired)
{
     settings->insert("juliusdrequired",juliusdrequired ? "1" : "0"); 
}
     
void SettingsManager::setAskBeforeExit(bool askbeforeexit)
{
     settings->insert("askbeforeexit",askbeforeexit ? "1" : "0"); 
}
     
void SettingsManager::setPathToLexicon(QString pathtolexicon)
{
     settings->insert("pathtolexicon",pathtolexicon); 
}
     
void SettingsManager::setPathToGrammar(QString pathtogrammar)
{
     settings->insert("pathtogrammar",pathtogrammar); 
}
     
void SettingsManager::setPathToCommando(QString pathtocommando)
{
     settings->insert("pathtocommando",pathtocommando); 
}
     
void SettingsManager::setPathToVocabul(QString pathtovocabul)
{
     settings->insert("pathtovocabul",pathtovocabul); 
}
     
void SettingsManager::setPathToPrompts(QString pathtoprompts)
{
     settings->insert("pathtoptompts",pathtoprompts); 
}
     
void SettingsManager::setIpAdress(QString ipadress)
{
     settings->insert("juliusdip",ipadress); 
}
     
void SettingsManager::setMixing(bool mixing)
{
     settings->insert("mixing",mixing ? "1" : "0"); 
}
     
void SettingsManager::setSaveAllRecordings(bool saveallrecordings)
{
     settings->insert("saveallrecordings",saveallrecordings ? "1" : "0"); 
}
     
void SettingsManager::setChannel(QString channel)
{
     settings->insert("channel",channel); 
}
     
void SettingsManager::setPathToSaveRecordings(QString pathtosaverecordings)
{
     settings->insert("pathtosaverecordings",pathtosaverecordings);      
}

void SettingsManager::setSamplerate(int samplerate)
{
     settings->insert("samplerate",QString::number(samplerate)); 
}
     
void SettingsManager::setVolume(int volume)
{
     settings->insert("volume",QString::number(volume)); 
}
     
void SettingsManager::setDefaultDevice(QString defaultdeviceid)
{
     settings->insert("device",defaultdeviceid); 
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
    XMLSetting *xmls= new XMLSetting();
    if(!xmls) return;
    xmls->loadSettings();
    settings=xmls->getSettings();
}

int SettingsManager::saveSettings()
{
    XMLSetting *xmls= new XMLSetting();
    //if(!xmls) return;
    return xmls->saveSettings(this->settings);
     
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




