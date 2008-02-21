//
// C++ Implementation: atobject
//
// Description:
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "atobject.h"
#include "ato.h"
#include "eventhandler.h"
#include <QMessageBox>

ATObject::ATObject ( ATObject* parent, QString nam, QString cName, QString desc ) :
			QWidget(parent),className(cName), name(nam)
{
	this->description=desc;
}

void ATObject::addMenu(ATOMenu* menu)
{
	this->menuList.append(menu);
}

void ATObject::trigger()
{
	EventHandler *eventHandler = EventHandler::getInstance();
	int xC = x()+(width()/2);
	int yC = y()+(height()/2);
	eventHandler->click(xC, yC);
}


ATObject::~ATObject()
{
}


