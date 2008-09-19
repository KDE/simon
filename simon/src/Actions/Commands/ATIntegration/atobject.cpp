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
#include "../EventSimulation/eventhandler.h"
#include <kmessagebox.h>

ATObject::ATObject ( ATObject* parent, const QString& nam, const QString& cName, const QString& desc ) :
			QWidget(parent),className(cName), name(nam)
{
	this->description=desc;
}

void ATObject::addMenu(ATOMenu* menu)
{
// 	KMessageBox::critical(0, "sodfij", menu->title);
// 	for (int i=0; i < menu->actions.count(); i++)
// 		KMessageBox::information(0, "sodfij", menu->actions[i]->title);
	this->menuList.append(menu);
}

void ATObject::trigger()
{
	int xC = x()+(width()/2);
	int yC = y()+(height()/2);
	EventHandler::getInstance()->click(xC, yC);
}


ATObject::~ATObject()
{
}


