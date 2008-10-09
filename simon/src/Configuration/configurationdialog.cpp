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

#include "configurationdialog.h"
#include "coreconfiguration.h"

#include <KConfigDialog>
#include <KCModule>
#include <KAboutData>

ConfigurationDialog* ConfigurationDialog::instance=0;


ConfigurationDialog::ConfigurationDialog(QWidget *parent) : QObject(parent)
{

}

KConfigDialog* ConfigurationDialog::configDialog()
{
	KConfigDialog *dlg = KConfigDialog::exists("coreconfiguration");
	if (!dlg) {
		QWidget *parentWidget = dynamic_cast<QWidget*>(parent());

		dlg = new KConfigDialog(parentWidget, "coreconfiguration", CoreConfiguration::self());
	}
	
	return dlg;
}

void ConfigurationDialog::show()
{
	if ( KConfigDialog::showDialog( "coreconfiguration" ) )
		return;

	KConfigDialog* dialog = configDialog();
	if (!dialog) return;

	//User edited the configuration - update your local copies of the 
	//configuration data 
	// connect( dialog, SIGNAL(settingsChanged()), 
	// 	this, SLOT(updateConfiguration()) ); 
	
	dialog->show();
}


bool ConfigurationDialog::registerManagedWidget(QWidget *widget, const QString& title, const QByteArray& iconName)
{
	Q_ASSERT(widget);
	
	KConfigDialog *dlg = configDialog();
	if (!dlg) return false;
	
	dlg->addPage( widget, title, iconName); 
	return true;
}

bool ConfigurationDialog::registerModule(KCModule *module)
{
	if (!module) return false;
	
	KConfigDialog *dlg = configDialog();
	if (!dlg) return false;
	
	const KAboutData *about = module->aboutData();
	if (!about) return false;
	
	KPageWidgetItem *page = dlg->addPage( module, about->programName(), 
					      about->programIconName(), 
					      QString()); 
	
	connect (module, SIGNAL(changed(bool)), this, SLOT(moduleChanged(bool)));
// 	connect (module, SIGNAL(destroyed(QObject*)), this, SLOT(moduleDestroyed(QObject*)));
	connect (dlg, SIGNAL(applyClicked()), module, SLOT(save()));
	connect (dlg, SIGNAL(okClicked()), module, SLOT(save()));
	connect (dlg, SIGNAL(cancelClicked()), module, SLOT(load()));
	connect (dlg, SIGNAL(defaultClicked()), module, SLOT(defaults()));
	pluginChangedStatus.insert(module, false);
	pluginConfigPage.insert(module, page);
	
	return true;
}

void ConfigurationDialog::unregisterModule(KCModule *module)
{
	pluginChangedStatus.remove(module);
	
	KConfigDialog *dlg = configDialog();
	if (!dlg) return;
	
	dlg->removePage(pluginConfigPage.value(module));
}

void ConfigurationDialog::moduleChanged(bool changed)
{
	KConfigDialog *dlg = configDialog();
	if (!dlg) return;
	
	KCModule *senderModule = dynamic_cast<KCModule*>(sender());
	if (!senderModule) return;
	
	pluginChangedStatus.insert(senderModule, changed);
	
	bool allChanged = pluginChangedStatus.values().contains(true);
	dlg->enableButton( KDialog::Apply, allChanged );
	dlg->enableButton( KDialog::Default, allChanged );
}


ConfigurationDialog::~ConfigurationDialog()
{
	instance=0;
}
