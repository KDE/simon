//
// C++ Interface: inireader
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef INIREADER_H
#define INIREADER_H

#include <QHash>
class QFile;
class QVariant;


/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class IniReader{
private:
	QFile *file;
	QHash<QString, QString> document;
protected:
	QString path;
	QString getKey(QString key);
public:
    IniReader(QString path="");
    bool load(QString path="");

    ~IniReader();

};

#endif
