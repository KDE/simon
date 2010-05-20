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

#ifndef SIMON_TRAININGSWIZARD_H_6F6A97AD216E4EABBBE96AC5142A709B
#define SIMON_TRAININGSWIZARD_H_6F6A97AD216E4EABBBE96AC5142A709B

#include <simonuicomponents/simonwizard.h>

class QWizard;
class QStringList;

class TrainingsWizard : public SimonWizard
{
	Q_OBJECT

	private slots:
		void submit();

	public:
		TrainingsWizard(QWidget *parent=0);
		//bool init(const TrainingText &text);
		~TrainingsWizard();

		enum TrainingsType {
			Repeating=1,
			Training=2,
			Interview=3
		};

		int collectSamples(TrainingsType type, qint32 userId);

	private:
		QWizardPage* createIntroPage();
		QWizardPage* createFinishedPage();
		bool init(qint32 userId, TrainingsType type, const QStringList& prompts, const QString& name);

		QStringList repeatPrompts();
		QStringList trainingPrompts();
		QStringList interviewQuestions();

		QStringList parsePromptsFromFile(const QString& path);
		bool cleanUp();



};



#endif
