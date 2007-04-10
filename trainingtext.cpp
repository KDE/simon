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
 * \param int pageCount
 * count of pages in the text
 * \param float relevance
 * The relevance of the text - the higher the better it'd be to train the text
 */
TrainingText::TrainingText(  QString name, int pageCount, float relevance  )
{
	this->name = name;
	this->pageCount = pageCount;
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


