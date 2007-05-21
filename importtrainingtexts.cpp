//
// C++ Implementation: importtrainingtexts
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "importtrainingtexts.h"


/**
 * \brief Constructor
 * \author Peter Grasch
 */
ImportTrainingTexts::ImportTrainingTexts(QWidget *parent) : QWizard(parent)
{
	
}


/**
 * \brief Shows the wizard
 * \author Peter Grasch
 */
void ImportTrainingTexts::start()
{
	QMessageBox::information(this, "Hallo", "Das ist ein Test");
}


/**
 * \brief Destructor
 * \author Peter Grasch
 */
ImportTrainingTexts::~ImportTrainingTexts()
{
}


