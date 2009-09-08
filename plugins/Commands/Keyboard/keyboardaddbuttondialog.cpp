/*
 *   Copyright (C) 2009 Mario Strametz <strmam06@htl-kaindorf.ac.at>
 *   Copyright (C) 2009 Grasch Peter <grasch@simon-listens.org>
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

#include "keyboardaddbuttondialog.h"
#include <simoninfo/simoninfo.h>
#include <QWidget>
#include <KMessageBox>
#include <kdeversion.h>

KeyboardAddButtonDialog::KeyboardAddButtonDialog(QWidget *parent) : KDialog(parent)
{
	QWidget *w = new QWidget(this);
	ui.setupUi(w);
	
	setMainWidget( w );

	setCaption(i18n("Add key"));

#if KDE_IS_VERSION(4,1,60)
	ui.ksShortcut->setCheckForConflictsAgainst(KKeySequenceWidget::None);
#endif
}

KeyboardButton* KeyboardAddButtonDialog::addButton()
{
	if(exec())
	{
		if(!ui.leName->text().isEmpty() && !ui.leTrigger->text().isEmpty() && !ui.leTextValue->text().isEmpty())
		{
			Keyboard::ButtonType type;
			switch (ui.cbValueType->currentIndex())
			{
				case 0: type = Keyboard::TextButton;
					break;
				case 1: type = Keyboard::ShortcutButton;
					break;
				default: return NULL; //error
			}

			QString value;
			switch (type) {
				case Keyboard::TextButton:
					value = ui.leTextValue->text();
						break;
				case Keyboard::ShortcutButton:
					value = ui.ksShortcut->keySequence().toString();
						break;
				default: return NULL; //error
			}

	   		return (new KeyboardButton(ui.leName->text(), ui.leTrigger->text(), type, value));
		} else {
			KMessageBox::sorry(this, i18n("All fields are mandatory"));
		}
	}

	return NULL;
}

int KeyboardAddButtonDialog::exec()
{
	return KDialog::exec();
}

KeyboardAddButtonDialog::~KeyboardAddButtonDialog()
{
}

