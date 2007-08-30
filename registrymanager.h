//
// C++ Interface: registrymanager
//
// Description: 
//
//
// Author: Susanne Tschernegg
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef REGISTRYMANAGER_H
#define REGISTRYMANAGER_H

#include <QString>

/**
    \class registrymanager.h
    
    \author Susanne Tschernegg
    
    This class is to access the registry of Windows. So you can get Information about something like commands to execute them.
    
*/
class RegistryManager
{
    
public:
    RegistryManager();
    
    ~RegistryManager();

    void startProcess(QString command);
    QStringList* getAllPrograms(QString format);
    QStringList* getAllPrograms(QStringList *formats);
    QStringList* getAllFormats(QString categorie = "all");
    QString getPath(QString exeStr);
};



#endif
