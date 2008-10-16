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


#include "generalsettings.h"
#include <KMessageBox>
#include <knewpassworddialog.h>
#include <KLocalizedString>
#include "coreconfiguration.h"
#include <kgenericfactory.h>

K_PLUGIN_FACTORY( GeneralSettingsFactory, 
			registerPlugin< GeneralSettings >(); 
		)
        
K_EXPORT_PLUGIN( GeneralSettingsFactory("GeneralSettings") )
/**
 * \brief Constructs a new GeneralSettings object
 * \author Peter Grasch
 * @param parent The parent of the widget
 */
GeneralSettings::GeneralSettings(QWidget* parent, const QVariantList& args): 
				KCModule(KGlobal::mainComponent(), parent)
{
	Q_UNUSED(args);

	ui.setupUi(this);
	connect(ui.kcfg_PasswordProtected, SIGNAL(toggled(bool)), this, SLOT(checkPassword()));
	connect(ui.pbResetPassword, SIGNAL(clicked()), this, SLOT(newPassword()));


	addConfig(CoreConfiguration::self(), this);
}


void GeneralSettings::checkPassword()
{
	if (ui.kcfg_PasswordProtected->isChecked() && CoreConfiguration::adminPassword().isEmpty())
	{
		newPassword();

		if (CoreConfiguration::adminPassword().isEmpty()) //still?
			ui.kcfg_PasswordProtected->setChecked(false);
	}
}

void GeneralSettings::newPassword()
{
	KNewPasswordDialog *dlg = new KNewPasswordDialog( this );
	dlg->setPrompt( i18n( "Bitte geben Sie das gewünschte Systemverwaltungspasswort an." ));
	if (dlg->exec())
	{
		CoreConfiguration::setAdminPassword(dlg->password());
	}
}


/**
 * \brief Destructor
 * \author Peter Grasch
 */
GeneralSettings::~GeneralSettings()
{
}


