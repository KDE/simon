//
// C++ Interface: lexicondict
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LEXICONDICT_H
#define LEXICONDICT_H

#include <dict.h>

/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class LexiconDict : public Dict
{
	Q_OBJECT

signals:
	void loaded();

private:
	QString path;
public:
    LexiconDict(QString path, QObject* parent=0);
    void load(QString path="");

    ~LexiconDict();

};

#endif
