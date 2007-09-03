//
// C++ Interface: localplacepage
//
// Description:
//
//
// Author: Susanne Tschernegg
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LOCALPLACEPAGE_H
#define LOCALPLACEPAGE_H

#include <QWizardPage>
#include <QLabel>
#include <QVBoxLayout>

/**
	@author Susanne Tschernegg
*/
/**
 * \class $CLASSNAME
 * \author Susanne Tschernegg
 * \date
 * \version 0.1
 * \brief
 */
class LocalPlacePage : public QWizardPage
{
    Q_OBJECT
public:
        LocalPlacePage(QWidget* parent);

        ~LocalPlacePage();

private:
        QVBoxLayout *vboxLayout;
        QLabel *label;
};

#endif
