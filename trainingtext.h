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
#include <QStringList>
#include <QString>
/**
 *	@class TrainingText
 *	@brief Convenient class to be used as a container to hold all theinformation for one text
 *
 *	@version 0.1
 *	@date 23.01.2006
 *	@author Peter Grasch
 */
class TrainingText;
typedef  QList<TrainingText*> TrainingList;

class TrainingText{
private:
	QString name;
	QString path;
	QStringList pages;
	float relevance;
	public:
	/**
	 * \brief Returns the name of the text
	 * \author Peter Grasch
	 * \return
	 * Name
	 */
	QString getName() { return this->name; }
	
	/**
		 * \brief Returns the path to the text
		 * \author Peter Grasch
		 * \return
		 * path
	 */
	QString getPath() { return this->path; }
	
	/**
	 * \brief Returns the page count of the text
	 * \author Peter Grasch
	 * \return
	 * count of pages
	 */
	int getPageCount() { return pages.count(); }
	
	/**
	 * \brief Returns the pages of the text
	 * \author Peter Grasch
	 * \return
	 * pages
	 */
	QStringList getPages() { return pages; }
	
	/**
	 * \brief Returns the page <page> of the text
	 * \author Peter Grasch
	 * \return 
	 * page
	 */
	QString getPage( int page ) { return pages.at(page); }
	
	
	/**
	 * \brief Returns the relevance of the text
	 * 
	 * The higher the value, the more the text would improve the recognition rate
	 * 
	 * \author Peter Grasch
	 * \return 
	 * relevance
	 */
	float getRelevance() { return this->relevance; }
	
	
	/**
	 * \brief Sets the relevance of the current text to the given value
	 * \author Peter Grasch
	 * @param relevance
	 * The relevance
	 */
	void setRelevance( float relevance ) {	this->relevance = relevance; }
	
    TrainingText( QString name, QString path, QStringList pages );

    ~TrainingText();

};

#endif
