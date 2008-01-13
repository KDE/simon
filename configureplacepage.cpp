//
// C++ Implementation: configureplacepage
//
// Description:
//
//
// Author: Susanne Tschernegg
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "configureplacepage.h"
#include <QMessageBox>

/**
*   \brief constructor
*
*   @author Susanne Tschernegg
*   @param QWidget *parent
*/
ConfigurePlacePage::ConfigurePlacePage(QWidget* parent): QWizardPage(parent)
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
    
    vboxLayout->addWidget(label);
    vboxLayout->addLayout(hboxLayoutName);
    vboxLayout->addLayout(hboxLayoutPath);
    
    placeName = "";
    placeValue = "";
}

/**
*   \brief writes the chosen program with its values to the screen
*
*   @author Susanne Tschernegg
*/
void ConfigurePlacePage::writeInformation()
{
    label->setText(tr("Sie haben einen Ort mit unten angeführten Werten gewählt. \n\nHier können Sie falls nötig die Werte ändern.\n"));
    leName->setText(placeName);
    lePath->setText(placeValue);
}

/**
*   \brief destructor
*
*   @author Susanne Tschernegg
*/
ConfigurePlacePage::~ConfigurePlacePage()
{}

/**
*   \brief to set the place value
*
*   @author Susanne Tschernegg
*/
void ConfigurePlacePage::setPlaceValue()
{
    placeValue = lePath->text();
}

/**
*   \brief to set the place name
*
*   @author Susanne Tschernegg
*/
void ConfigurePlacePage::setPlaceName()
{
    placeName = leName->text();
}



