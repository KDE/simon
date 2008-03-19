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

class QVBoxLayout;
class QLabel;
class QHBoxLayout;
class QLineEdit;
class QPushButton;
class QFileDialog;


/**
 * \class LocalPlacePage
 * \author Susanne Tschernegg
 * \date 03.09.2007
 * \version 0.1
 * \brief At this page the user can choose a directory at his computer to add this to the commandlist. Therefor a filedialog will be called.
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
