//
// C++ Interface: program
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef PROGRAM_H
#define PROGRAM_H

#include "programcategory.h"
#include <QIcon>
#include <QString>
#include <QStringList>

/**
 * \class Program
 * \brief Describes one Program to display in the ImportProgram-Wizard
 * \author Peter Grasch
 * \version 0.1
 * \date 30.8.2007
*/
enum ProgramType {
	AudioProgram,
	OfficeProgram,
	DevelopmentProgram,
	GraphicProgram,
	InternetProgram,
	OtherProgram,
	GamesProgram,
	SystemProgram,
	VideoProgram
};

class Program;
typedef QList<Program> ProgramList;
	
class Program{
private:
	QString name, exec, description;
	QIcon icon;
	ProgramCategoryList categories;
public:
    Program(QString name, QString exec, QString description, 
		ProgramCategoryList categories=ProgramCategoryList(), QIcon &con=QIcon())
    {
    	this->name = name;
    	this->exec = exec;
    	this->description = description;
    	this->categories = categories;
    	this->icon = icon;
    }
    
    const QString getName() const { return name; }
    const QString getExec() const { return exec; }
    const QString getDescription() const { return description; }
    const ProgramCategoryList getCategories() const { return categories; }

    ~Program(){}

};

#endif
