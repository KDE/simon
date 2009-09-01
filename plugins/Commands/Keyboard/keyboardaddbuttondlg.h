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

#ifndef KEYBOARDADDBUTTONDLG_H
#define KEYBOARDADDBUTTONDLG_H

#include "ui_keyboardaddbuttondlg.h"

class KeyboardAddButtonDLG : public KDialog
{
	private:
		Ui::KeyboardAddButtonDLG ui;
		bool *addOk;
		
	private slots:
		void ok();
		void abbort();
		
	public:
		KeyboardAddButtonDLG(QWidget *parent=0);
		KeyboardAddButtonDLG(QWidget *parent=0, bool *ok=false);
		QString getName();
		QString getTrigger();
		QString getValue();
		short getValueType();
		~KeyboardAddButtonDLG();
		
};

#endif
