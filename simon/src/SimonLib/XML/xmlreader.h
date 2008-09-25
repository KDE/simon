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

#ifndef XMLREADER_H
#define XMLREADER_H
#include <QObject>
/**
 *  @class XMLReader
 *  @brief To handle with xml-files
 *
 *  @version 0.1
 *  @date 17.03.2007
 *  @author Martin Gigerl, Peter Grasch
 */
 

 
class XMLReader : public QObject {
 	Q_OBJECT
signals:
	void loaded();
	void closed();
	void written();

protected:
 	QString path;
 
public:
	explicit XMLReader(QString path, QObject *parent=0);
	
	~XMLReader();
};

#endif
