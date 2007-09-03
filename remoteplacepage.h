//
// C++ Interface: remoteplacepage
//
// Description:
//
//
// Author: Susanne Tschernegg
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef REMOTEPLACEPAGE_H
#define REMOTEPLACEPAGE_H

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
class RemotePlacePage : public QWizardPage
{
    Q_OBJECT
public:
        RemotePlacePage(QWidget* parent);

        ~RemotePlacePage();

private:
        QVBoxLayout *vboxLayout;
        QLabel *label;
};

#endif
