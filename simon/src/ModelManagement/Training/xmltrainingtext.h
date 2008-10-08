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

#ifndef XMLTRAININGTEXT_H
#define XMLTRAININGTEXT_H

#include <QStringList>
#include <simonxml/xmldomreader.h>

/**
 *  @class XMLTrainingText
 *  @brief To handle with xml-documents
 *
 *  @version 0.1
 *  @date 17.03.2007
 *  @author Christoph Kirschner, Peter Grasch
 */
class XMLTrainingText : public XMLDomReader
{
private:
	QString title;
	QStringList pages;
public: 
	XMLTrainingText(QString path="");
	
	int getPageCount();
	void save(QString path="");
	
	void load(QString path="");
	
	QString getTitle();
	
	QString getPage(int index);
	
	QStringList getAllPages();
	
	void addPages(QStringList pages);
	void setTitle(QString title) { this->title =title; }
	
	
	~XMLTrainingText();
};

#endif
