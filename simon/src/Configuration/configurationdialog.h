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


#ifndef CONFIGURATIONDIALOG_H
#define CONFIGURATIONDIALOG_H

#include <QObject>
#include <QHash>

class KCModule;
class KConfigDialog;

class ConfigurationDialog : public QObject
{
	Q_OBJECT

	private:
		static ConfigurationDialog* instance;
		
		QHash<KCModule*,bool> pluginCompletionStatus;
		ConfigurationDialog(QWidget *parent);
		
		KConfigDialog* configDialog();

	private slots:
		void moduleChanged(bool complete);

	public:
		static ConfigurationDialog* getInstance(QWidget *parent=0)
		{
			if (!instance) instance = new ConfigurationDialog(parent);
			return instance;
		}
		
		void show();
		
		bool registerManagedWidget(QWidget *widget, const QString& title, const QByteArray& iconName);
		
		bool registerModule(KCModule *module);
		virtual ~ConfigurationDialog();

};

#endif
