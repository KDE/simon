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

#ifndef COMMANDCONFIGURATION_H
#define COMMANDCONFIGURATION_H

#include "simonmodelmanagement_export.h"
#include <simonscenariobase/scenarioobject.h>
#include <QWidget>

class Scenario;
class QString;
class KComponentData;
class KAboutData;

class MODELMANAGEMENT_EXPORT CommandConfiguration : public QWidget, public ScenarioObject
{
	Q_OBJECT
	signals:
		void changed(bool);
	protected:
		KAboutData *about;
	
	private slots:
		void slotChanged();
	
	public:
		CommandConfiguration(Scenario *parent, const QByteArray& internalName, const KLocalizedString& name, 
				      const QByteArray& version, const KLocalizedString& desc,
				      const QString& iconName, const KComponentData& componentData,
				      const QVariantList &args = QVariantList());
		~CommandConfiguration();
		KAboutData* aboutData() { return about; }
		
	public slots:
		virtual bool deSerialize(const QDomElement&)=0;
		virtual QDomElement serialize(QDomDocument *doc)=0;
	/*	virtual void save()=0;
		virtual void load()=0;*/
		virtual void defaults()=0;
		
};

#endif
