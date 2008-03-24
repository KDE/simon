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
* @author Susanne Tschernegg
*/
RegistryManager::RegistryManager()
{}
    
/**
* \brief Destructor
* @author Susanne Tschernegg
*/
RegistryManager::~RegistryManager()
{}

/**
* \brief Watches first out, if the comment is the final comment, including the path to the exe-datafile.
*    If it exists, the process will be started.
*    Otherwise it reads the path of the exe-datafile out of the registry and starts it - if the command exists.
*
* @author Susanne Tschernegg
* @param QString command
*       holds the name of the command
* @param QString workingDirectory
*       holds the workingdirectory
*       FIXME: WTF? -- peter
*/
void RegistryManager::startProcess(QString command, QString workingDirectory)
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
        QProcess *process = new QProcess();
        process->setWorkingDirectory(workingDirectory);    //sets the workingdirectory
        process->startDetached(str);
    }   
}

/**
* \brief This method returns all programs, which provides a specific format.
*
* @author Susanne Tschernegg
* @param QString format
*   holds the format of the programs, which should be returned
* @return QStringList*
*   returns a list of all programms with the specified format
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
    format.prepend(".");
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

/**
*   \brief get all formats of a special categorie
*
*   @author Susanne Tschernegg
*   @param QString categorie
*       holds the name of a given category
*   @return QStringList*
*       returns a list with all formats of this category
*/
QStringList* RegistryManager::getAllFormats(QString categorie)
{
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
        
        QStringList *formatListCateg = new QStringList();
        for(int i=0; i<formatList->count(); i++)
        {
            success = RegOpenKeyEx(HKEY_CLASSES_ROOT, (WCHAR*)formatList->at(i).utf16(), 0, KEY_QUERY_VALUE, &hKey2)==ERROR_SUCCESS;
            success = RegQueryValueEx(hKey2, L"PerceivedType", NULL, NULL, temp, &size)==ERROR_SUCCESS;
            QString tempStr = QString::fromUtf16((const ushort*)temp);
            if(success && (tempStr == categorie))
            {
                formatListCateg->append(formatList->at(i));
            }
        }
        return formatListCateg;
    }
    
    RegCloseKey(hKey);
    
    return formatList;
}

/**
*   \brief returns the path of a program
*
*   @author Susanne Tschernegg
*   @param QString exeStr
*       holds the .exe of a program
*   @return QString
*       returns the path to the .exe of the program
*/
QString RegistryManager::getPath(QString exeStr)
{
    unsigned char temp[1024] = {""};
        unsigned long size = 1024;
        HKEY hKey;
        bool noerror;
        
        QString strAusfuehren = "\\Applications\\" + exeStr + "\\shell\\open\\command";
        noerror = RegOpenKeyEx(HKEY_CLASSES_ROOT, (WCHAR*)strAusfuehren.utf16(), 0, KEY_QUERY_VALUE, &hKey)==ERROR_SUCCESS;
        if(!noerror)
        {
            strAusfuehren = "\\Applications\\" + exeStr + "\\shell\\edit\\command";
            noerror = RegOpenKeyEx(HKEY_CLASSES_ROOT, (WCHAR*)strAusfuehren.utf16(), 0, KEY_QUERY_VALUE, &hKey)==ERROR_SUCCESS;
            if(!noerror)
            {
                return "";
            }
        }
        
        bool success = RegQueryValueEx(hKey, L"", NULL, NULL, temp, &size)==ERROR_SUCCESS;
        if(!success)
        {
            RegCloseKey(hKey);
            return "";
        }
        RegCloseKey(hKey);
        QString str = QString::fromUtf16((const ushort*)temp);
        
        int indexStart = str.indexOf("\"");
        str = str.mid(indexStart, str.indexOf("\"", indexStart+1)-(indexStart-1));
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
        return str;
}

/**
*   \brief returns a list of programs, and each program in this list, exists only once.
*   @author Susanne Tschernegg
*   @param QStringList formats
*       holds different formats like ".jpg",".xls", etc.
*   @return QStringList*
*       returns a list of all programms, which provides at least one format of the list
*/
QStringList* RegistryManager::getAllPrograms(const QStringList formats)
{
   QStringList* progList = new QStringList();
    bool exists = false;
   for(int i=0; i<formats.count(); i++)
   {
        QStringList *newProgs = getAllPrograms(formats.at(i));
        for(int z=0; z<newProgs->count(); z++)
        {
            if(!newProgs->at(z).contains(".exe", Qt::CaseInsensitive))
                continue;
            for(int a=0; a<progList->count(); a++)
            {
                if(QString::compare(newProgs->at(z), progList->at(a), Qt::CaseInsensitive)==0)
                {
                    exists = true;
                    continue;
                }
            }
            if(!exists)
            {
                progList->append(newProgs->at(z));
            }
            else exists = false;
        }
    }
    return progList;
}
