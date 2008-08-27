//
// C++ Interface: programcategory
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef PROGRAMCATEGORY_H
#define PROGRAMCATEGORY_H

#include <QIcon>
#include <QList>
/**
	@author Peter Grasch <bedahr@gmx.net>
*/

class ProgramCategory;
typedef QList<ProgramCategory> ProgramCategoryList;

class ProgramCategory{
private:
	QString name, description;
	QIcon icon;
	QStringList formats;

public:
	ProgramCategory(QString name, QString description, QIcon icon=QIcon(), QStringList 
			formats=QStringList())
	{
		this->name = name;
		this->description = description;
		this->icon = icon;
		this->formats = formats;
	}
	
	QString getName() const { return name; }
	QString getDescription() const { return description; }
	QIcon getIcon() const { return icon; }
	QStringList getFormats() const { return formats; }

	bool operator==(ProgramCategory c)
	{
		return (name == c.getName());
	}

	~ProgramCategory() {}

};

#endif
