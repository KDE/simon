//
// C++ Implementation: trainingtext
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "trainingtext.h"

/**
 * @brief Constructor
 *
 *	@author Peter Grasch
 * \param QString name
 * The name of the text
 * \param QStringList pages
 * The pages of the text
 * \param float relevance
 * The relevance of the text - the higher the better it'd be to train the text
 */
TrainingText::TrainingText(  QString name, QString path, QStringList pages, 
			     float relevance  )
{
	this->name = name;
	this->path = path;
	this->pages = pages;
	this->relevance = relevance;
}


/**
 * @brief Destructor
 *
 *	@author Peter Grasch
 */
TrainingText::~TrainingText()
{
}


