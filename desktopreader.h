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

#include "inireader.h"
/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class Program;

class DesktopReader : public IniReader {
public:
    DesktopReader(QString path="");
    Program* readDesktopFile(QString path);

    ~DesktopReader();

};

#endif
