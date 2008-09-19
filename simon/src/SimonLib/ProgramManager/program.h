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


class Program;
typedef QList<Program> ProgramList;
	
class Program{
private:
	QString name, exec, description, path, iconPath;
	QIcon icon;
	ProgramCategoryList categories;
public:
    Program(QString name, QString exec, QString description, QString path,
		ProgramCategoryList categories=ProgramCategoryList(), QIcon icon=QIcon(), QString iconPath="")
    {
    	this->name = name;
    	this->exec = exec;
	this->path = path;
    	this->description = description;
    	this->categories = categories;
    	this->icon = icon;
	this->iconPath = iconPath;
    }
    
    void setCategories(ProgramCategoryList categories) { this->categories = categories; }
    void setIcon(QIcon icon, QString src) { this->icon = icon; this->iconPath = src; }
    const QString getName() const { return name; }
    const QString getExec() const { return exec; }
    const QString getPath() const { return path; }
    const QString getIconPath() const { return iconPath; }
    const QIcon getIcon() const { return icon; }
    const QString getDescription() const { return description; }
    const ProgramCategoryList getCategories() const { return categories; }

    ~Program() { }

};

#endif
