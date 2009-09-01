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

#include <KDialog>
#include "keyboardbutton.h"
#include "ui_addbuttondlg.h"

class KeyboardAddButtonDLG : public KDialog
{
	Q_OBJECT
			
	private:
		Ui::AddButtonDlg ui;
                bool *addOk;
                int exec();
		
	public:
                KeyboardAddButtonDLG(QWidget *parent=0);
                KeyboardButton *addButton();
		~KeyboardAddButtonDLG();
		
};

#endif
