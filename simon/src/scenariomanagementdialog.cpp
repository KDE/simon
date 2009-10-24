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


#include "scenariomanagementdialog.h"
#include <QWidget>
#include <QListWidget>
#include <QVariant>
#include <QFileInfo>
#include <QListWidgetItem>
#include <KSharedConfig>
#include <KConfigGroup>
#include <KDebug>
#include <KMessageBox>
#include <KStandardDirs>
#include <speechmodelmanagement/author.h>
#include <simonscenariobase/versionnumber.h>
#include <speechmodelmanagement/scenario.h>
#include <speechmodelmanagement/scenariomanager.h>


ScenarioManagementDialog::ScenarioManagementDialog(QWidget *parent) : KDialog(parent)
{
	QWidget *widget = new QWidget( this );
	ui.setupUi(widget);

	setMainWidget( widget );
	setCaption( i18n("Manage scenarios") );

	initDisplay();
}

void ScenarioManagementDialog::initDisplay()
{
	QListWidget *available = ui.asScenarios->availableListWidget();
	QListWidget *selected = ui.asScenarios->selectedListWidget();
	available->clear();
	selected->clear();

	KSharedConfigPtr config = KSharedConfig::openConfig("simonscenariosrc");
	KConfigGroup cg(config, "");
	QStringList  selectedIds = cg.readEntry("SelectedScenarios", QStringList());


	QStringList scenarioSrcs = KGlobal::dirs()->findAllResources("appdata", "scenarios/");
	QStringList scenarioIds;

	foreach (const QString& src, scenarioSrcs) {
		QFileInfo f(src);
		QString idToBe = f.fileName();
		if (!scenarioIds.contains(idToBe)) 
			scenarioIds << idToBe;
	}

	kDebug() << "Found scenarios: " << scenarioIds;
	/*	QString licence() { return m_licence; }
	QString simonMinVersion() { return m_simonMinVersion; }
	QString simonMaxVersion() { return m_simonMaxVersion; }
	QString authors() { return m_authors; }*/

	foreach (const QString& id, scenarioIds) {
		Scenario *s = new Scenario(id);
		kDebug() << "Initializing scenario" << id;
		if (!s->init()) {
			KMessageBox::information(this, i18n("Could not init scenario \"%1\"", id));
		} else {
			QListWidget *target;
			if (selectedIds.contains(id))
				target = selected;
			else target = available;

			QListWidgetItem *item = new QListWidgetItem(s->icon(), s->name(), target);

			QString tooltip;
			QString licence = s->licence();
			QString minVersion = s->simonMinVersion()->toString();

			QString maxVersion;
			if (s->simonMaxVersion())
				maxVersion = s->simonMaxVersion()->toString();
			else
				maxVersion = "-";

			QString strAuthors;
			QList<Author*>  authors = s->authors();
			foreach (Author* a, authors)
				strAuthors += i18nc("Name and contact information", "<p>%1 (%2)</p>", a->name(), a->contact());

			tooltip = i18nc("Infos about the scenario", "<html><head /><body>"
					"<h4>Licence</h4><p>%1</p>"
					"<h4>Compatibility</h4><p>Minimum version: %2</p><p>Maximum version: %3</p>"
					"<h4>Authors</h4><p>%4</p>", licence, minVersion, maxVersion, strAuthors);

			item->setToolTip(tooltip);
			item->setData(Qt::UserRole, id);
		}
	}
}

int ScenarioManagementDialog::exec()
{
	int ret = KDialog::exec();
	if (ret) {
		QListWidget *s = ui.asScenarios->selectedListWidget();
		QStringList ids;

		for (int i=0; i < s->count(); i++)
			ids << s->item(i)->data(Qt::UserRole).toString();

		KSharedConfigPtr config = KSharedConfig::openConfig("simonscenariosrc");
		KConfigGroup cg(config, "");
		cg.writeEntry("SelectedScenarios", ids);
	}

	return ret;
}


ScenarioManagementDialog::~ScenarioManagementDialog()
{
}

