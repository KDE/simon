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
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QFileDialog>

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

        QString getPlacePath();

private:
        QVBoxLayout *vboxLayout;
        QLabel *label;
        QLabel *lOrdner;
        QHBoxLayout *hboxLayout;
        QLineEdit *leOrdner;
        QPushButton *pbOrdner;
        QFileDialog *dialog;

private slots:
        void openFileDialog();
};

#endif
