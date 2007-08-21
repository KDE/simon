//3
// C++ Implementation: registrymanager
//
// Description:
//
//
// Author: Susanne Tschernegg
//
//

#include "registrymanager.h"
#include <windows.h>
#include <winreg.h>
#include <winnt.h>
#include <QFile>
#include <QProcess>
#include <QMessageBox>
#include <QStringList>

/**
* \brief Constructor
* @autor Susanne Tschernegg
*/
RegistryManager::RegistryManager()
{}
    
/**
* \brief Destructor
* @autor Susanne Tschernegg
*/
RegistryManager::~RegistryManager()
{}

/**
* \brief Watches first out, if the comment is the final comment, including the path to the exe-datafile.
    If it exists, the process will be started.
    Otherwise it reads the path of the exe-datafile out of the registry and starts it - if the command exists.
*
* @autor Susanne Tschernegg
*/
void RegistryManager::startProcess(QString command)
{
      if(QFile::exists(command))
        QProcess::startDetached(command);
    else
    {
        unsigned char temp[1024] = {""};
        unsigned long size = 1024;
        HKEY hKey;
        bool noerror;
        
        QString strAusfuehren = "\\Applications\\" + command + "\\shell\\open\\command";
        noerror = RegOpenKeyEx(HKEY_CLASSES_ROOT, (WCHAR*)strAusfuehren.utf16(), 0, KEY_QUERY_VALUE, &hKey)==ERROR_SUCCESS;
        if(!noerror)
        {
            strAusfuehren = "\\Applications\\" + command + "\\shell\\edit\\command";
            noerror = RegOpenKeyEx(HKEY_CLASSES_ROOT, (WCHAR*)strAusfuehren.utf16(), 0, KEY_QUERY_VALUE, &hKey)==ERROR_SUCCESS;
            if(!noerror)
            {
                return;
            }
        }
        
        bool success = RegQueryValueEx(hKey, L"", NULL, NULL, temp, &size)==ERROR_SUCCESS;
        if(!success)
        {
            RegCloseKey(hKey);
            return;
        }
        RegCloseKey(hKey);
        QString str = QString::fromUtf16((const ushort*)temp);
        
        int indexStart = str.indexOf("\"");
        str = str.mid(indexStart, str.indexOf("\"", indexStart+1)-(indexStart-1));
        //QMessageBox::information(0, " j " , getenv("systemroot"));
        int startIndex = str.indexOf("%");
        int endIndex = str.indexOf("%",startIndex+1);
        while(startIndex>=0 && endIndex>0)
        {
            QString searchStr = str.mid(startIndex+1, endIndex-startIndex-1);   //z.b. systemroot
            QString replaceStr = getenv((const char*)searchStr.toUtf8());   //z.b. c:\windows
            str.replace(startIndex, endIndex-startIndex+1, replaceStr);
            startIndex = str.indexOf("%");
            endIndex = str.indexOf("%",startIndex+1);
        } 
        QProcess::startDetached(str);
    }   
}

/**
* \brief This method returns all programs, which provides a specific format.
*
* @autor Susanne Tschernegg
*/
QStringList* RegistryManager::getAllPrograms(QString format)
{
    QStringList *programs = new QStringList();
    unsigned long cSubKeys = 0;
    unsigned long u = 0;
    unsigned long cbName;
    unsigned char achKey[512] = {""};
    HKEY hKey;
    bool success;
    
    QString strFormatPath = "\\" + format + "\\OpenWithList";
    success = RegOpenKeyEx(HKEY_CLASSES_ROOT, (WCHAR*)strFormatPath.utf16(), 0, KEY_ALL_ACCESS, &hKey)==ERROR_SUCCESS;
    if(!success)
    {
            return programs;
    }
    
    
    RegQueryInfoKey(hKey, NULL, NULL, NULL, &cSubKeys, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

    if (cSubKeys)
    {
        for (u=0; u<cSubKeys; u++) 
        { 
            cbName = 255;
            RegEnumKeyEx(hKey, u, (WCHAR*)achKey, &cbName, NULL, NULL, NULL, NULL);

            programs->append(QString::fromUtf16((const ushort*)achKey));
        }
    }

    
    RegCloseKey(hKey);
    
    return programs;
}

QStringList* RegistryManager::getAllFormats(QString categorie)
{
    //QMessageBox::information(this, "getAllFormats - Registrymanager", "debug 1");
    QStringList *formatList = new QStringList();
    unsigned long cSubKeys = 0;
    unsigned long u = 0;
    unsigned long cbName;
    unsigned char achKey[512] = {""};
    HKEY hKey;
    bool success;
    
    success = RegOpenKeyEx(HKEY_CLASSES_ROOT, NULL, 0, KEY_ALL_ACCESS, &hKey)==ERROR_SUCCESS;
    if(!success)
    {
            return formatList;
    }
    
    
    RegQueryInfoKey(hKey, NULL, NULL, NULL, &cSubKeys, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

    if (cSubKeys)
    {
        for (u=0; u<cSubKeys; u++) 
        { 
            cbName = 255;
            RegEnumKeyEx(hKey, u, (WCHAR*)achKey, &cbName, NULL, NULL, NULL, NULL);

            if((QString::fromUtf16((const ushort*)achKey)).left(1)==".")
                formatList->append(QString::fromUtf16((const ushort*)achKey));
        }
    }
    
    
    if(categorie != "all")
    {
        unsigned char temp[1024] = {""};
        unsigned long size = 1024;
        HKEY hKey2;
        
        QStringList *formatList_copy = formatList;
        formatList->clear();
        for(int i=0; i<formatList_copy->count(); i++)
        {
            success = RegOpenKeyEx(HKEY_CLASSES_ROOT, (WCHAR*)formatList_copy->at(i).utf16(), 0, KEY_QUERY_VALUE, &hKey2)==ERROR_SUCCESS;
            success = RegQueryValueEx(hKey2, L"PerceivedType", NULL, NULL, temp, &size)==ERROR_SUCCESS;
            if(success && ((const char*)temp == categorie))
            {
                formatList->append(formatList_copy->at(i));
            }
        }
    }
    RegCloseKey(hKey);
    
    return formatList;
}
