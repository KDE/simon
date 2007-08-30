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
#include "settings.h"
#include "command.h"
#include <QMessageBox>

/**
*   \brief constructor
*   @autor Susanne Tschernegg
*/
ImportProgramPage::ImportProgramPage(QWidget* parent): QWizardPage(parent)
{
    vboxLayout = new QVBoxLayout(this);
    
    label = new QLabel(this);
    label->setText("Programm wird den Komandos hinzugefügt");
    
    progressbar = new QProgressBar(this);
    
    //wenn da eine fehlermeldung kommt, dass falls abbrechen gedrückt wird, der wizard zur configurepage zurück geht
    vboxLayout->addWidget(label);
    vboxLayout->addWidget(progressbar);
    progressbar->setMaximum(100);
    progressbar->setValue(0);
    progressbar->setValue(100);
    //createCommand("test","testkasdjf");
   // QMessageBox::information(0, "importProgrampage","constructor");
}

/**
*   \brief destructor
*   @autor Susanne Tschernegg
*/
ImportProgramPage::~ImportProgramPage()
{}

/**
*   \brief 
*   @autor Susanne Tschernegg
*/
bool ImportProgramPage::isComplete() const
{
        return QWizardPage::isComplete();
}

/**
*   \brief emits a the signal "commandCreated(newCommand)", when this page is the current page
*
*   @autor Susanne Tschernegg
*/
void ImportProgramPage::createCommand(QString name, QString value)
{
    //QMessageBox::information(0, "importProgrampage","createCommand");
    Command *newCommand = new Command(name, CommandType(0), value);
    emit commandCreated(newCommand);
}

