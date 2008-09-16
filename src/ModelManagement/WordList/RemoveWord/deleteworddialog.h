//
// C++ Interface: deleteworddialog
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef DELETEWORDDIALOG_H
#define DELETEWORDDIALOG_H

#include <KDialog>
#include "ui_removeword.h"

/**
 \class DeleteWordDialog
 \author Peter Grasch
 \date 13.12.2007
 \brief Shows a dialog which lets the user decide if he just wants to move the word to the shadow list or delete it
 \version 0.1
*/
class Word;

class DeleteWordDialog : public KDialog
{

private:
	Ui::DlgRemoveWord ui;
public:

enum DeletionType {
MoveToShadow			= 0x00,
RemoveCompletely		= 0x01
};

    DeleteWordDialog(QWidget* parent=0, Qt::WindowFlags f=0);

	int exec(Word word, bool isShadowed);

	DeletionType getDeletionType()
	{
		return (ui.rbShadow->isChecked()) ? MoveToShadow : RemoveCompletely;
	}

};

#endif
