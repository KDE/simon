//
// C++ Implementation: remoteplacepage
//
// Description:
//
//
// Author: Susanne Tschernegg
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "remoteplacepage.h"
#include "settings.h"
#include "command.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QHBoxLayout>

/**
*   \brief constructor. creates the widgets and connects
*   @author Susanne Tschernegg
*   @param QWidget *parent
*/
RemotePlacePage::RemotePlacePage ( QWidget* parent ) : QWizardPage ( parent )
{
	QVBoxLayout *vboxLayout = new QVBoxLayout ( this );

	QLabel *label = new QLabel ( this );
	label->setText ( tr ( "Wählen Sie bitte den Typ des Ortes aus und geben Sie anschließend den Rest der URL ein." ) );
	label->setWordWrap ( true );

	QHBoxLayout *hbLayout1 = new QHBoxLayout();
	QLabel *lTypText = new QLabel ( this );
	lTypText->setText ( tr ( "Typ:" ) );
	cbTyp = new QComboBox ( this );
	cbTyp->addItem ( "http" );
	cbTyp->addItem ( "https" );
	cbTyp->addItem ( "ftp" );
	cbTyp->addItem ( "sftp" );
	cbTyp->addItem ( "smb" );
	hbLayout1->addWidget ( lTypText );
	hbLayout1->addWidget ( cbTyp );

	lAdress = new QLabel ( this );
	lAdress->setText ( "http://" );

	QHBoxLayout *hbLayout2 = new QHBoxLayout();
	QLabel * lHost = new QLabel ( this );
	lHost->setText ( tr ( "Host: " ) );
	leHost = new QLineEdit ( this );
	hbLayout2->addWidget ( lHost );
	hbLayout2->addWidget ( leHost );

	QHBoxLayout *hbLayout3 = new QHBoxLayout();
	lUser = new QLabel ( this );
	lUser->setText ( tr ( "User: " ) );
	leUser = new QLineEdit ( this );
	hbLayout3->addWidget ( lUser );
	hbLayout3->addWidget ( leUser );

	QHBoxLayout *hbLayout4 = new QHBoxLayout();
	lPassword = new QLabel ( this );
	lPassword->setText ( tr ( "Passwort: " ) );
	lePassword = new QLineEdit ( this );
	hbLayout4->addWidget ( lPassword );
	hbLayout4->addWidget ( lePassword );

	vboxLayout->addWidget ( label );
	vboxLayout->addLayout ( hbLayout1 );
	vboxLayout->addWidget ( lAdress );
	vboxLayout->addLayout ( hbLayout2 );
	vboxLayout->addLayout ( hbLayout3 );
	vboxLayout->addLayout ( hbLayout4 );

	setWidgetsVisible ( false );

	connect ( cbTyp, SIGNAL ( activated ( int ) ), this, SLOT ( writeTyp() ) );
	connect ( leHost, SIGNAL ( textChanged ( const QString & ) ), this, SLOT ( writeTyp() ) );
	connect ( leUser, SIGNAL ( textChanged ( const QString & ) ), this, SLOT ( writeTyp() ) );
	connect ( lePassword, SIGNAL ( textChanged ( const QString & ) ), this, SLOT ( writeTyp() ) );
}

/**
*   \brief destructor
*   @author Susanne Tschernegg
*/
RemotePlacePage::~RemotePlacePage()
{
    cbTyp->deleteLater();
    lAdress->deleteLater();
    leHost->deleteLater();
    lUser->deleteLater();
    leUser->deleteLater();
    lPassword->deleteLater();
    lePassword->deleteLater();
}

/**
*   \brief This method writes the type to the adresslabel, if the type of the place changes or if the text of any lineEdit changes.
*   @author Susanne Tschernegg
*/
void RemotePlacePage::writeTyp()
{
	lAdress->clear();
	if ( cbTyp->currentText() =="http" )
	{
		setWidgetsVisible ( false );
		lAdress->setText ( cbTyp->currentText() +"://"+leHost->text() );
	}
	else
	{
		setWidgetsVisible ( true );
		if ( leUser->text() !="" && lePassword->text() !="" && leHost->text() !="" )
			lAdress->setText ( cbTyp->currentText() +"://"+leUser->text() +":"+lePassword->text() +"@"+leHost->text() );
		else
			lAdress->setText ( cbTyp->currentText() +"://"+leHost->text() );

	}
}

/**
*   \brief to geht the whole path of the place
*   @author Susanne Tschernegg
*   @return QString
*       returns the path of the place
*/
QString RemotePlacePage::getPlacePath()
{
	return lAdress->text();
}

/**
*   \brief It asks for the vixible state of the widgets, which depents on the type of the place. It will be called, after the type of the remoteplace changes.
*   @author Susanne Tschernegg
*   @param bool visible
*       holds wheater the widgets are visible or not
*/
void RemotePlacePage::setWidgetsVisible ( bool visible )
{
	lUser->setVisible ( visible );
	leUser->setVisible ( visible );
	lPassword->setVisible ( visible );
	lePassword->setVisible ( visible );
	if ( visible==false )
	{
		leUser->clear();
		lePassword->clear();
	}
}
