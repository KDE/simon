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
#include "ui_selectprogramdlg.h"
#include "../../SimonLib/ProgramManager/programcategory.h"
#include "../../SimonLib/ProgramManager/program.h"

// class ProgramManager;


/**
 * \class SelectProgramPage
 * \author Peter Grasch
 * \date 16.08.2007
 * \version 0.1
 * \brief This is a page of the ImportProgramWizard. The user must choose a category, to display the corresponding programs, where he also must choose one to continue.
 */
class SelectProgramPage : public QWizardPage
{
    Q_OBJECT

private:
	Ui::SelectProgramDlg ui;
// 	ProgramManager *programManager;
// 	ProgramCategoryList categoryList;

public:
        SelectProgramPage(QWidget* parent);
        ~SelectProgramPage();

//         void insertCategories(const ProgramCategoryList& categorieList);
//         void insertPrograms(const ProgramList *programList);

	void initializePage();

	void findCategories(QString relPath);

	QString getExecPath();
	QString getName();
	QString getIcon();
	QString getWorkingDirectory();



    public slots:
        void searchForPrograms();
};

#endif
