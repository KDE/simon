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
#include <KCModule>

class KSharedConfig;
class QString;
class KComponentData;

class MODELMANAGEMENT_EXPORT CommandConfiguration : public KCModule
{
	Q_OBJECT
	
	protected:
		KSharedConfig::Ptr config;
 
	private slots:
		void slotChanged();
	
	public:
		CommandConfiguration(const QByteArray& internalName, const KLocalizedString& name, 
				      const QByteArray& version, const KLocalizedString& desc,
				      const QString& iconName, const KComponentData& componentData,
				      QWidget *parent=0, const QVariantList &args = QVariantList());
		~CommandConfiguration();
		virtual void destroy();
		
// 		KSharedConfig::Ptr* getConfig() { return &config; }

	public slots:
		virtual void save()=0;
		virtual void load()=0;
		virtual void defaults()=0;
		
};

#endif
