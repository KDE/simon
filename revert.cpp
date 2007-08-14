//
// C++ Implementation: revert
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "revert.h"

/**
 * \brief Constructor - Inits the helptext and sets up the gui
 * \author Peter Grasch
 * @param parent the parent of the object
 */
Revert::Revert(QWidget *parent) : SystemWidget(tr("Zurücksetzen"), QIcon(":/images/icons/gtk-undo-ltr.svg"), tr("Stellen Sie einen vorherigen Zeitpunkt des Systems wieder her"), parent)
{
	ui.setupUi(this);
	hide();

	//set help
	help = tr("Dieser Menüpunkt erlaubt es Ihnen, die Einstellungen simpel und einfach zu einem früheren Stadium zurückzusetzen.\n\nUm zum Beispiel zum Stand vom 1.1. zurückzusetzen, wählen Sie einfach im Kalender den 1. Jänner, selektieren alle Änderungen und klicken auf \"Die ausgewählten Änderungen Rückgängig machen\".\n\nDie Suche dient als Einschränkung der Änderung.\n\nBitte beachten sie das die immer nur die selektierten Änderungen zurückgenommen werden!");
}


/**
 * \brief Inits the Control
 * \author Peter Grasch
 * @return success
 */
bool Revert::init()
{
	
	return true;
}


/**
 * \brief Applys the changes
 * \author Peter Grasch
 * @return success
 */
bool Revert::apply()
{
	
	return true;
}

/**
 * \brief Resets the Control
 * \author Peter Grasch
 */
bool Revert::reset()
{
	
	return true;
}

/**
 * \brief Destructor
 * \author Peter Grasch
 */
Revert::~Revert()
{
}


