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
#include <QCryptographicHash>
#include <QMessageBox>
#include <QString>
#include "../Settings/settings.h"

/**
*   \brief constructor which creats the dialog
*   @author Susanne Tschernegg
*   @param QWidget *parent
*/
PasswordDlg::PasswordDlg(QWidget *parent): QDialog(parent)
{
    setWindowTitle(tr("Zugriff verweigert"));
    
    QVBoxLayout *vbLayout = new QVBoxLayout();
	QLabel *label = new QLabel(tr("Dieser Bereich ist passwortgeschützt.\n\nBitte authentifizieren Sie sich:\n"));
	label->setWordWrap(true);
    vbLayout->addWidget(label);
    
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
    
    connect(pbOk, SIGNAL(clicked()), this, SLOT(checkPassword()));
    connect(pbCancel, SIGNAL(clicked()), this, SLOT(reject()));
}

void PasswordDlg::checkPassword()
{
    QString password = Settings::getS ( "Password" );

	QCryptographicHash *hasher = new QCryptographicHash(QCryptographicHash::Md5);
	hasher->addData(lePassword->text().toLatin1());
	QString hash = hasher->result();

	if ( password.compare ( hash ) !=0 )
	{
		int result = QMessageBox::question ( this, tr ( "Falsches Passwort" ), tr ( "Sie haben ein falsches Passwort eingegeben.\n\nWollen Sie das Passwort erneut eingeben?" ),
		                                     QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel );

		if ( result == QMessageBox::Yes )
		{
			return;
		}
		else
		{
			reject();
		}
	}
    else
    {
        accept();    
    }
}

/**
*   \brief destructor
*   @author Susanne Tschernegg
*/
//PasswordDlg::~PasswordDlg()
//{}
