/*
 *  Copyright (C) 2009 Peter Grasch <grasch@simon-listens.org>
 *  
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2,
 *  or (at your option) any later version, as published by the Free
 *  Software Foundation
 *  
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details
 * 
 *  You should have received a copy of the GNU General Public
 *  License along with this program; if not, write to the
 *  Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */


#include "samview.h"
#include "accuracydisplay.h"
#include <speechmodelcompilation/modelcompilationmanager.h>
#include <simonmodeltest/modeltest.h>
#include <QHash>
#include <KStandardAction>
#include <KActionCollection>
#include <KAction>
#include <KIcon>
#include <KStandardDirs>
#include <KConfig>
#include <KConfigGroup>
#include <KMessageBox>


SamView::SamView(QWidget *parent, Qt::WFlags flags) : KXmlGuiWindow(parent, flags)
{
	ui.setupUi(this);
	ui.saWordResultDetails->setWidget(ui.wgWordResultDetails);
	ui.saSentenceResultDetails->setWidget(ui.wgSentenceResultDetails);

	KAction* getPathsFromSimon = new KAction(this);
	getPathsFromSimon->setText(i18n("Modify simons model"));
	getPathsFromSimon->setIcon(KIcon("simon"));
	actionCollection()->addAction("getPathsFromSimon", getPathsFromSimon);
	connect(getPathsFromSimon, SIGNAL(triggered(bool)),
		this, SLOT(getBuildPathsFromSimon()));
	
	KAction* recompile = new KAction(this);
	recompile->setText(i18n("Build model"));
	recompile->setIcon(KIcon("view-refresh"));
	recompile->setShortcut(Qt::CTRL + Qt::Key_F5);
	actionCollection()->addAction("compileModel", recompile);
	connect(recompile, SIGNAL(triggered(bool)),
		this, SLOT(compileModel()));
	
	KAction* test= new KAction(this);
	test->setText(i18n("Test model"));
	test->setIcon(KIcon("chronometer"));
	actionCollection()->addAction("testModel", test);
	connect(test, SIGNAL(triggered(bool)),
		this, SLOT(testModel()));
	
	KAction* testResults= new KAction(this);
	testResults->setText(i18n("Test results"));
	testResults->setIcon(KIcon("view-pim-tasks"));
	actionCollection()->addAction("testResults", testResults);
	connect(testResults, SIGNAL(triggered(bool)),
		this, SLOT(switchToTestResults()));



	connect(ui.pbCompileModel, SIGNAL(clicked()), this, SLOT(compileModel()));
	connect(ui.pbTestModel, SIGNAL(clicked()), this, SLOT(testModel()));

	KStandardAction::quit(this, SLOT(close()),
			      actionCollection());

	setupGUI();

	ui.urHmmDefs->setMode(KFile::File|KFile::ExistingOnly|KFile::LocalOnly);
	ui.urTiedlist->setMode(KFile::File|KFile::ExistingOnly|KFile::LocalOnly);
	ui.urDict->setMode(KFile::File|KFile::ExistingOnly|KFile::LocalOnly);
	ui.urDFA->setMode(KFile::File|KFile::ExistingOnly|KFile::LocalOnly);
	ui.urPromptsBasePath->setMode(KFile::Directory|KFile::ExistingOnly|KFile::LocalOnly);
	ui.urLexicon->setMode(KFile::File|KFile::ExistingOnly|KFile::LocalOnly);
	ui.urGrammar->setMode(KFile::File|KFile::ExistingOnly|KFile::LocalOnly);
	ui.urVocabulary->setMode(KFile::File|KFile::ExistingOnly|KFile::LocalOnly);
	ui.urPrompts->setMode(KFile::File|KFile::ExistingOnly|KFile::LocalOnly);
	ui.urTestPromptsBasePath->setMode(KFile::Directory|KFile::ExistingOnly|KFile::LocalOnly);
	ui.urTestPrompts->setMode(KFile::File|KFile::ExistingOnly|KFile::LocalOnly);
	ui.urTreeHed->setMode(KFile::File|KFile::ExistingOnly|KFile::LocalOnly);
	ui.urWavConfig->setMode(KFile::File|KFile::ExistingOnly|KFile::LocalOnly);

	getBuildPathsFromSimon();

	modelCompilationManager = new ModelCompilationManager("internalsamuser", this);
	connect(modelCompilationManager, SIGNAL(modelCompiled()), this, SLOT(retrieveCompleteBuildLog()));
	connect(modelCompilationManager, SIGNAL(activeModelCompilationAborted()), this, SLOT(retrieveCompleteBuildLog()));
	connect(modelCompilationManager, SIGNAL(status(const QString&, int, int)), this, SLOT(slotModelCompilationStatus(const QString&, int, int)));
	connect(modelCompilationManager, SIGNAL(error(const QString&)), this, SLOT(slotModelCompilationError(const QString&)));

	connect(modelCompilationManager, SIGNAL(classUndefined(const QString&)), this, 
			SLOT(slotModelCompilationClassUndefined(const QString&)));
	connect(modelCompilationManager, SIGNAL(wordUndefined(const QString&)), this, 
			SLOT(slotModelCompilationWordUndefined(const QString&)));
	connect(modelCompilationManager, SIGNAL(phonemeUndefined(const QString&)), this, 
			SLOT(slotModelCompilationPhonemeUndefined(const QString&)));
	
	modelTest = new ModelTest("internalsamuser", this);
	connect(modelTest, SIGNAL(testComplete()), this, SLOT(retrieveCompleteTestLog()));
	connect(modelTest, SIGNAL(testComplete()), this, SLOT(analyzeTestOutput()));
	connect(modelTest, SIGNAL(testAborted()), this, SLOT(retrieveCompleteTestLog()));
	connect(modelTest, SIGNAL(status(const QString&, int, int)), this, SLOT(slotModelTestStatus(const QString&, int, int)));
	connect(modelTest, SIGNAL(recognitionInfo(const QString&)), this, SLOT(slotModelTestRecognitionInfo(const QString&)));
	connect(modelTest, SIGNAL(error(const QString&)), this, SLOT(slotModelTestError(const QString&)));
	connect(modelTest, SIGNAL(testComplete()), this, SLOT(switchToTestResults()));
}

void SamView::getBuildPathsFromSimon()
{
	ui.urHmmDefs->setUrl(KUrl(KStandardDirs::locate("data", "simon/model/hmmdefs")));
	ui.urTiedlist->setUrl(KUrl(KStandardDirs::locate("data", "simon/model/tiedlist")));
	ui.urDict->setUrl(KUrl(KStandardDirs::locate("data", "simon/model/model.dict")));
	ui.urDFA->setUrl(KUrl(KStandardDirs::locate("data", "simon/model/model.dfa")));
	ui.urPromptsBasePath->setUrl(KUrl(KStandardDirs::locate("data", "simon/model/training.data/")));
	ui.urTestPromptsBasePath->setUrl(KUrl(KStandardDirs::locate("data", "simon/model/training.data/")));
	ui.urLexicon->setUrl(KUrl(KStandardDirs::locate("data", "simon/model/lexicon")));
	ui.urGrammar->setUrl(KUrl(KStandardDirs::locate("data", "simon/model/model.grammar")));
	ui.urVocabulary->setUrl(KUrl(KStandardDirs::locate("data", "simon/model/model.voca")));


	QString promptsPath = KStandardDirs::locate("data", "simon/model/prompts");
	QString testPromptsPath = KStandardDirs::locate("data", "simon/model/prompts_test");
	if (!QFile::exists(testPromptsPath))
		testPromptsPath = promptsPath;

	ui.urPrompts->setUrl(KUrl(promptsPath));
	ui.urTestPrompts->setUrl(KUrl(testPromptsPath));

	ui.urTreeHed->setUrl(KUrl(KStandardDirs::locate("data", "simon/model/tree1.hed")));
	ui.urWavConfig->setUrl(KUrl(KStandardDirs::locate("data", "simon/model/wav_config")));

	KSharedConfig::Ptr config = KSharedConfig::openConfig("speechmodelmanagementrc");
	KConfigGroup group(config, "Model");
	int sampleRate = group.readEntry("ModelSampleRate", "16000").toInt();

	ui.sbSampleRate->setValue(sampleRate);
	ui.urJConf->setUrl(KUrl(KStandardDirs::locate("data", "simon/model/julius.jconf")));
}

void SamView::compileModel()
{
	ui.twMain->setCurrentIndex(1);

	ui.teBuildLog->clear();
	
	modelCompilationManager->startCompilation(ui.urHmmDefs->url().path(),
			ui.urTiedlist->url().path(),
			ui.urDict->url().path(),
			ui.urDFA->url().path(),
			ui.urPromptsBasePath->url().path(),
			ui.urLexicon->url().path(),
			ui.urGrammar->url().path(),
			ui.urVocabulary->url().path(),
			ui.urPrompts->url().path(),
			ui.urTreeHed->url().path(),
			ui.urWavConfig->url().path()
			);

}

void SamView::slotModelCompilationStatus(const QString& status, int now, int max)
{
	ui.pbBuildProgress->setMaximum(max);
	ui.pbBuildProgress->setValue(now);

	ui.teBuildLog->append(status);
}

void SamView::slotModelCompilationError(const QString& error)
{
	KMessageBox::error(this, error);
	retrieveCompleteBuildLog();
}

void SamView::slotModelCompilationClassUndefined(const QString& undefinedClass)
{
	KMessageBox::error(this, i18n("Grammar class undefined: %1", undefinedClass));
}

void SamView::slotModelCompilationWordUndefined(const QString& undefinedWord)
{
	KMessageBox::error(this, i18n("Word undefined: %1", undefinedWord));

}

void SamView::slotModelCompilationPhonemeUndefined(const QString& undefinedPhoneme)
{
	KMessageBox::error(this, i18n("Phoneme undefined: %1", undefinedPhoneme));

}

void SamView::retrieveCompleteBuildLog()
{
	ui.teBuildLog->clear();
	ui.teBuildLog->append(modelCompilationManager->getGraphicBuildLog());
}

void SamView::retrieveCompleteTestLog()
{
	ui.teTestLog->clear();
	ui.teTestLog->append(modelTest->getGraphicLog());
}

void SamView::slotModelTestStatus(const QString& status, int now, int max)
{
	ui.pbTestProgress->setMaximum(max);
	ui.pbTestProgress->setValue(now);

	ui.teTestLog->append(status);
}

void SamView::slotModelTestRecognitionInfo(const QString& status)
{
	ui.teTestLog->append(status);
}

void SamView::slotModelTestError(const QString& error)
{
	KMessageBox::error(this, error);
	retrieveCompleteBuildLog();
}


void SamView::switchToTestResults()
{
	ui.twMain->setCurrentIndex(3);
}


void SamView::testModel()
{
	ui.twMain->setCurrentIndex(2);

	ui.teTestLog->clear();

	modelTest->startTest(ui.urHmmDefs->url().path(),
			ui.urTiedlist->url().path(),
			ui.urDict->url().path(),
			ui.urDFA->url().path(),
			ui.urTestPromptsBasePath->url().path(),
			ui.urTestPrompts->url().path(),
			ui.sbSampleRate->value(),
			ui.urJConf->url().path()
			);
}

void SamView::analyzeTestOutput()
{
	QHash<QString, FloatList> wordRates = modelTest->getWordRates();
	QHash<QString, FloatList> sentenceRates = modelTest->getSentenceRates();

	QStringList wordList = wordRates.keys();
	qSort(wordList);
	QStringList sentenceList = sentenceRates.keys();
	qSort(sentenceList);

	float overallRecognitionRate = 0;

	float wordRecognitionRate = 0;
	float sentenceRecognitionRate = 0;

	QObjectList accs = ui.wgWordResultDetails->children();
	foreach (QObject *acc, accs) {
		if (dynamic_cast<AccuracyDisplay*>(acc)) {
			acc->deleteLater();
			fprintf(stderr, "deleted accuracy display\n");
		}
	}
	accs = ui.wgSentenceResultDetails->children();
	foreach (QObject *acc, accs)
		if (dynamic_cast<AccuracyDisplay*>(acc))
			acc->deleteLater();

	foreach (const QString& word, wordList) {
		FloatList rates = wordRates.value(word);
		float avg=0;
		int rateCorrect = 0;

		foreach (float rate, rates) {
			avg += rate;
			if (rate > 0)
				rateCorrect++;
		}
		avg /= (float) rates.count();

		fprintf(stderr, "Recognition rate of \"%s\": %f\nRecognition count: %d\n\n", word.toLocal8Bit().data(), avg, rates.count());

		wordRecognitionRate += avg;

		AccuracyDisplay *acc = new AccuracyDisplay(word, rates.count(), rateCorrect, avg, ui.wgWordResultDetails);
		ui.vbWordResultDetails->addWidget(acc);
	}
	wordRecognitionRate /= (float) wordList.count();

	fprintf(stderr, "--------------------------------------------------------\n");

	foreach (const QString& sentence, sentenceList) {
		FloatList rates = sentenceRates.value(sentence);
		float avg=0;
		int rateCorrect = 0;
		foreach (float rate, rates) {
			avg += rate;
			if (rate > 0)
				rateCorrect++;
		}
		avg /= (float) rates.count();

		fprintf(stderr, "Recognition rate of \"%s\": %f\nRecognition count: %d\n\n", sentence.toLocal8Bit().data(), avg, rates.count());
		sentenceRecognitionRate += avg;

		AccuracyDisplay *acc = new AccuracyDisplay(sentence, rates.count(), rateCorrect, avg, ui.wgSentenceResultDetails);
		ui.vbSentenceResultDetails->addWidget(acc);
	}
	sentenceRecognitionRate /= (float) sentenceList.count();

	overallRecognitionRate = (sentenceRecognitionRate + wordRecognitionRate) / 2.0f;

	ui.pbRecognitionRate->setValue(round(overallRecognitionRate*100.0f));
	ui.pbRecognitionRate->setFormat(QString::number(overallRecognitionRate*100.0f)+" %");
}




SamView::~SamView()
{
}

