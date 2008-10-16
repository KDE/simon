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


#ifndef IMPORTTRAININGTEXTREMOTEPAGE_H
#define IMPORTTRAININGTEXTREMOTEPAGE_H

#include <QWizardPage>
#include "ui_importtrainingtextremotepage.h"

class QuickDownloader;
/**
 * \class ImportTrainingTextRemotePage
 * \author Peter Grasch
 * \version 0.1
 * \date 10.8.2007
 * \brief Displays the remote-importPage for the Trainingtexts
 */
class ImportTrainingTextRemotePage : public QWizardPage {
	Q_OBJECT
	private:
		QuickDownloader *downloader;
		Ui::ImportRemotePage ui;
	private slots:
		void importList(const QString& path);
		
	public:
		void initializePage();
		ImportTrainingTextRemotePage(QWidget* parent);
		int nextId() const {
			return 4;
		}
        ~ImportTrainingTextRemotePage();
};

#endif
