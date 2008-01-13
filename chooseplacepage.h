//
// C++ Interface: chooseplacepage
//
// Description:
//
//
// Author: Susanne Tschernegg
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef CHOOSEPLACEPAGE_H
#define CHOOSEPLACEPAGE_H

#include <QWizardPage>
#include <QLabel>
#include <QVBoxLayout>

/**
 * \class $CLASSNAME
 * \author Susanne Tschernegg
 * \date 03.09.2007
 * \version 0.1
 * \brief This class is a wizardpage where you can choose to import a remoteplace or a localplace. It is used in the ImportPlaceWizard.
 */
class ChoosePlacePage : public QWizardPage
{
    Q_OBJECT
public:
        ChoosePlacePage(QWidget* parent);

        ~ChoosePlacePage();

private:
        QVBoxLayout *vboxLayout;
        QLabel *label;
};
#endif
