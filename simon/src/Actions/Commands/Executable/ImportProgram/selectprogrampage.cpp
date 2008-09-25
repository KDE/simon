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
#include <KMessageBox>

#include <KListWidget>
#include <QListWidgetItem>
#include <QWidget>
#include <KService>
#include <KServiceGroup>
#include <QDebug>

// #ifdef Q_OS_UNIX
// #include "../../SimonLib/ProgramManager/kdeprogrammanager.h"
// #endif
// #ifdef Q_OS_WIN
// #include "../../SimonLib/ProgramManager/windowsprogrammanager.h"
// #endif

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
	ui.setupUi(this);
// #ifdef Q_OS_UNIX
// 	this->programManager = new KDEProgramManager();
// #endif
// #ifdef Q_OS_WIN
// 	this->programManager = new WindowsProgramManager();
// #endif

        registerField("executable*", ui.lwPrograms);

        connect(ui.lwCategories, SIGNAL(itemSelectionChanged()), this, SLOT(searchForPrograms()));
	
	ui.lwCategories->setIconSize(QSize(24,24));
	ui.lwPrograms->setIconSize(QSize(24,24));
}

/**
 * \brief Initializes the page (under linux this also loads the list of programs)
 * \author Peter Grasch
 */
void SelectProgramPage::initializePage()
{
	findCategories("");
}

void SelectProgramPage::findCategories(QString relPath)
{
	KServiceGroup::Ptr root = KServiceGroup::group(relPath);
	KServiceGroup::List list = root->entries();

	for (KServiceGroup::List::ConstIterator it = list.begin(); it != list.end(); ++it)
	{
		const KSycocaEntry::Ptr p = (*it);

		if (p->isType(KST_KServiceGroup))
		{
			const KServiceGroup::Ptr serviceGroup = KServiceGroup::Ptr::staticCast(p);

			if (serviceGroup->noDisplay() || serviceGroup->childCount() == 0)
				continue;

			QListWidgetItem *item = new QListWidgetItem(KIcon(serviceGroup->icon()), serviceGroup->caption());
			item->setData(Qt::UserRole, serviceGroup->relPath());
			ui.lwCategories->addItem(item);
			findCategories(serviceGroup->relPath());
		}
	}
}

/**
*   \brief destructor
*
*   @author Susanne Tschernegg
*/
SelectProgramPage::~SelectProgramPage()
{
//     delete programManager;
}

/**
*   \brief gets the whole exe-name of the program (e.g. program.exe)
*
*   @author Peter Grasch
*   @return QString
*       returns the name of the .exe file
*/
QString SelectProgramPage::getExecPath()
{
	Q_ASSERT(ui.lwPrograms->currentItem());
	return ui.lwPrograms->currentItem()->data(Qt::UserRole+1).toString();
}

/**
*   \brief gets the iconsrc of the program
*
*   @author Peter Grasch
*/
QString SelectProgramPage::getIcon()
{
	Q_ASSERT(ui.lwPrograms->currentItem());
	return ui.lwPrograms->currentItem()->data(Qt::UserRole+2).toString();
}


/**
*   \brief gets the name of the program
*
*   @author Peter Grasch
*   @return QString
*       returns the name of the program
*/
QString SelectProgramPage::getName()
{
	Q_ASSERT(ui.lwPrograms->currentItem());
	return ui.lwPrograms->currentItem()->data(Qt::UserRole).toString();
}

/**
*   \brief inserts all given programms in a list
*
*   @author Susanne Tschernegg
*   @param ProgramList *programList
*       is a list, which holds all programms of a specified category
*/
// void SelectProgramPage::insertPrograms(const ProgramList *programList)
// {
//     ui.lwPrograms->clear();
//     QListWidgetItem* lwItem;
//     for (int i=0; i<programList->count(); i++)
//     {
//         lwItem = new QListWidgetItem(ui.lwPrograms);
//         lwItem->setText(programList->at(i).getName());
// 	lwItem->setIcon(programList->at(i).getIcon());
//         lwItem->setData(Qt::UserRole, programList->at(i).getExec());
//         lwItem->setData(Qt::UserRole+1, programList->at(i).getIconPath());
//         lwItem->setData(Qt::UserRole+2, programList->at(i).getPath());
//     }
// }

/**
*   \brief searches for all programs, which contains the associated formats of a category
*
*   @author Susanne Tschernegg
*/
void SelectProgramPage::searchForPrograms()
{
	ui.lwPrograms->clear();

	QListWidgetItem *curCategory = ui.lwCategories->currentItem();
	if (!curCategory) return;

	KServiceGroup::Ptr root = KServiceGroup::group(curCategory->data(Qt::UserRole).toString());
	KServiceGroup::List list = root->entries();

	for (KServiceGroup::List::ConstIterator it = list.begin(); it != list.end(); ++it)
	{
		const KSycocaEntry::Ptr p = (*it);
		if (p->isType(KST_KService))
		{
			const KService::Ptr service = KService::Ptr::staticCast(p);

			if (service->noDisplay())
				continue;
			
			QString displayName;
			if (!service->genericName().isEmpty())
				displayName = i18n("%1 (%2)", service->name(), service->genericName());
			else displayName = service->name();
			QListWidgetItem *item = new QListWidgetItem(KIcon(service->icon()), displayName);
			
			item->setData(Qt::UserRole, service->name());
			item->setData(Qt::UserRole+1, service->exec());
			item->setData(Qt::UserRole+2, service->icon());
			item->setData(Qt::UserRole+3, service->path());
			ui.lwPrograms->addItem(item);
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
	return ui.lwPrograms->currentItem()->data(Qt::UserRole+3).toString();
}

