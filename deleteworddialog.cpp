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

int DeleteWordDialog::exec(Word word)
{
	ui.lbName->setText(word.getWord());
	ui.lbPronunciation->setText(word.getPronunciation());
	ui.lbTerminal->setText(word.getTerminal());
	ui.lbRecognitionRate->setText(QString::number(word.getPropability()));
	return QDialog::exec();
}

DeleteWordDialog::~DeleteWordDialog()
{
}


