//
// C++ Interface: bompdict
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef BOMPDICT_H
#define BOMPDICT_H

#include <dict.h>

class QString;

/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class BOMPDict : public Dict
{
	Q_OBJECT

signals:
	void loaded();
private:
	QString path;
public:
    BOMPDict(QString path="", QObject* parent=0);
    void load(QString path="");

    ~BOMPDict();

};

#endif
