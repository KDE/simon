//
// C++ Implementation: localplacepage
//
// Description:
//
//
// Author: Susanne Tschernegg
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "localplacepage.h"
#include "settings.h"
#include "command.h"
#include <QMessageBox>

/**
*   \brief constructor
*   @author Susanne Tschernegg
*   @param QWidget *parent
*/
LocalPlacePage::LocalPlacePage(QWidget* parent): QWizardPage(parent)
{   
    vboxLayout = new QVBoxLayout(this);
    
    label = new QLabel(this);
    label->setText(tr("\nWählen Sie jetzt einen Ordner aus!\n\n"));
    
    lOrdner = new QLabel(this);
    lOrdner->setText(tr("Ordner"));
    hboxLayout = new QHBoxLayout();
    leOrdner = new QLineEdit(this);
    pbOrdner = new QPushButton(this);
    pbOrdner->setIcon(QIcon(":/images/icons/folder.svg"));
    
    vboxLayout->addWidget(label);
    vboxLayout->addWidget(lOrdner);
    hboxLayout->addWidget(leOrdner);
    hboxLayout->addWidget(pbOrdner);
    vboxLayout->addLayout(hboxLayout);
    
    connect(pbOrdner, SIGNAL(clicked()), this, SLOT(openFileDialog()));
}

/**
*   \brief destructor
*   @author Susanne Tschernegg
*/
LocalPlacePage::~LocalPlacePage()
{}

/**
*   \brief Creates a filedialog, to choose a place on the computer.
*   @author Susanne Tschernegg
*/
void LocalPlacePage::openFileDialog()
{
    dialog = new QFileDialog();
    dialog->setFileMode(QFileDialog::DirectoryOnly);
    QStringList fileList;
    QString file;    
    QDir dir;
    dir.setPath("C:/");
    dialog->setDirectory(dir);
    dialog->setReadOnly(true);
    
   int success = dialog->exec();
   if(success!=0)  // ! cancel
   {
        fileList = dialog->selectedFiles();
        if(!fileList.isEmpty())
        {
            file = fileList.at(0);
            leOrdner->setText(file);
        }            
   }
}

/**
*   \brief to geht the whole path of a place
*   @author Susanne Tschernegg
*   @return QString
*       returns the path to the place
*/
QString LocalPlacePage::getPlacePath()
{
    return leOrdner->text();
}
