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

#ifndef SIMON_PRONUNCIATIONTRAININGCONFIGURATION_H_DF2B936F15A84FD8962EF68E1DAFE24F
#define SIMON_PRONUNCIATIONTRAININGCONFIGURATION_H_DF2B936F15A84FD8962EF68E1DAFE24F 

#include <commandpluginbase/commandconfiguration.h>
#include "ui_pronunciationtrainingconfigurationdlg.h"
#include <KSharedConfig>
#include <QPointer>

class PronunciationTrainingConfiguration : public CommandConfiguration
{
	Q_OBJECT
	
	private:
		Ui::PronunciationTrainingConfigurationDlg ui;
		static QPointer<PronunciationTrainingConfiguration> instance;
 
	public slots:
		virtual void save();
		virtual void load();
		virtual void defaults();
	
	public:
		static PronunciationTrainingConfiguration *getInstance(QWidget *parent=0, const QVariantList &args = QVariantList()) {
			if (!instance) instance = new PronunciationTrainingConfiguration(parent, args);
			return instance;
		}
		PronunciationTrainingConfiguration(QWidget *parent=0, const QVariantList &args = QVariantList());
		~PronunciationTrainingConfiguration();
		
		void destroy();
		
		//configuration options
		QString trigger();
		QString terminal();
};

#endif
