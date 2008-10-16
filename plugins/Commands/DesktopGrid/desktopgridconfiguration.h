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

#ifndef DESKTOPGRIDCONFIGURATION_H
#define DESKTOPGRIDCONFIGURATION_H

#include <commandpluginbase/commandconfiguration.h>
#include "ui_desktopgridconfigurationdlg.h"
#include <KSharedConfig>
#include <QPointer>

class DesktopGridConfiguration : public CommandConfiguration
{
	Q_OBJECT
	
	private:
		Ui::DesktopGridConfigurationDlg ui;
		static QPointer<DesktopGridConfiguration> instance;
 
	public slots:
		virtual void save();
		virtual void load();
		virtual void defaults();
	
	public:
		static DesktopGridConfiguration *getInstance(QWidget *parent=0, const QVariantList &args = QVariantList()) {
			if (!instance) instance = new DesktopGridConfiguration(parent, args);
			return instance;
		}
		DesktopGridConfiguration(QWidget *parent=0, const QVariantList &args = QVariantList());
		~DesktopGridConfiguration();
		
		void destroy();
		
		//configuration options
		QString trigger();
		bool useRealTransparency();
};

#endif