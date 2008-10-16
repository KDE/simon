/*
 *   Copyright (C) 2008 Peter Grasch <grasch@simon-listens.org>
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


#ifndef TRAININGTEXT_H
#define TRAININGTEXT_H

#include <QStringList>
#include "speechmodelbase_export.h"
/**
 *	@class TrainingText
 *	@brief Convenient class to be used as a container to hold all theinformation for one text
 *
 *	@version 0.1
 *	@date 23.01.2006
 *	@author Peter Grasch
 */
class TrainingText;

class SPEECHMODELBASE_EXPORT TrainingText{
protected:
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

	/**
	 * \brief reimplement this in your *TrainingText class to store the text
	 * \author Peter Grasch
	 */
	virtual bool save()
	{ return true; }
// 	bool save();

	virtual ~TrainingText();

};

#endif
