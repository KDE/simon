//
// C++ Implementation: SimonDirSelector
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "simondirselector.h"



SimonDirSelector::SimonDirSelector(QWidget *parent) : SimonFileSystemSelector("", i18n("Ordner auswählen"), 
						".", 0, "", QFileDialog::DirectoryOnly, parent)
{
}
