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
#include "../word.h"

/**
 * \brief Constructor
 * \author Peter Grasch
 * @param parent The parent of the dialog
 * @param f Window flags
 */
DeleteWordDialog::DeleteWordDialog(QWidget* parent, Qt::WindowFlags f): QDialog(parent, f)
{
	ui.setupUi(this);
}

/**
 * \brief Executes the dialog
 * @param word The word to delete
 * @param isShadowed Returns true if the word is in the shadow lexicon
 * @return False, if the dialog is aborted
 */
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
