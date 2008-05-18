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
#include "../../SimonLib/Settings/settings.h"
#include <QMessageBox>

/**
*   \brief constructor
*   @author Susanne Tschernegg
*   @param QWidget* parent
*/
ImportPlacePage::ImportPlacePage(QWidget* parent): QWizardPage(parent)
{
    QVBoxLayout *vboxLayout = new QVBoxLayout(this);
    
    label = new QLabel(this);
    label->setText(tr("Ort wird den Komandos hinzugefügt\n\n"));
    
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
ImportPlacePage::~ImportPlacePage()
{
    label->deleteLater();
    progressbar->deleteLater();
}

/**
*   \brief Returns, wheter the importPlacePage is complete or not.
*   @author Susanne Tschernegg
*   @return bool
*       if the importPlacePage is complete, it will return true, otherwise false
*/
bool ImportPlacePage::isComplete() const
{
    return QWizardPage::isComplete();
}

/**
*   \brief emits a the signal "commandCreated(newCommand)", when this page is the current page; it creates a net command
*
*   @author Susanne Tschernegg
*   @param QString name
*       holds the name of the new command
*   @param QString value
*       holds the value of the new command
*/
void ImportPlacePage::createCommand(QString name, QString value)
{
    Command *newCommand = new Command(name, CommandType(1), value);
    emit commandCreated(newCommand);
}
