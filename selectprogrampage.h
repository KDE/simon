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
#include <QVariant>
#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QHBoxLayout>
#include <QListWidget>
#include <QVBoxLayout>
#include <QWidget>

#ifdef __WIN32
#include "registrymanager.h"
#endif

class ProgramManager;
/**
	@author Peter Grasch <bedahr@gmx.net>
*/
/**
 * \class $CLASSNAME
 * \author Peter Grasch
 * \date
 * \version 0.1
 * \brief
 */
class SelectProgramPage : public QWizardPage
{
    Q_OBJECT

private:
	ProgramManager *programManager;

#ifdef __WIN32
        RegistryManager *regMan;
#endif

public:
        SelectProgramPage(QWidget* parent);
        ~SelectProgramPage();

        QStringList* getAllFormats(QString format);
        QString getExecPath();
        QString getName();

        QVBoxLayout *vboxLayout;
        QListWidget *lwCategories;
        QListWidget *lwPrograms;
        QHBoxLayout *hboxLayout;


    public slots:
        void searchForPrograms();
};

#endif
