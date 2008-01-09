//
// C++ Implementation: importplacepage
//
// Description:
//
//
// Author: Susanne Tschernegg
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "importplacepage.h"
#include "settings.h"
#include "command.h"
#include <QMessageBox>

/**
*   \brief constructor
*   @author Susanne Tschernegg
*/
ImportPlacePage::ImportPlacePage(QWidget* parent): QWizardPage(parent)
{
    QVBoxLayout *vboxLayout = new QVBoxLayout(this);
    
    label = new QLabel(this);
    label->setText(tr("Ort wird den Komandos hinzugefügt\n\n"));
    
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
*   @author Susanne Tschernegg
*/
ImportPlacePage::~ImportPlacePage()
{}

/**
*   \brief 
*   @author Susanne Tschernegg
*/
bool ImportPlacePage::isComplete() const
{
    return QWizardPage::isComplete();
}

/**
*   \brief emits a the signal "commandCreated(newCommand)", when this page is the current page
*
*   @author Susanne Tschernegg
*/
void ImportPlacePage::createCommand(QString name, QString value)
{
    //QMessageBox::information(0, "importPlacePage","createCommand");
    Command *newCommand = new Command(name, CommandType(1), value);
    emit commandCreated(newCommand);
}
