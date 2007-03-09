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
	this->actstate=0;
	spellui.setupUi(this);
	//recordui.setupUi(this);
	connect(spellui.pbNext, SIGNAL(clicked()), this, SLOT(nextStep()));
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
 * The current state is determined by the member-variable
 * actstate. It reassambles the state by a integer value.
 * "0"=>"Spell the word"
 * "1"=>"Record two samples of the word"
 * "2"=>"Completion"
 *
 *	@author Peter Grasch
 * @see prevStep()
*/
void AddWordView::nextStep()
{
	this->close();
	for (int i=0; i<this->children().size(); i++)
		this->children().at(i)->deleteLater();
	qApp->processEvents();
	recordui.setupUi(this);
	this->show();
	connect(recordui.pbNext, SIGNAL(clicked()), this, SLOT(accept()));
	connect(recordui.pbBack, SIGNAL(clicked()), this, SLOT(prevStep()));
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
 * The current state is determined by the member-variable
 * actstate. It reassambles the state by a integer value.
 * "0"=>"Spell the word"
 * "1"=>"Record two samples of the word"
 * "2"=>"Completion"
 *
 *	@author Peter Grasch
 * @see nextStep()
*/
void AddWordView::prevStep()
{
	this->close();
	for (int i=0; i<this->children().size(); i++)
		this->children().at(i)->deleteLater();
	qApp->processEvents();
	spellui.setupUi(this);
	this->show();
	connect(spellui.pbNext, SIGNAL(clicked()), this, SLOT(nextStep()));
	connect(spellui.pbBack, SIGNAL(clicked()), this, SLOT(reject()));
}

/**
 * @brief Destructor
 *
 *	@author Peter Grasch
*/
AddWordView::~AddWordView()
{

}

