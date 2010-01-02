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


#ifndef SIMON_IMPORTTRAININGTEXTWORKINGPAGE_H_7BF1491864534D0290E547D5AAFB0747
#define SIMON_IMPORTTRAININGTEXTWORKINGPAGE_H_7BF1491864534D0290E547D5AAFB0747

#include <QWizardPage>
#include "ui_importtrainingtextworkingpage.h"

/**
 * \class ImportTrainingTextWorkingPage
 * \brief Imports the given trainingstext
 * \author Peter Grasch
 * \version 0.1
 */
class ImportTrainingTextWorkingPage : public QWizardPage {
	Q_OBJECT 
	private:
		Ui::ImportTextWorkingPage ui;
	
	private slots:
		void start();

	public slots:
		void startImport(KUrl path);
		void parseFile(QString path);
		void parseAdd();
		QStringList parse(QIODevice *input, const QString& encoding);
		void createTrainingsText(const QString& name, const QStringList& sentences);

		void initializePage();
	public:
		ImportTrainingTextWorkingPage(QWidget* parent);
};

#endif
