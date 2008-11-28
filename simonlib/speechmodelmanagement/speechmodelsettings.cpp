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


#include "speechmodelsettings.h"
#include "speechmodelmanagementconfiguration.h"
#include <KMessageBox>
#include <KGlobal>
#include <KPageWidget>
#include "grammarmanager.h"
#include <kgenericfactory.h>

K_PLUGIN_FACTORY( SpeechModelSettingsFactory, 
			registerPlugin< SpeechModelSettings >(); 
		)
        
K_EXPORT_PLUGIN( SpeechModelSettingsFactory("simonlib") )

SpeechModelSettings::SpeechModelSettings(QWidget* parent, const QVariantList& args): KCModule(KGlobal::mainComponent(), parent)
{
	QVBoxLayout *lay = new QVBoxLayout(this);
	KPageWidget *pageWidget = new KPageWidget(this);
	lay->addWidget(pageWidget);
	
	if (args.count() == 1)
		pageWidget->setFaceType(KPageView::Tabbed);
	
	QWidget *trainingsDataWidget = new QWidget(this);
	uiTrainingsData.setupUi(trainingsDataWidget);
	
// 	KPageWidgetItem *grammar = pageWidget->addPage(grammarWidget, i18n("Grammatik"));
	KPageWidgetItem *trainingsData = pageWidget->addPage(trainingsDataWidget, i18n("Trainingsdata"));

// 	grammar->setIcon(KIcon("user-properties"));
	trainingsData->setIcon(KIcon("view-pim-news"));

// 	grammar->setHeader("");
	trainingsData->setHeader("");
	
	addConfig(SpeechModelManagementConfiguration::self(), this);
}

SpeechModelSettings::~SpeechModelSettings()
{
}
