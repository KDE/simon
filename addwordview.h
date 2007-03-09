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


/**
 *	@class AddWordView
 *	@brief The Dialog to add a new word to the Model
 *
 *	This dialog is using a step-by-step guide through the procedure to
 *	add a new word to the Language Model.
 *	It uses three different UI files - one for each step.
 *
 *	@version 0.1
 *	@date 08.01.2006
 *	@author Peter Grasch
 *	@todo Implementing functions
*/
 
#ifndef ADDWORDVIEW_H
#define ADDWORDVIEW_H

#include <QMainWindow>
#include <QMessageBox>
#include "simoninfo.h"
#include "ui_addwordspell.h"
#include "ui_addwordrecord.h"


class AddWordView : public QDialog
{
	Q_OBJECT

	private:
		Ui::AddWordSpell spellui; //!< AddWordSpell UI definition - made by uic from the QTDesigner .ui
		int actstate; //!< defines the current state the wizard is in - [0-2]
		//SimonInfo info;
//		Ui::AddWordRecord recordui; //!< AddWordRecord UI definition - made by uic from the QTDesigner .ui
		Ui::AddWordRecord recordui;
	public slots:
		void nextStep();
		void prevStep();
		
	public:
		AddWordView(QWidget *parent=0, Qt::WFlags flags=0);
		~AddWordView();
};

#endif
