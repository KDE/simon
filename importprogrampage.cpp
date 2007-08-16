//
// C++ Implementation: importprogrampage
//
// Description:
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "importprogrampage.h"

ImportProgramPage::ImportProgramPage(QWidget* parent): QWizardPage(parent)
{}


ImportProgramPage::~ImportProgramPage()
{}


bool ImportProgramPage::isComplete() const
{
        return QWizardPage::isComplete();
}

