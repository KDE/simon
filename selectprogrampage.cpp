//
// C++ Implementation: selectprogrampage
//
// Description:
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "selectprogrampage.h"
#include <QMessageBox>
#include <QSize>
#include "windowsprogrammanager.h"
#include <QDebug>
#include "program.h"
#ifdef linux
#include "kdeprogrammanager.h"
#endif
#include "programcategory.h"

/**
*   \brief Constructor
            Creates the wizardpage, where you can select a special categorie (audio, office, etc.).
            To this categorie you get a list of programs, which provides standardformats of this categorie.
*
*   @autor Susanne Tschernegg, Peter Grasch
*   @author Susanne Tschernegg
*/
SelectProgramPage::SelectProgramPage(QWidget* parent): QWizardPage(parent)
{
#ifdef linux
	this->programManager = new KDEProgramManager();
#endif
#ifdef __WIN32
	this->programManager = new WindowsProgramManager();
#endif
	ProgramCategoryList catList = programManager->readCategories();
	for (int i=0; i < catList.count(); i++)
	{
		QString text = catList.at(i).getDescription()+":\n";
		QStringList cats = catList.at(i).getFormats();
		for (int j=0; j<cats.count(); j++)
			text += "\n"+cats.at(j);

		//QMessageBox::information(this, catList.at(i).getName(), text);
	}

        vboxLayout = new QVBoxLayout(this);
        vboxLayout->setObjectName("vboxLayout");

        lwCategories = new QListWidget(this);
        lwCategories->setObjectName("lwCategories");
        lwCategories->setIconSize(QSize(24,24));

        this->insertCategories(catList);

        lwPrograms = new QListWidget(this);
        lwPrograms->setMaximumHeight(140);

        registerField("executable*", lwPrograms);

        vboxLayout->addWidget(lwCategories);
        vboxLayout->addWidget(lwPrograms);

        connect(lwCategories, SIGNAL(itemSelectionChanged()), this, SLOT(searchForPrograms()));
        //connect(lwPrograms, sIGNAL(itemSelectionChanged()), this, SLOT(SelectProgramPage()));
}

/**
*   \brief destructor
*
*   @autor Susanne Tschernegg
*/
SelectProgramPage::~SelectProgramPage()
{}

/**
*   \brief gets the whole exe-name of the program (e.g. program.exe)
*
*   @autor Susanne Tschernegg
*/
QString SelectProgramPage::getExecPath()
{
    QString exeStr = lwPrograms->currentItem()->data(Qt::UserRole).toString();
    return exeStr;
}

/**
*   \brief gets the name of the program
*
*   @autor Susanne Tschernegg
*/
QString SelectProgramPage::getName()
{
    return lwPrograms->currentItem()->text();
}

void SelectProgramPage::insertCategories(ProgramCategoryList categorieList)
{
    lwCategories->clear();
    QListWidgetItem* item;
    for(int i=0; i<categorieList.count(); i++)
    {
        item = new QListWidgetItem(lwCategories);
        item->setText(categorieList.at(i).getName());
        item->setData(Qt::UserRole, categorieList.at(i).getDescription());
        item->setIcon(categorieList.at(i).getIcon());
    }
}

void SelectProgramPage::insertPrograms(ProgramList *programList)
{
    lwPrograms->clear();
    QListWidgetItem* lwItem;
    for (int i=0; i<programList->count(); i++)
    {
        lwItem = new QListWidgetItem(lwPrograms);
        lwItem->setText(programList->at(i).getName());
        lwItem->setData(Qt::UserRole, programList->at(i).getExec());
    }
}

/**
*   \brief searches for all programs, which contains the associated formats of a categorie
*
*   @author Susanne Tschernegg
*/
void SelectProgramPage::searchForPrograms()
{
    QString catName = lwCategories->currentItem()->text();
    
    ProgramCategoryList catList = programManager->readCategories();
    for(int i=0; i<catList.count(); i++)
    {
        if(catName == catList.at(i).getName())
        {
            insertPrograms(programManager->getPrograms(catList.at(i)));
            break;
        }            
    }
}
