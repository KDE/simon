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

#include "modelsettings.h"
#include "speechmodelmanagementuiconfiguration.h"
#include <simonscenarios/scenariomanager.h>
#include <QFile>
#include <QDateTime>
#include <QFileInfo>
#include <KGlobal>
#include <KConfig>
#include <KFileDialog>
#include <KMessageBox>


ModelSettings::ModelSettings(QWidget* parent, const QVariantList &args) : KCModule(KGlobal::mainComponent(), parent, args),
	m_hmmDefsToImport(QString()),
	m_tiedlistToImport(QString())
{
	Q_UNUSED(args);

	ui.setupUi(this);

	addConfig(SpeechModelManagementUiConfiguration::self(), this);

	connect(ui.rbStatic, SIGNAL(toggled(bool)), this, SLOT(slotChanged()));
	connect(ui.rbBaseModel, SIGNAL(toggled(bool)), this, SLOT(slotChanged()));
	connect(ui.rbDynamic, SIGNAL(toggled(bool)), this, SLOT(slotChanged()));

	ui.pbLoadBaseHMM->setIcon(KIcon("document-open"));
	ui.pbLoadBaseTiedlist->setIcon(KIcon("document-open"));

	connect(ui.pbLoadBaseHMM, SIGNAL(clicked()), this, SLOT(loadBaseHMM()));
	connect(ui.pbLoadBaseTiedlist, SIGNAL(clicked()), this, SLOT(loadBaseTiedlist()));
}

void ModelSettings::slotChanged()
{
	emit changed(true);
}

void ModelSettings::load()
{
	switch (ScenarioManager::getInstance()->baseModelType())
	{
		case 0: ui.rbStatic->animateClick();
			break;
		case 1: ui.rbBaseModel->animateClick();
			break;
		case 2: ui.rbDynamic->animateClick();
			break;
		default:
			kDebug() << "Unknown model type: " << ScenarioManager::getInstance()->baseModelType();
	}

	m_storedModelType = ScenarioManager::getInstance()->baseModelType();

	ui.lbLastLoadedBaseHMM->setText(ScenarioManager::getInstance()->baseModelHMMName());
	ui.lbLastLoadedBaseTiedlist->setText(ScenarioManager::getInstance()->baseModelTiedlistName());
	KCModule::load();
}

void ModelSettings::save()
{
	int modelType = 0;
	if (ui.rbBaseModel->isChecked())
		modelType = 1;
	else if (ui.rbDynamic->isChecked()) 
		modelType = 2;

	if (!m_hmmDefsToImport.isEmpty()) {
		QString targetPathHMM = KStandardDirs::locateLocal("appdata", "model/basehmmdefs");
		if (QFile::exists(targetPathHMM) && !QFile::remove(targetPathHMM)) {
			KMessageBox::sorry(this, i18n("Couldn't remove current base model"));
			return;
		}

		if (!QFile::copy(m_hmmDefsToImport, targetPathHMM))
			KMessageBox::sorry(this, i18n("Couldn't import hmm definitions."));

		m_hmmDefsToImport = QString();
	}

	if (!m_tiedlistToImport.isEmpty()) {
		QString targetPathTiedlist = KStandardDirs::locateLocal("appdata", "model/basetiedlist");
		if (QFile::exists(targetPathTiedlist) && !QFile::remove(targetPathTiedlist)) {
			KMessageBox::sorry(this, i18n("Couldn't remove current base model"));
			return;
		}

		if (!QFile::copy(m_tiedlistToImport, targetPathTiedlist))
			KMessageBox::sorry(this, i18n("Couldn't import tiedlist."));
		m_tiedlistToImport = QString();
	}

	ScenarioManager::getInstance()->setBaseModel(modelType, ui.lbLastLoadedBaseHMM->text(), ui.lbLastLoadedBaseTiedlist->text());
	KCModule::save();
}

void ModelSettings::defaults()
{
	ScenarioManager::getInstance()->setBaseModel(2, i18n("None"), i18n("None"));

	QFile::remove(KStandardDirs::locateLocal("appdata", "model/basehmmdefs"));
	QFile::remove(KStandardDirs::locateLocal("appdata", "model/basetiedlist"));

	KCModule::defaults();

	load();
}

void ModelSettings::touchModelSrcRc()
{
	KConfig config( KStandardDirs::locateLocal("appdata", "model/modelsrcrc"), KConfig::SimpleConfig );
	KConfigGroup cGroup(&config, "");
	cGroup.writeEntry("BaseModelDate", QDateTime::currentDateTime());
	config.sync();
}

void ModelSettings::loadBaseHMM()
{
	QString path = KFileDialog::getOpenFileName(KUrl(), QString(), this, i18n("Select base hmmdefs"));
	if (path.isEmpty()) return;

	m_hmmDefsToImport = path;

	emit changed(true);
	QFileInfo f(path);
	ui.lbLastLoadedBaseHMM->setText(f.fileName());
}

void ModelSettings::loadBaseTiedlist()
{
	QString path = KFileDialog::getOpenFileName(KUrl(), QString(), this, i18n("Select base tiedlist"));
	if (path.isEmpty()) return;

	m_tiedlistToImport = path;
	
	emit changed(true);

	QFileInfo f(path);
	ui.lbLastLoadedBaseTiedlist->setText(f.fileName());
}


ModelSettings::~ModelSettings()
{
}

