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

/**
*   \brief constructor
*   @author Susanne Tschernegg
*   @param QWidget* parent
*/
ImportProgramPage::ImportProgramPage(QWidget* parent): QWizardPage(parent)
{
    vboxLayout = new QVBoxLayout(this);
    
    label = new QLabel(this);
    label->setText(tr("Programm wird den Komandos hinzugefügt"));
    
    progressbar = new QProgressBar(this);
    
    vboxLayout->addWidget(label);
    vboxLayout->addWidget(progressbar);
    progressbar->setMaximum(100);
    progressbar->setValue(0);
    progressbar->setValue(100);
}

/**
*   \brief destructor
*   @author Susanne Tschernegg
*/
ImportProgramPage::~ImportProgramPage()
{}

/**
*   \brief returns, wheter the importProgramPage is completed or not.
*   @author Susanne Tschernegg
*   @return bool
*       returns wheter the ImportProgramPage was finished or not
*/
bool ImportProgramPage::isComplete() const
{
        return QWizardPage::isComplete();
}

/**
*   \brief emits a the signal "commandCreated(newCommand)", when this page is the current page
*
*   @author Susanne Tschernegg
*   @param QString icon
*       holds the resource of the icon for the command
*   @param QString name
*       holds the name of the command
*   @param QString value
*       holds the value of the command
*   @param QString workingDir
*       holds the working directory of the command
*/
void ImportProgramPage::createCommand(QString icon, QString name, QString value, QString workingDir)
{
    Command *newCommand = new Command(name, CommandType(0), value, icon, workingDir);
    emit commandCreated(newCommand);
}

