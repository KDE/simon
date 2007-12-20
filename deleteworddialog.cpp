//
// C++ Implementation: deleteworddialog
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "deleteworddialog.h"
#include "word.h"

DeleteWordDialog::DeleteWordDialog(QWidget* parent, Qt::WindowFlags f): QDialog(parent, f)
{
	ui.setupUi(this);
}

int DeleteWordDialog::exec(Word word, bool isShadowed)
{
	ui.lbName->setText(word.getWord());
	ui.lbPronunciation->setText(word.getPronunciation());
	ui.lbTerminal->setText(word.getTerminal());
	ui.lbRecognitionRate->setText(QString::number(word.getPropability()));
	if (isShadowed)
	{
		ui.lbShadowDesc->setEnabled(false);
		ui.rbShadow->setEnabled(false);
		ui.rbDelete->setChecked(true);
	} else {
		ui.lbShadowDesc->setEnabled(true);
		ui.rbShadow->setEnabled(true);
		ui.rbShadow->setChecked(true);
	}
	return QDialog::exec();
}

DeleteWordDialog::~DeleteWordDialog()
{
}


