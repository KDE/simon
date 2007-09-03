//
// C++ Implementation: remoteplacepage
//
// Description:
//
//
// Author: Susanne Tschernegg
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "remoteplacepage.h"
#include "settings.h"
#include "command.h"
#include <QMessageBox>

/**
*   \brief constructor
*   @autor Susanne Tschernegg
*/
RemotePlacePage::RemotePlacePage(QWidget* parent): QWizardPage(parent)
{
    vboxLayout = new QVBoxLayout(this);
    
    label = new QLabel(this);
    label->setText(tr("Ort wird den Komandos hinzugefügt"));
    
    //wenn da eine fehlermeldung kommt, dass falls abbrechen gedrückt wird, der wizard zur configurepage zurück geht
    vboxLayout->addWidget(label);
}

/**
*   \brief destructor
*   @autor Susanne Tschernegg
*/
RemotePlacePage::~RemotePlacePage()
{}
