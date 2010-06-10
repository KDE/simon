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
#include "trainingswizard.h"
#include "trainsamplepage.h"
#include "trainsampleintropage.h"
#include "trainsamplevolumepage.h"

#include "../AddWord/addwordview.h"

#include <simonscenarios/trainingmanager.h>
#include <simonscenarios/scenariomanager.h>
#include <simonscenarios/scenario.h>
#include <simonscenarios/trainingtext.h>
#include <simonsound/soundserver.h>

#include <QWizardPage>
#include <QStringList>

#include <QVBoxLayout>
#include <QLabel>
#include <QTimer>
#include <QCheckBox>

#include <KLocalizedString>
#include <KMessageBox>
#include <KStandardDirs>

#include <math.h>

TrainingsWizard::TrainingsWizard(QWidget *parent) : SimonWizard(parent)
{
	addPage(createIntroPage());

	if (SoundServer::getCalibrateVolume())
		addPage(createVolumePage());

	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	setBanner("training");
}

bool TrainingsWizard::init(const QStringList& prompts)
{
	return init(prompts, i18n("Training"));
}

/**
 * \param smartSentences If this is true, the trainingswizard will try to ask the user for 
 * 		meaningful sentences utilizing the individual words
 */
bool TrainingsWizard::init(const QList<Word*>& wList, bool smartSentences) 
{
	QStringList pages;

	if (smartSentences) {
		//resolve sentences
		foreach (Word *w, wList) {
			QStringList examples = ScenarioManager::getInstance()->
				getCurrentScenario()->getExampleSentences(w->getWord(), w->getTerminal(), 1);
			if (examples.isEmpty())
				pages << w->getWord();
			else pages << examples[0];
		}
	} else {
		//we try to guess the perfect amount of words/page
		//We first go through the possible words/page word counts from 5 to 12
		//If we find a perfect match (means we have x pages with the /same/ amount of words
		//on them, we found a perfect words/page value
		//If not, we try to find the w/p count that leaves the last page with the closest
		//value of w/p than the others.
		//for example if we have given 37 words, we would end up with the following:
		//
		//	+---------+-----------------+
		//	| w/p     |    w/p last page|
		//	+---------------------------+
		//	|  5      |         2	    |
		//	|  6      |         1	    |
		//	|  7      |         2	    |
		//	|  8      |         5	    |
		//	|  9      |         1	    |
		//	|  10     |         7	    |
		//	|  11     |         4	    |
		//	|  12     |         1	    |
		//	+---------+-----------------+
		//
		//In this case the perfect amount of w/p would be 10 because even the last page
		//would have enough words for perfect accuracy

		short wordCount = wList.count();
		short wordsPerPage=5;

		short maxLeftOver=0;
		short leftOverWordsPerPage=5;

		while ( ( wordCount%wordsPerPage != 0 ) && ( wordsPerPage <=12 ) )
		{
			if ( wordCount%wordsPerPage > maxLeftOver )
			{
				maxLeftOver = wordCount%wordsPerPage;
				leftOverWordsPerPage = wordsPerPage;
			}

			wordsPerPage++;
		}
		if ( wordsPerPage==13 ) wordsPerPage=leftOverWordsPerPage;

		QString page;
		QString time;
		for ( int i=0; i< ceil((double)wordCount/wordsPerPage); i++ )
		{
			page="";
			for ( int j=0; ( j<wordsPerPage ) && ( j+ ( i*wordsPerPage ) < wordCount ); j++ )
			{
				page += wList.at(j+(i*wordsPerPage))->getWord() + QString(" ");
			}
			page = page.trimmed();

			pages.append(page);
		}
	}
	
	return init(pages, i18n("Special Training"));
}

bool TrainingsWizard::init(const TrainingText &text)
{
	return init(text.getPages(), text.getName());
}

bool TrainingsWizard::init(const QStringList& prompts, const QString& name)
{
	QStringList missingWords = TrainingManager::getInstance()->missingWords(prompts);
	while (!missingWords.isEmpty())
	{
		if (KMessageBox::questionYesNoCancel(0, i18n("Your vocabulary does not define all words used in this text. These words are "
"missing:\n%1\n\nDo you want to add them now?", missingWords.join(", "))) == KMessageBox::Yes)
		{
			AddWordView *addWord = new AddWordView(0);
			addWord->addWords(missingWords);
			if (!addWord->exec())
				return false;
		} else {
			return false;
		}
		missingWords = TrainingManager::getInstance()->missingWords(prompts);
	}

	setWindowTitle(name);
	
	int nowPage=1;
	int maxPage=prompts.count();
	foreach (const QString& prompt, prompts)
	{
		TrainSamplePage *page = new TrainSamplePage(prompt, nowPage++, maxPage, name, this);
		addPage(page);
	}
	connect(this, SIGNAL(accepted()), this, SLOT(submit()));
	connect(this, SIGNAL(rejected()), this, SLOT(cleanUp()));
	addPage(createFinishedPage());
	return true;
}



QWizardPage* TrainingsWizard::createIntroPage()
{
	return new TrainSampleIntroPage(this);
}


QWizardPage* TrainingsWizard::createVolumePage()
{
	return new TrainSampleVolumePage(this);
}


QWizardPage* TrainingsWizard::createFinishedPage()
{
	QWizardPage *finished = new QWizardPage(this);
	finished->setTitle(i18n("Training completed"));
	QLabel *lbFinished = new QLabel(finished);
	lbFinished->setText(i18n("The training has now been completed. The changes are being applied in the "
"background."));
	lbFinished->setWordWrap(true);
	QVBoxLayout *lay = new QVBoxLayout(finished);
	lay->addWidget(lbFinished);
	finished->setLayout(lay);
	return finished;
}

void TrainingsWizard::submit()
{
	int i=0;
	forever
	{
		QWizardPage *plainPage = page(i++);
		if (!plainPage) break;
		
		TrainSamplePage *page = dynamic_cast<TrainSamplePage*>(plainPage);
		if (!page) 
			continue;
		
		page->submit();
	}
	if (!TrainingManager::getInstance()->savePrompts())
	{
		KMessageBox::error(this, i18n("Couldn't save changes to the trainings corpus.\n\nPlease restart simon."));
	}
}

void TrainingsWizard::cleanUp()
{
	//clean up the samples
}

TrainingsWizard::~TrainingsWizard()
{

}
