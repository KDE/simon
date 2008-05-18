//
// C++ Interface: desktopreader
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef DESKTOPREADER_H
#define DESKTOPREADER_H

#include "../INI/inireader.h"
#include <QStringList>
/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class Program;

class DesktopReader : public IniReader {
private:
	QStringList strCategories;
	QString iconname;
public:
    DesktopReader(QString path="");
    Program* readDesktopFile(QString path);
    QString getIconname() { return iconname; }
    QStringList getStrCategories() { return strCategories; }

};

#endif
