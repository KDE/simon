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


#ifndef SPEECHMODELSETTINGS_H
#define SPEECHMODELSETTINGS_H


#include "ui_grammarsettingsdlg.h"
#include "ui_trainingsettings.h"
#include <KCModule>
#include <QVariantList>

/**
	@author Peter Grasch <bedahr@gmx.net>
*/

class SpeechModelSettings : public KCModule
{
Q_OBJECT
private:
	Ui::GrammarSettingsDlg uiGrammar;
	Ui::TrainingSettingsWidget uiTrainingsData;
	

signals:
	void changed();

private slots:
	void showRenameWizard();
	void showImportWizard();
	void showMergeWizard();
	void askToSaveGrammar();

public slots:
	void mergeGrammar(QStringList);

public:
    SpeechModelSettings(QWidget* parent, const QVariantList& args=QVariantList());

    ~SpeechModelSettings();

};


#endif