/*
 *   Copyright (C) 2009 Dominik Neumeister && Mario Strametz <neudob06@edvhtl.at> & <strmam06@htl-kaindorf.ac.at>
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
#include <QWidget>

KeyboardAddButtonDLG::KeyboardAddButtonDLG(QWidget *parent) : KDialog(parent)
{
	QWidget *w = new QWidget(this);
	ui.setupUi(this);
	setMainWidget( w );
}

KeyboardAddButtonDLG::KeyboardAddButtonDLG(QWidget *parent, bool *ok) : KDialog(parent)
{
	QWidget *w = new QWidget(this);
	ui.setupUi(this);
	setMainWidget( w );
	addOk = ok;
	
	connect(ui.pbOK, SIGNAL(clicked()), this, SLOT(ok()));
	connect(ui.pbAbbort, SIGNAL(clicked()), this, SLOT(abbort()));
}

QString KeyboardAddButtonDLG::getName()
{
	return ui.leName->text();
}


QString KeyboardAddButtonDLG::getTrigger()
{
	return ui.leTrigger->text();
}

QString KeyboardAddButtonDLG::getValue()
{
	return ui.leValue->text();
}


short KeyboardAddButtonDLG::getValueType()
{
	return ui.cbValueType->currentIndex(); // 0 and 1 can be returned ,... 0 is text; 1 is shortcut
}


void KeyboardAddButtonDLG::ok()
{
	ui.hide();
	addOk = true;
}


void KeyboardAddButtonDLG::abbort()
{
	ui.hide();
	addOk = false;
}

KeyboardAddButtonDLG::~KeyboardAddButtonDLG()
{
}

