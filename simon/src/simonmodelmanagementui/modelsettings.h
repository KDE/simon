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


#ifndef SIMON_MODELSETTINGS_H_69CD51EDBC2948939B9B8D4BD76FDAF7
#define SIMON_MODELSETTINGS_H_69CD51EDBC2948939B9B8D4BD76FDAF7

#include "ui_modelsettingsdlg.h"
#include <KCModule>
#include <QVariantList>
#include <QString>
/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class ModelSettings : public KCModule
{
Q_OBJECT
private:
	Ui::ModelDlg ui;
	QString m_hmmDefsToImport;
	QString m_tiedlistToImport;

	int m_storedModelType;
	void touchModelSrcRc();

private slots:
	void slotChanged();
	void loadBaseHMM();
	void loadBaseTiedlist();

public slots:
	void load();
	void save();
	void defaults();

public:
    ModelSettings(QWidget* parent, const QVariantList &args=QVariantList());

    ~ModelSettings();

};

#endif
