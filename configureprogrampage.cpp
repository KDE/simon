//
// C++ Implementation: configureprogrampage
//
// Description:
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "configureprogrampage.h"

ConfigureProgramPage::ConfigureProgramPage(QWidget* parent): QWizardPage(parent)
{
    vboxLayout = new QVBoxLayout(this);
    vboxLayout->setObjectName("vboxLayout");

    label = new QLabel(this);
    
    hboxLayoutPath = new QHBoxLayout(this);
    lPath = new QLabel(this);
    lPath->setText("Pfad: ");
    lePath = new QLineEdit(this);
    hboxLayoutPath->addWidget(lPath);
    hboxLayoutPath->addWidget(lePath);
    
    hboxLayoutWorkingPath = new QHBoxLayout(this);
    lWorkingPath = new QLabel(this);
    lWorkingPath->setText("Arbeitspfad: ");
    leWorkingPath = new QLineEdit(this);
    hboxLayoutWorkingPath->addWidget(lWorkingPath);
    hboxLayoutWorkingPath->addWidget(leWorkingPath);
    
    vboxLayout->addWidget(label);
    vboxLayout->addLayout(hboxLayoutPath);
    vboxLayout->addLayout(hboxLayoutWorkingPath);
}

void ConfigureProgramPage::writeInformation()
{
    label->setText("Sie haben das Programm " + progName + " gewählt. \n\nHier können Sie nun weitere Einstellungen vornehmen.\n");
    lePath->setText(execPath);
}


ConfigureProgramPage::~ConfigureProgramPage()
{}


