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
#include <QLineEdit>
#include <QComboBox>
#include <QHBoxLayout>

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

        QString getPlacePath();

private:
        QComboBox *cbTyp;
        QLabel *lAdress;
        QLineEdit *leHost;
        QLabel *lUser;
        QLineEdit *leUser;
        QLabel *lPassword;
        QLineEdit *lePassword;

        void setWidgetsVisible(bool visible);

private slots:
        void writeTyp();
};

#endif
