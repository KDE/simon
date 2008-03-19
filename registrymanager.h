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
 * \class RegistryManager
 * \author Susanne Tschernegg
 * \date 20.08.2007
 * \version 0.1
 * \brief This class is to access the registry of Windows. So you can get Information about something like commands to execute them.
 */
class RegistryManager
{
    
public:
    RegistryManager();
    
    ~RegistryManager();

    void startProcess(QString command, QString workingDirectory="");
    QStringList* getAllPrograms(QString format);
    QStringList* getAllPrograms(const QStringList formats);
    QStringList* getAllFormats(QString categorie = "all");
    QString getPath(QString exeStr);
};



#endif
