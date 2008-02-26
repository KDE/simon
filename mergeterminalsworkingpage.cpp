//
// C++ Implementation: mergeterminalworkingpage
//
// Description:
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "mergeterminalsworkingpage.h"
#include "grammarmanager.h"
#include "mergeterminals.h"

MergeTerminalsWorkingPage::MergeTerminalsWorkingPage (QWidget* parent ) : QWizardPage ( parent )
{
	ui.setupUi ( this );

	this->mergeTerminals = new MergeTerminals ( this );
	connect ( mergeTerminals, SIGNAL ( status ( QString ) ), this, SLOT ( displayStatus ( QString ) ) );
	connect ( mergeTerminals, SIGNAL ( done() ), this, SLOT ( finished() ) );
	connect ( mergeTerminals, SIGNAL ( progress ( int, int ) ), this, SLOT ( displayProgress ( int, int ) ) );

	setTitle ( tr ( "Verbinde..." ) );
}

void MergeTerminalsWorkingPage::finished()
{
	this->complete = true;
	emit completeChanged();
	emit done();
}

void MergeTerminalsWorkingPage::initializePage()
{
	complete = false;
	mergeTerminals->setTerminalA ( field ( "terminalA" ).toString() );
	mergeTerminals->setTerminalB ( field ( "terminalB" ).toString() );
	mergeTerminals->setNewName ( field ( "newName" ).toString() );
	mergeTerminals->setIncludeShadow ( field ( "includeShadow" ).toBool() );
	mergeTerminals->start();
}


void MergeTerminalsWorkingPage::displayProgress ( int progress, int max )
{
	ui.pbProgress->setMaximum ( max );
	ui.pbProgress->setValue ( progress );
	QCoreApplication::processEvents();
}

void MergeTerminalsWorkingPage::displayStatus ( QString status )
{
	ui.lbStatus->setText ( status );
	QCoreApplication::processEvents();
}


MergeTerminalsWorkingPage::~MergeTerminalsWorkingPage()
{
    mergeTerminals->deleteLater();
}


