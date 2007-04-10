//
// C++ Interface: trainingtext
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef TRAININGTEXT_H
#define TRAININGTEXT_H

#include <QList>
#include <QString>
/**
 *	@class TrainingText
 *	@brief Convenient class to be used as a container to hold all theinformation for one text
 *
 *	@version 0.1
 *	@date 23.01.2006
 *	@author Peter Grasch
 *	@todo Implementing
 */
class TrainingText;
typedef  QList<TrainingText*> TrainingList;

class TrainingText{
private:
	QString name;
	int pageCount;
	float relevance;
public:
	/**
	 * \brief Returns the name of the text
	 * \author Peter Grasch
	 * \return QString
	 * Name
	 */
	QString getName() { return this->name; }
	
	/**
	 * \brief Returns the page count of the text
	 * \author Peter Grasch
	 * \return QString
	 * count of pages
	 */
	int getPageCount() { return this->pageCount; }
	
	/**
	 * \brief Returns the relevance of the text
	 * 
	 * The higher the value, the more the text would improve the recognition rate
	 * 
	 * \author Peter Grasch
	 * \return float
	 * relevance
	 */
	float getRelevance() { return this->relevance; }
	
    TrainingText( QString name, int pageCount, float relevance );

    ~TrainingText();

};

#endif
