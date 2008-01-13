//
// C++ Interface: introplacepage
//
// Description:
//
//
// Author: Susanne Tschernegg
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef INTROPLACEPAGE_H
#define INTROPLACEPAGE_H

#include <QWizardPage>
#include <QRadioButton>

class QLabel;
class QVBoxLayout;
//class QRadioButton;

/**
	@author Susanne Tschernegg
*/
/**
 * \class $CLASSNAME
 * \author Susanne Tschernegg
 * \date 12.09.2007
 * \version 0.1
 * \brief This is the first page of the ImportPlacePage, which only includes a text and the user can choose, if he wants to create a localplace-command or
 *          a remoteplace-command.
 */
class IntroPlacePage : public QWizardPage
{
    Q_OBJECT
public:
        IntroPlacePage(QWidget* parent);

        ~IntroPlacePage();

        QRadioButton *rbLocalPlace;
        QRadioButton *rbRemotePlace;

private:
        QVBoxLayout *vboxLayout;
        QLabel *label;

signals:
    void placeChanged();
};

#endif
