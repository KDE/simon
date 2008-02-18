//
// C++ Interface: selectprogrampage
//
// Description:
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SELECTPROGRAMPAGE_H
#define SELECTPROGRAMPAGE_H

#include <QWizardPage>
#include "programcategory.h"
#include "program.h"

class ProgramManager;
class QListWidget;
class QHBoxLayout;
class QVBoxLayout;


/**
 * \class $CLASSNAME
 * \author Peter Grasch
 * \date 16.08.2007
 * \version 0.1
 * \brief This is a page of the ImportProgramWizard. The user must choose a category, to display the corresponding programs, where he also must choose one to continue.
 */
class SelectProgramPage : public QWizardPage
{
    Q_OBJECT

private:
	ProgramManager *programManager;
	ProgramCategoryList categoryList;

public:
        SelectProgramPage(QWidget* parent);
        ~SelectProgramPage();

        void insertCategories(ProgramCategoryList categorieList);
        void insertPrograms(ProgramList *programList);

	void initializePage();

        QString getExecPath();
        QString getName();
        QString getIcon();
	    QString getWorkingDirectory();

        QVBoxLayout *vboxLayout;
        QListWidget *lwCategories;
        QListWidget *lwPrograms;
        QHBoxLayout *hboxLayout;


    public slots:
        void searchForPrograms();
};

#endif
