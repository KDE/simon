/* *   Copyright (C) 2008 Peter Grasch <grasch@simon-listens.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public Licence version 2,
 *   or (at your option) any later version, as published by the Free
 *   Software Foundation
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public Licence for more details
 *
 *   You should have received a copy of the GNU General Public
 *   Licence along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "newscenario.h"
#include "newauthor.h"
#include "../../version.h"

#include <simonscenarios/scenario.h>
#include <simonscenarios/author.h>
#include <simonscenariobase/versionnumber.h>

#include <QDateTime>

#include <KUrl>
#include <KMessageBox>
#include <KDialogButtonBox>

NewScenario::NewScenario(QWidget* parent) : KDialog(parent)
{
	QWidget *widget = new QWidget( this );
	ui.setupUi(widget);

	setMainWidget( widget );
	setCaption( i18n("Scenario") );
	
	connect(ui.leName, SIGNAL(textChanged(const QString&)), this, SLOT(setWindowTitleToScenarioName(QString)));
	connect(ui.leName, SIGNAL(textChanged(const QString&)), this, SLOT(checkIfComplete()));
	connect(ui.leMinVersion, SIGNAL(textChanged(const QString&)), this, SLOT(checkIfComplete()));
	connect(ui.cbLicence, SIGNAL(editTextChanged(const QString&)), this, SLOT(checkIfComplete()));

	connect(ui.pbAddAuthor, SIGNAL(clicked()), this, SLOT(addAuthor()));
	connect(ui.pbRemoveAuthor, SIGNAL(clicked()), this, SLOT(removeAuthor()));
	
	checkIfComplete();
}

void NewScenario::checkIfComplete()
{
	bool complete  = ! ui.leName->text().isEmpty() &&
			 ! ui.cbLicence->currentText().isEmpty() &&
			 ! ui.leMinVersion->text().isEmpty() &&
			 ! m_authors.isEmpty();

	enableButtonOk(complete);
}

QString NewScenario::createId()
{
	QString id = ui.leName->text();
	id.replace(" ", "_").replace("/","_").remove("?").replace("\\", "_").remove("<").remove(">").remove("|").remove("\"");
	return id+"-"+QDateTime::currentDateTime().toString("dd.MM.dd.yyyy-hh:mm:ss:zzz");
}

void NewScenario::setWindowTitleToScenarioName(QString name)
{
	if (!name.isEmpty())
		setCaption(i18n("Scenario: %1", name));
	else setCaption(i18n("Scenario"));
}

void NewScenario::addAuthor()
{
	NewAuthor *newAuthor = new NewAuthor(this);
	Author *a = newAuthor->newAuthor();
	delete newAuthor;

	if (!a) return;

	m_authors << a;
	updateAuthorDisplay();
}

void NewScenario::removeAuthor()
{
	int index = ui.lwAuthors->currentRow();
	if (index == -1) return;

	Q_ASSERT(index >= m_authors.count());

	m_authors.removeAt(index);

	updateAuthorDisplay();
}

void NewScenario::updateAuthorDisplay()
{
	ui.lwAuthors->clear();

	foreach (Author *a, m_authors) {
		ui.lwAuthors->addItem(i18nc("Author (contact information)", "%1 (%2)", a->name(), a->contact()));
	}
	checkIfComplete();
}

int NewScenario::exec()
{
	return KDialog::exec();
}

bool NewScenario::newScenario()
{
	bool success = true;
	m_authors.clear();
	ui.leMinVersion->setText(simon_version);
	if (exec())
	{
		//creating
		Scenario *s = new Scenario(createId());
		VersionNumber *minVersion = new VersionNumber(NULL, ui.leMinVersion->text());
		VersionNumber *maxVersion = new VersionNumber(NULL, ui.leMaxVersion->text());
		if (!s->create(ui.leName->text(), ui.pbIcon->icon(), minVersion, maxVersion, ui.cbLicence->currentText(), m_authors)) {
			KMessageBox::sorry(this, i18n("Scenario could not be created"));
			success = false;
		} else if (!s->save()) {
			KMessageBox::sorry(this, i18n("New scenario could not be saved"));
			success = false;
		}
		delete s;
	} else
		qDeleteAll(m_authors);
	m_authors.clear();
	return success;
}

void NewScenario::displayScenario(Scenario *s)
{
	ui.leName->setText(s->name());
	ui.leMinVersion->setText(s->simonMinVersion()->toString());
	VersionNumber *max = s->simonMaxVersion();
	QString maxStrVersion = "";
	if (max)
		maxStrVersion = max->toString();
	ui.leMaxVersion->setText(maxStrVersion);
	ui.cbLicence->setEditText(s->licence());

	ui.pbIcon->setIcon(s->icon());

	m_authors = s->authors();
	updateAuthorDisplay();
}

bool NewScenario::editScenario(Scenario *s)
{
	if (!s) return false;

	bool success = true;
	displayScenario(s);
	if (exec())
	{
		//TODO: updating
		if (!s->save()) {
			KMessageBox::sorry(this, i18n("New scenario could not be saved"));
			success = false;
		}
	}
	m_authors.clear();
	return success;
}

NewScenario::~NewScenario()
{
}
