/***************************************************************************
 *   Copyright (C) 2006 by Peter Grasch   *
 *   bedahr@gmx.net   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
 
 #include "addwordview.h"

/**
 * @brief Constructor
 *
 * Constructs a new AddWordView Object.
 * Initializes the UI and sets it to the first page
 *
 *	@author Peter Grasch
 * @param QWidget *parent
 * Parent of the Dialog; Default: 0
 * @param Qt::Wflags f
 * Qt Windowflags - default 0
*/

AddWordView::AddWordView(QWidget *parent, Qt::WFlags f)
	: QDialog (parent, f)
{
	ui.setupUi(this);
	connect(ui.pbNext, SIGNAL(clicked()), this, SLOT(nextStep()));
	connect(ui.pbBack, SIGNAL(clicked()), this, SLOT(prevStep()));
	connect(ui.pbNext_2, SIGNAL(clicked()), this, SLOT(nextStep()));
	connect(ui.pbBack_2, SIGNAL(clicked()), this, SLOT(prevStep()));
}

/**
 * @brief Sends the wizard to the next step
 *
 * This function sends the wizard to the next step.
 * There are three steps in this wizard.
 * 	* Spell the word
 * 	* Record two samples of the word
 * 	* Completion
 *
 * 
 * If there is no step left, it calls the method finish()
 *
 *	@author Peter Grasch
 * @see prevStep() finish()
*/
void AddWordView::nextStep()
{
	if (ui.swMain->currentIndex() + 1 < ui.swMain->count())
		ui.swMain->setCurrentIndex( ui.swMain->currentIndex()+1 );
	else finish();
}

void AddWordView::finish()
{
	//finishs up
}

/**
 * @brief Sends the wizard to the previous step
 *
 * This function sends the wizard to the previous step.
 * There are three steps in this wizard.
 * 	* Spell the word
 * 	* Record two samples of the word
 * 	* Completion
 *
 * If there is no previous step it rejects the wizard
 *
 *	@author Peter Grasch
 * @see nextStep()
*/
void AddWordView::prevStep()
{
	if (ui.swMain->currentIndex() - 1 >= 0)
		ui.swMain->setCurrentIndex( ui.swMain->currentIndex()-1 );
	else reject();
}

/**
 * @brief Destructor
 *
 *	@author Peter Grasch
*/
AddWordView::~AddWordView()
{

}

