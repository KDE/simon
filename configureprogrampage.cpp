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

/**
*   \brief constructor
*
*   @autor Susanne Tschernegg
*/
ConfigureProgramPage::ConfigureProgramPage(QWidget* parent): QWizardPage(parent)
{
    vboxLayout = new QVBoxLayout(this);
    vboxLayout->setObjectName("vboxLayout");

    label = new QLabel(this);
    
    QHBoxLayout *hboxLayoutName = new QHBoxLayout();
    lName = new QLabel(this);
    lName->setText(tr("Name: "));
    leName = new QLineEdit(this);
    hboxLayoutName->addWidget(lName);
    hboxLayoutName->addWidget(leName);
    
    hboxLayoutPath = new QHBoxLayout();
    lPath = new QLabel(this);
    lPath->setText(tr("Pfad: "));
    lePath = new QLineEdit(this);
    hboxLayoutPath->addWidget(lPath);
    hboxLayoutPath->addWidget(lePath);
    
    hboxLayoutWorkingPath = new QHBoxLayout();
    lWorkingPath = new QLabel(this);
    lWorkingPath->setText(tr("Arbeitspfad: "));
    leWorkingPath = new QLineEdit(this);
    hboxLayoutWorkingPath->addWidget(lWorkingPath);
    hboxLayoutWorkingPath->addWidget(leWorkingPath);
    
    vboxLayout->addWidget(label);
    vboxLayout->addLayout(hboxLayoutName);
    vboxLayout->addLayout(hboxLayoutPath);
    vboxLayout->addLayout(hboxLayoutWorkingPath);
}

/**
*   \brief writes the chosen program with its values to the screen
*
*   @autor Susanne Tschernegg
*/
void ConfigureProgramPage::writeInformation()
{
    label->setText("Sie haben das Programm " + progName + " gewählt. \n\nHier können Sie nun weitere Einstellungen vornehmen.\n");
    leName->setText(progName);
    lePath->setText(execPath);
}

/**
*   \brief destructor
*
*   @autor Susanne Tschernegg
*/
ConfigureProgramPage::~ConfigureProgramPage()
{}


