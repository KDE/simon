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
 \class BOMPDict
 \author Peter Grasch
 \version 0.1
 \date 10.08.2007
 \brief Represetnts a dictionary from the university Bonn (Germany)
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
