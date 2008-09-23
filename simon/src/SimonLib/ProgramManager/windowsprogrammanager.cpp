//
// C++ Implementation: windowsprogrammanager
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "windowsprogrammanager.h"
#include "program.h"

#include "windows.h"

#include "../WindowsLib/registrymanager.h"


/**
*   \brief constructor. initalizes the registrymanager
*   @author Susanne Tschernegg
*/
WindowsProgramManager::WindowsProgramManager(): ProgramManager()
{
    regMan = new RegistryManager();
}

/**
*   \brief destructor
*   @author Susanne Tschernegg
*/
WindowsProgramManager::~WindowsProgramManager()
{
    delete regMan;
}

/**
*   \brief lists all programms of a specified categorie
*   @author Susanne Tschernegg
*   @param ProgramCategory categorie
*       holds a program-category like "office", "games", etc.
*   @return ProgramList*
*       returns a list of programs, which are added to a special categorie
*/
ProgramList* WindowsProgramManager::getPrograms(ProgramCategory categorie)
{
    //first the registry manager searches for the endings and looks, if there is a "openwithlist" directory and looks for the perceivedtype.
  
    QStringList *progNameList = new QStringList();

    progNameList = regMan->getAllPrograms(categorie.getFormats());
/*
Program(QString name, QString exec, QString description, 
		ProgramCategoryList categories=ProgramCategoryList(), QIcon icon=QIcon())    
*/
    
    QString name;
    QString exec;
    QString description = "";
    QString path = "";  //TODO set a path for the program
    QIcon icon("unknown");
    
    //to geht the name of the program ... cuts only the .exe out of the string
    int startIndex;
    int length;
    ProgramList *progList = new ProgramList();
    ProgramCategoryList progCatList;
    progCatList.append(categorie);
    for (int i=0; i<progNameList->count(); i++)
    {
        startIndex = progNameList->at(i).lastIndexOf(".");
        length = progNameList->at(i).length();
        exec = progNameList->at(i);
        name = exec;
        if (startIndex>=0) {
                name.remove(startIndex, length-startIndex);
        }
        Program prog(name, exec, description, path, progCatList, icon);
        progList->append(prog);
    }
    delete progNameList;
   return progList;
}
