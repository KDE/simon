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
#include <QPushButton>
#include <QSpacerItem>
#include <QVBoxLayout>
#include <QWidget>
#include "registrymanager.h"

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
public:
        SelectProgramPage(QWidget* parent);
        ~SelectProgramPage();

        QStringList* getAllFormats();

        QVBoxLayout *vboxLayout;
        QListWidget *lwCategories;
        QListWidget *lwPrograms;
        QHBoxLayout *hboxLayout;
        QSpacerItem *spacerItem;
        QPushButton *pbAbbrechen;
        QPushButton *pbHinzufuegen;

        RegistryManager *regMan;

    public slots:
        void searchForPrograms();
};

#endif
