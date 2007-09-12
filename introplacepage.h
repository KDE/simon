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
 * \date
 * \version 0.1
 * \brief
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
