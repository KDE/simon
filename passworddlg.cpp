//
// C++ Implementation: passworddlg
//
// Description:
//
//
// Author: Susanne Tschernegg
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "passworddlg.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>

/**
*   \brief constructor which creats the dialog
*   @author Susanne Tschernegg
*   @param QWidget *parent
*/
PasswordDlg::PasswordDlg(QWidget *parent): QDialog(parent)
{
    setWindowTitle(tr("Settingspasswort"));
    
    QVBoxLayout *vbLayout = new QVBoxLayout();
    vbLayout->addWidget(new QLabel(tr("Bitte geben Sie hier das Passwort ein, \num alle Settings anzuzeigen.\n\nPassword:\n")));
    
    lePassword = new QLineEdit();
    lePassword->setEchoMode(QLineEdit::Password);
    vbLayout->addWidget(lePassword);
    
    setLayout(vbLayout);
    QHBoxLayout *hbLayout = new QHBoxLayout();
    
    QPushButton *pbOk = new QPushButton();
    pbOk->setText(tr("Ok"));
    //pbOk->setEditFocus(true);
    
    QPushButton *pbCancel = new QPushButton();
    pbCancel->setText(tr("Abbrechen"));
    
     hbLayout->addWidget(pbOk);
    hbLayout->addWidget(pbCancel);
    vbLayout->addLayout(hbLayout);
    
    connect(pbOk, SIGNAL(clicked()), this, SLOT(accept()));
    connect(pbCancel, SIGNAL(clicked()), this, SLOT(reject()));
}

/**
*   \brief destructor
*   @author Susanne Tschernegg
*/
//PasswordDlg::~PasswordDlg()
//{}
