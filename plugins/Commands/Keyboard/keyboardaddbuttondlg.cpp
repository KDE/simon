/*
 *   Copyright (C) 2009 Mario Strametz <strmam06@htl-kaindorf.ac.at>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License version 2,
 *   or (at your option) any later version, as published by the Free
 *   Software Foundation
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "keyboardaddbuttondlg.h"
#include <simoninfo/simoninfo.h>
#include <QWidget>

KeyboardAddButtonDLG::KeyboardAddButtonDLG(QWidget *parent) : KDialog(parent)
{
	QWidget *w = new QWidget(this);
	ui.setupUi(this);
	setMainWidget( w );
	
}

KeyboardButton* KeyboardAddButtonDLG::addButton()
{
	if(exec() == 1)
	{
		if(ui.leName->text() != "" && ui.leTrigger->text() != "" && ui.leValue->text() != "")
		{
	   		return (new KeyboardButton(ui.leName->text(), ui.leTrigger->text(), ui.cbValueType->currentIndex(), ui.leValue->text()));
		}
		else
		{
			SimonInfo::showMessage(i18n("Please fill out the add-button-dialog correctly"), 3000, new KIcon("accessories-calculator"));
			return NULL;
		}
	}
	else
		return NULL;
}

int KeyboardAddButtonDLG::exec()
{
	return KDialog::exec();
}

KeyboardAddButtonDLG::~KeyboardAddButtonDLG()
{
}

