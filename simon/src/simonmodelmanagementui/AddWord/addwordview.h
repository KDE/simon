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


/**
 *	@class AddWordView
 *	@brief The wizard to add a new word to the Model
 *
 *	This wizard is using a step-by-step guide through the procedure
 *	to add a new word to the Language Model.
 *
 *	@version 0.1
 *	@date 08.01.2006
 *	@author Peter Grasch
*/


#ifndef ADDWORDVIEW_H
#define ADDWORDVIEW_H

#include <QWizard>
#include <QStringList>
#include <speechmodelbase/word.h>
#include "../simonmodelmanagementui_export.h"

class QWizardPage;
class AddWordResolvePage;
class GrammarManager;
class AddWordIntroPage;
class WordListManager;
class TrainingManager;

class SIMONMODELMANAGEMENTUI_EXPORT AddWordView : public QWizard
{
	Q_OBJECT

	private:
		static AddWordView *instance;
		
		QString recordingName1, recordingName2;
		QStringList wordsToAdd;
		int prevId;
		AddWordResolvePage *resolvePage;
		AddWordIntroPage *welcomePage;
		WordList *listToAdd;
		QHash<QString,QString> *promptsToAdd;
		
		void hideEvent(QHideEvent *event) { 
			emit hidden(); return QWidget::hideEvent(event); }

		void commitList();
	
	signals:
		void addedWord();
		void hidden();
		
	private slots:
		void setRecordingNames(QString name1, QString name2);
		void askToAddWord(QString word);
		void askToAddWords(QStringList words);
		void cleanUp();

	public slots:
		
		void finish(int done);

		AddWordIntroPage* createWelcomePage();
		QWizardPage* createRecordPage();
		AddWordResolvePage* createResolvePage();
		QWizardPage* createFinishedPage();
		
	protected:
		AddWordView(QWidget *parent);

	public:
		static AddWordView* getInstance();
		~AddWordView();
    
        void createWord(QString word);
};

#endif
