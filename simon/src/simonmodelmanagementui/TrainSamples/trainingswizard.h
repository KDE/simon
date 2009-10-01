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

#ifndef SIMON_TRAININGSWIZARD_H_81DBD07A9C9E4223823558E55A8BAF3E
#define SIMON_TRAININGSWIZARD_H_81DBD07A9C9E4223823558E55A8BAF3E

#include <QWizard>
#include <speechmodelbase/word.h>

class QWizard;
class QStringList;
class TrainingText;

class TrainingsWizard : public QWizard
{
	Q_OBJECT

	private slots:
		void cleanUp();
		void submit();

	private:
		QWizardPage* createIntroPage();
		QWizardPage* createFinishedPage();

		bool init(const QStringList& prompts, const QString& name);

	public:
		TrainingsWizard(QWidget *parent=0);
		bool init(const QStringList &prompts);
		bool init(const WordList &wList);
		bool init(const TrainingText &text);
		~TrainingsWizard();

};



#endif
