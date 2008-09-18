//
// C++ Implementation: SimonFileSelector
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "simonfileselector.h"

SimonFileSelector::SimonFileSelector(QWidget *parent) : SimonFileSystemSelector("", i18n("Datei öffnen"), 
						".", 0, "", QFileDialog::ExistingFile, parent)
{
	
}
