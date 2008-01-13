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
*   @author Susanne Tschernegg, Peter Grasch
*   @param QWidget *parent
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


#ifdef linux
	((KDEProgramManager*) this->programManager)->loadPrograms();
#endif
/*	for (int i=0; i < catList.count(); i++)
	{
		QString text = catList.at(i).getDescription()+":\n";
		QStringList cats = catList.at(i).getFormats();
		for (int j=0; j<cats.count(); j++)
			text += "\n"+cats.at(j);

		//QMessageBox::information(this, catList.at(i).getName(), text);
	}*/

        vboxLayout = new QVBoxLayout(this);
        vboxLayout->setObjectName("vboxLayout");

        lwCategories = new QListWidget(this);
        lwCategories->setObjectName("lwCategories");
        lwCategories->setIconSize(QSize(24,24));

        this->insertCategories(programManager->readCategories());

        lwPrograms = new QListWidget(this);
        lwPrograms->setMaximumHeight(140);

        registerField("executable*", lwPrograms);

        vboxLayout->addWidget(lwCategories);
        vboxLayout->addWidget(lwPrograms);

        connect(lwCategories, SIGNAL(itemSelectionChanged()), this, SLOT(searchForPrograms()));
        //connect(lwPrograms, SIGNAL(itemSelectionChanged()), this, SLOT(searchForPrograms()));
}

/**
*   \brief destructor
*
*   @author Susanne Tschernegg
*/
SelectProgramPage::~SelectProgramPage()
{}

/**
*   \brief gets the whole exe-name of the program (e.g. program.exe)
*
*   @author Susanne Tschernegg
*   @return QStirng
*       returns the name of the .exe file
*/
QString SelectProgramPage::getExecPath()
{
    QString exeStr = lwPrograms->currentItem()->data(Qt::UserRole).toString();
    return exeStr;
}

/**
*   \brief gets the iconsrc of the program
*
*   @author Peter Grasch
*/
QString SelectProgramPage::getIcon()
{
    return lwPrograms->currentItem()->data(Qt::UserRole+1).toString();
}


/**
*   \brief gets the name of the program
*
*   @author Susanne Tschernegg
*   @return QString
*       returns the name of the program
*/
QString SelectProgramPage::getName()
{
    return lwPrograms->currentItem()->text();
}

/**
*   \brief inserts the different categories, which were listed in the categorieList.
*
*   @author Susanne Tschernegg
*   @param ProgramCategoryList categorieList
*       holds the categories, which where read out of a xml-file
*/
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

/**
*   \brief inserts all given programms in a list
*
*   @author Susanne Tschernegg+
*   @param ProgramList *programList
*       is a list, which holds all programms of a specified category
*/
void SelectProgramPage::insertPrograms(ProgramList *programList)
{
    lwPrograms->clear();
    QListWidgetItem* lwItem;
    for (int i=0; i<programList->count(); i++)
    {
        lwItem = new QListWidgetItem(lwPrograms);
        lwItem->setText(programList->at(i).getName());
	lwItem->setIcon(programList->at(i).getIcon());
        lwItem->setData(Qt::UserRole, programList->at(i).getExec());
        lwItem->setData(Qt::UserRole+1, programList->at(i).getIconPath());
        lwItem->setData(Qt::UserRole+2, programList->at(i).getPath());
    }
}

/**
*   \brief searches for all programs, which contains the associated formats of a category
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
            ProgramList *pl = programManager->getPrograms(catList.at(i));
            insertPrograms(pl);
            break;
        }
    }
}

/**
*   \brief returns the workingdirectory, which the user set
*
*   @author Susanne Tschernegg
*   @return QString
*       returns the workingdirectory
*/
QString SelectProgramPage::getWorkingDirectory()
{
	return lwPrograms->currentItem()->data(Qt::UserRole+2).toString();
}

