/*
 *   Copyright (C) 2009 Peter Grasch <grasch@simon-listens.org>
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


#ifndef SCENARIOOBJECT_H
#define SCENARIOOBJECT_H
#include <QString>
#include <QDomElement>
#include "speechmodelbase_export.h"

class SPEECHMODELBASE_EXPORT ScenarioObject{

protected:
	QString scenarioId;
public:
	
	ScenarioObject(const QString& scenarioId)
	{
		this->scenarioId = scenarioId;
	}

	virtual bool deSerialize(const QDomElement&)=0;

	virtual QDomElement serialize(QDomDocument *doc)=0;

	virtual ~ScenarioObject() {}

};

#endif
