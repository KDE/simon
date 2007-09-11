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
#include <QDebug>
#include "program.h"

#ifdef linux
#include "kdeprogrammanager.h"
#endif

#ifdef __WIN32
#include "windowsprogrammanager.h"
#endif


/**
*   \brief Constructor
            Creates the wizardpage, where you can select a special categorie (audio, büroprogramme, etc.).
            To this categorie you get a list of programs, which provides standardformats of this categorie.
*
*   @author Peter Grasch, Susanne Tschernegg
*/
SelectProgramPage::SelectProgramPage(QWidget* parent): QWizardPage(parent)
{
#ifdef linux
	this->programManager = new KDEProgramManager();
#endif
#ifdef __WIN32
	this->programManager = new WindowsProgramManager();
#endif


	this->categoryList = programManager->readCategories();
	
#ifdef linux
	((KDEProgramManager*) programManager)->loadPrograms();
#endif

        vboxLayout = new QVBoxLayout(this);
        vboxLayout->setObjectName("vboxLayout");

        lwCategories = new QListWidget(this);
        lwCategories->setObjectName("lwCategories");
        lwCategories->setIconSize(QSize(24,24));

        this->displayCategories(categoryList);

        lwPrograms = new QListWidget(this);
        lwPrograms->setMaximumHeight(140);

        registerField("executable*", lwPrograms);

        vboxLayout->addWidget(lwCategories);
        vboxLayout->addWidget(lwPrograms);

        connect(lwCategories, SIGNAL(itemSelectionChanged()), this, SLOT(searchForPrograms()));
}

/**
*   \brief Asks the ProgramManager for the programs of the selected category and inserts them
*
*   @author Peter Grasch
*/
void SelectProgramPage::searchForPrograms()
{
	ProgramCategory *cat = 
		programManager->getCategory(lwCategories->currentItem()->data(Qt::UserRole).toString());
	
	if (!cat) return; //we failed
	
	ProgramList *programs = programManager->getPrograms(*cat);
	if (!programs) return;

	insertPrograms(programs);
}




void SelectProgramPage::displayCategories(ProgramCategoryList categorieList)
{
    lwCategories->clear();
    QListWidgetItem* item;
    for(int i=0; i<categorieList.count(); i++)
    {
        item = new QListWidgetItem(lwCategories);
        item->setText(categorieList.at(i).getDescription());
        item->setToolTip(categorieList.at(i).getName());
        item->setData(Qt::UserRole, categorieList.at(i).getName());
        item->setIcon(categorieList.at(i).getIcon());
    }
    lwCategories->sortItems(Qt::AscendingOrder);
}

void SelectProgramPage::insertPrograms(ProgramList *programList)
{
    lwPrograms->clear();
    QListWidgetItem* lwItem;
    for (int i=0; i<programList->count(); i++)
    {
        lwItem = new QListWidgetItem(lwPrograms);
	lwItem->setIcon(programList->at(i).getIcon());
        lwItem->setText(programList->at(i).getName());
        lwItem->setData(Qt::UserRole, programList->at(i).getExec());
        lwItem->setData(33, programList->at(i).getPath());
    }
    lwPrograms->sortItems(Qt::AscendingOrder);
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



QString SelectProgramPage::getWorkingDirectory()
{
	return lwPrograms->currentItem()->data(33).toString();
}