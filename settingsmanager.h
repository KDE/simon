//
// C++ Interface: settingsmanager
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H
#include <QString>
#include <QDebug>
#include "sounddevice.h"
#include "soundcontrol.h"
#include "xmlsetting.h"
/**
	\class SettingsManager
	
	\brief The class provides all the functions to store/load/get the current config
	
	\version 0.1
	\author Peter Grasch
	\todo Implementing
*/
class SettingsManager{

private:
         QHash<QString,QString> *settings;

public:
    SettingsManager();
     void loadFile();
     int getPortNum();
     bool getSimonAutoStart();
     bool getJuliusdAutoStart();
     bool getJuliusdRequired();
     bool getAskBeforeExit();
     QString getPathToConfig();
     QString getPathToLexicon();
     QString getPathToGrammar();
     QString getPathToCommando();
     QString getPathToVocabul();
     QString getPathToPrompts();
     QString getIpAdress();
     SoundDeviceList* getDevices();
     bool getMixing();
     bool getSaveAllRecordings();
     int getChannel();
     QString getPathToSaveRecordings();
     int getSamplerate();
     int getVolume();
     int getDefaultDevice();
    ~SettingsManager();

};

#endif
