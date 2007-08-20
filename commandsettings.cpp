//
// C++ Implementation: commandsettings
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "commandsettings.h"

CommandSettings::CommandSettings(QWidget* parent): SystemWidget(tr("Kommandos"), QIcon(":/images/icons/emblem-system.svg"), tr("Hier können Sie Programme und Orte importieren und vorhandene Kommandos bearbeiten"), parent)
{
}


CommandSettings::~CommandSettings()
{
}


