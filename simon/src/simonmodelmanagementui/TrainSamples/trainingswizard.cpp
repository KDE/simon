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
#include <speechmodelmanagement/trainingmanager.h>
#include <speechmodelbase/trainingtext.h>

#include <QWizardPage>
#include <QStringList>

#include <QVBoxLayout>
#include <QLabel>

#include <KLocalizedString>
#include <KMessageBox>
#include <KStandardDirs>

#include <math.h>

TrainingsWizard::TrainingsWizard(const QStringList &prompts, QWidget *parent) : QWizard(parent)
{
	init(prompts);
}

TrainingsWizard::TrainingsWizard(const WordList &wList, QWidget *parent) : QWizard(parent)
{
	QStringList pages;

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
			page += wList.at(j+(i*wordsPerPage)).getWord() + QString(" ");
		}
		page = page.trimmed();

		pages.append(page);
	}
	
	init(pages, i18n("Spezialtraining"));
}

TrainingsWizard::TrainingsWizard(const TrainingText &text, QWidget *parent) : QWizard(parent)
{
	init(text.getPages(), text.getName());
}

void TrainingsWizard::init(const QStringList& prompts, const QString& name)
{
	QString textName;
	if (name.isEmpty())
		textName = i18n("Training");
	else textName = name;
	
	setWindowTitle(textName);
	setPixmap(QWizard::WatermarkPixmap, QPixmap(KStandardDirs::locate("appdata", "themes/default/training.png")));
	
	int nowPage=1;
	int maxPage=prompts.count();
	foreach (const QString& prompt, prompts)
	{
		TrainSamplePage *page = new TrainSamplePage(prompt, nowPage++, maxPage, textName, this);
// 		connect(this, SIGNAL(accepted()), page, SLOT(submit()));
// 		connect(this, SIGNAL(rejected()), page, SLOT(cleanUp()));
		addPage(page);
	}
	connect(this, SIGNAL(accepted()), this, SLOT(submit()));
	connect(this, SIGNAL(rejected()), this, SLOT(cleanUp()));
	
}

QWizardPage* TrainingsWizard::createIntroPage()
{
	QWizardPage *intro = new QWizardPage(this);
	intro->setTitle(i18n("Training"));
	QLabel *lbIntro = new QLabel(intro);
	lbIntro->setWordWrap(true);
	lbIntro->setText(i18n("Dieser Assistent wird Ihnen dabei helfen, die Erkennungsrate Anhand von Trainings-Aufnahmen zu erhöhen.."));

	QVBoxLayout *lay = new QVBoxLayout(intro);
	lay->addWidget(lbIntro);
	intro->setLayout(lay);
	return intro;
}


QWizardPage* TrainingsWizard::createFinishedPage()
{
	QWizardPage *finished = new QWizardPage(this);
	finished->setTitle(i18n("Training abgeschlossen"));
	QLabel *lbFinished = new QLabel(finished);
	lbFinished->setText(i18n("Das Training wurde hiermit abgeschlossen.\n\nDie Änderungen werden im Hintergrund übernommen."));
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
		KMessageBox::error(this, i18n("Konnte Änderung der Trainingsdaten nicht speichern.\n\nBitte starten Sie simon neu."));
	}
}

void TrainingsWizard::cleanUp()
{
	//clean up the samples
}

TrainingsWizard::~TrainingsWizard()
{

}
