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
#include <speechmodelcompilationadapter/modelcompilationadapterhtk.h>
#include <simonmodeltest/modeltest.h>
#include <simonmodeltest/fileresultmodel.h>
#include <simonmodeltest/testresult.h>
#include <simonscenarioui/scenariomanagementdialog.h>
#include <simonsound/recwidget.h>
#include <QHash>
#include <KStandardAction>
#include <KActionCollection>
#include <KAction>
#include <KDialog>
#include <KIcon>
#include <KStandardDirs>
#include <KAboutData>
#include <KConfig>
#include <KConfigGroup>
#include <KCMultiDialog>
#include <KMessageBox>
#include <KFileDialog>
#include <KDebug>


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

	KStandardAction::openNew(this, SLOT(newProject()), actionCollection());
	KStandardAction::save(this, SLOT(save()), actionCollection());
	KStandardAction::saveAs(this, SLOT(saveAs()), actionCollection());
	KStandardAction::open(this, SLOT(load()), actionCollection());
	KStandardAction::preferences(this, SLOT(showConfig()), actionCollection());
	KStandardAction::quit(this, SLOT(close()), actionCollection());

	setupGUI();

	ui.urHmmDefs->setMode(KFile::File|KFile::ExistingOnly|KFile::LocalOnly);
	ui.urTiedlist->setMode(KFile::File|KFile::ExistingOnly|KFile::LocalOnly);
	ui.urDict->setMode(KFile::File|KFile::ExistingOnly|KFile::LocalOnly);
	ui.urJConf->seMode(KFile::File|KFile::ExistingOnly|KFile::LocalOnly);
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
	ui.urBaseHmmDefs->setMode(KFile::File|KFile::ExistingOnly|KFile::LocalOnly);
	ui.urBaseTiedlist->setMode(KFile::File|KFile::ExistingOnly|KFile::LocalOnly);
	ui.urBaseMacros->setMode(KFile::File|KFile::ExistingOnly|KFile::LocalOnly);
	ui.urBaseStats->setMode(KFile::File|KFile::ExistingOnly|KFile::LocalOnly);

	modelCompilationAdapter = new ModelCompilationAdapterHTK("internalsamuser", this);
	connect(modelCompilationAdapter, SIGNAL(adaptionComplete()), this, SLOT(slotModelAdaptionComplete()));
	connect(modelCompilationAdapter, SIGNAL(adaptionAborted()), this, SLOT(slotModelAdaptionAborted()));
	connect(modelCompilationAdapter, SIGNAL(status(QString, int)), this, SLOT(slotModelAdaptionStatus(QString, int)));
	connect(modelCompilationAdapter, SIGNAL(error(QString)), this, SLOT(slotModelAdaptionError(QString)));
	

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

	ui.tvFiles->setSelectionBehavior(QAbstractItemView::SelectRows);

	connect(ui.tvFiles, SIGNAL(pressed(QModelIndex)), this, SLOT(slotFileResultSelected(QModelIndex)));
	connect(ui.pbEditSample, SIGNAL(clicked()), this, SLOT(slotEditSelectedSample()));

	connect(ui.pbSerializeScenarios, SIGNAL(clicked()), this, SLOT(serializeScenarios()));
	connect(ui.pbSerializePrompts, SIGNAL(clicked()), this, SLOT(serializePrompts()));
}

void SamView::showConfig()
{
	KCMultiDialog *configDialog = new KCMultiDialog(this);
	configDialog->addModule("simonsoundconfig", QStringList() << "");
	configDialog->exec();
	configDialog->deleteLater();
}

void SamView::newProject()
{
	m_filename = "";
	updateWindowTitle();

	ui.urHmmDefs->setUrl(KUrl());
	ui.urTiedlist->setUrl(KUrl());
	ui.urDict->setUrl(KUrl());
	ui.urDFA->setUrl(KUrl());
	ui.urPromptsBasePath->setUrl(KUrl());
	ui.urTestPromptsBasePath->setUrl(KUrl());
	ui.urLexicon->setUrl(KUrl());
	ui.urGrammar->setUrl(KUrl());
	ui.urVocabulary->setUrl(KUrl());
	ui.urPrompts->setUrl(KUrl());
	ui.urTestPrompts->setUrl(KUrl());
	ui.urTreeHed->setUrl(KUrl());
	ui.urWavConfig->setUrl(KUrl());
	ui.sbSampleRate->setValue(16000 /* default*/);
	ui.urJConf->setUrl(KUrl());

	ui.twMain->setCurrentIndex(0);
}

void SamView::load()
{
	QString filename = KFileDialog::getOpenFileName(KUrl(), i18n("sam projects *.sam"), this);
	if (filename.isEmpty()) return;

	m_filename = filename;
	parseFile();
}

void SamView::save()
{
	if (m_filename.isEmpty()) {
		saveAs();
		return;
	}

	storeFile();
}

void SamView::saveAs()
{
	QString filename = KFileDialog::getSaveFileName(KUrl(), i18n("sam projects *.sam"), this);
	if (filename.isEmpty())
		return;

	m_filename = filename;
	storeFile();
}


void SamView::updateWindowTitle()
{
	QString decoFile = m_filename;
	if (m_filename.isEmpty())
		decoFile = i18n("Untitled");

	setWindowTitle(i18n("sam - %1", decoFile));
}

void SamView::parseFile()
{
	//read from m_filename
	QFile f(m_filename);
	if (!f.open(QIODevice::ReadOnly)) {
		KMessageBox::error(this, i18n("Cannot open file: %1", m_filename));
	}

	ui.urHmmDefs->setUrl(KUrl(QString::fromUtf8(f.readLine()).trimmed()));
	ui.urTiedlist->setUrl(KUrl(QString::fromUtf8(f.readLine()).trimmed()));
	ui.urDict->setUrl(KUrl(QString::fromUtf8(f.readLine()).trimmed()));
	ui.urDFA->setUrl(KUrl(QString::fromUtf8(f.readLine()).trimmed()));
	ui.urPromptsBasePath->setUrl(KUrl(QString::fromUtf8(f.readLine()).trimmed()));
	ui.urTestPromptsBasePath->setUrl(KUrl(QString::fromUtf8(f.readLine()).trimmed()));
	ui.urLexicon->setUrl(KUrl(QString::fromUtf8(f.readLine()).trimmed()));
	ui.urGrammar->setUrl(KUrl(QString::fromUtf8(f.readLine()).trimmed()));
	ui.urVocabulary->setUrl(KUrl(QString::fromUtf8(f.readLine()).trimmed()));
	ui.urPrompts->setUrl(KUrl(QString::fromUtf8(f.readLine()).trimmed()));
	ui.urTestPrompts->setUrl(KUrl(QString::fromUtf8(f.readLine()).trimmed()));
	ui.urTreeHed->setUrl(KUrl(QString::fromUtf8(f.readLine()).trimmed()));
	ui.urWavConfig->setUrl(KUrl(QString::fromUtf8(f.readLine()).trimmed()));
	ui.sbSampleRate->setValue(QString::fromUtf8(f.readLine()).trimmed().toInt());
	ui.urJConf->setUrl(KUrl(QString::fromUtf8(f.readLine()).trimmed()));

	updateWindowTitle();
}

void SamView::storeFile()
{
	//store to m_filename
	QFile f(m_filename);
	if (!f.open(QIODevice::WriteOnly)) {
		KMessageBox::error(this, i18n("Cannot open file: %1", m_filename));
	}

	f.write(ui.urHmmDefs->url().path().toUtf8()+"\n");
	f.write(ui.urTiedlist->url().path().toUtf8()+"\n");
	f.write(ui.urDict->url().path().toUtf8()+"\n");
	f.write(ui.urDFA->url().path().toUtf8()+"\n");
	f.write(ui.urPromptsBasePath->url().path().toUtf8()+"\n");
	f.write(ui.urTestPromptsBasePath->url().path().toUtf8()+"\n");
	f.write(ui.urLexicon->url().path().toUtf8()+"\n");
	f.write(ui.urGrammar->url().path().toUtf8()+"\n");
	f.write(ui.urVocabulary->url().path().toUtf8()+"\n");
	f.write(ui.urPrompts->url().path().toUtf8()+"\n");
	f.write(ui.urTestPrompts->url().path().toUtf8()+"\n");
	f.write(ui.urTreeHed->url().path().toUtf8()+"\n");
	f.write(ui.urWavConfig->url().path().toUtf8()+"\n");
	f.write(QString::number(ui.sbSampleRate->value()).toUtf8()+"\n");
	f.write(ui.urJConf->url().path().toUtf8()+"\n");

	updateWindowTitle();
}

void SamView::getBuildPathsFromSimon()
{
	ui.urHmmDefs->setUrl(KUrl(KStandardDirs::locate("data", "simon/model/hmmdefs")));
	ui.urTiedlist->setUrl(KUrl(KStandardDirs::locate("data", "simon/model/tiedlist")));
	ui.urDict->setUrl(KUrl(KStandardDirs::locate("data", "simon/model/model.dict")));
	ui.urDFA->setUrl(KUrl(KStandardDirs::locate("data", "simon/model/model.dfa")));
	ui.urPromptsBasePath->setUrl(KUrl(KStandardDirs::locate("data", "simon/model/training.data/")));
	ui.urTestPromptsBasePath->setUrl(KUrl(KStandardDirs::locate("data", "simon/model/training.data/")));

	ui.urTreeHed->setUrl(KUrl(KStandardDirs::locate("data", "simon/model/tree1.hed")));
	ui.urWavConfig->setUrl(KUrl(KStandardDirs::locate("data", "simon/model/wav_config")));

	KSharedConfig::Ptr config = KSharedConfig::openConfig("speechmodelmanagementrc");
	KConfigGroup group(config, "Model");
	int sampleRate = group.readEntry("ModelSampleRate", "16000").toInt();

	ui.sbSampleRate->setValue(sampleRate);
	ui.urJConf->setUrl(KUrl(KStandardDirs::locate("data", "simon/model/julius.jconf")));


	QString promptsPath = KFileDialog::getOpenFileName(KUrl(), "", this, i18n("Open simon prompts"));
	if (promptsPath.isEmpty()) return;

	QString path = getTargetDirectory();
	if (path.isEmpty()) return;

	KSharedConfig::Ptr scenarioRc = KSharedConfig::openConfig("simonscenariosrc");
	KConfigGroup scenarioRcGroup(scenarioRc, "");
	QStringList ids = scenarioRcGroup.readEntry("SelectedScenarios", QStringList());
	serializeScenariosRun(ids, path);
}


QString SamView::getTargetDirectory()
{
	KMessageBox::information(this, i18n("You now have to provide a (preferrably empty) directory where you want to serialize the scenarios to"), QString(), i18n("Don't ask again"));
	QString path = KFileDialog::getExistingDirectory(KUrl(), this, i18n("Serialized scenario output"));
	if (!path.isEmpty())
		path += QDir::separator();
	return path;
}


void SamView::serializePrompts()
{
	QString promptsPath = KFileDialog::getOpenFileName(KUrl(), "", this, i18n("Open simon prompts"));
	if (promptsPath.isEmpty()) return;

	QString path = getTargetDirectory();
	if (path.isEmpty()) return;
	serializePromptsRun(promptsPath, path);
	switchToAdapt();
}

void SamView::switchToAdapt()
{
	ui.twMain->setCurrentIndex(1);
	ui.teBuildLog->clear();
	ui.pbAdaptProgress->setValue(0);
}

void SamView::serializePromptsRun(const QString promptsPath, const QString& output)
{
	if (output.isEmpty()) return;

	QString promptsOutPath = output+"prompts";
	modelCompilationAdapter->startAdaption(
			(ModelCompilationAdapter::AdaptionType) (ModelCompilationAdapter::AdaptAcousticModel),
			QString(), QString(), 
			QString(), promptsOutPath, 
			QStringList(), promptsPath);

	QString promptsTestPath = output+"prompts_test" ;
	if (QFile::exists(promptsTestPath))
		promptsTestPath = promptsOutPath;
}

void SamView::serializeScenarios()
{
	QString prefix = "simon/"; //use simon scenarios and config
	ScenarioManagementDialog *dlg = new ScenarioManagementDialog(prefix, this);
	if (dlg->exec()) {
		//reparse scenarios
		QStringList ids = dlg->getSelectedScenarioIds();
		QString path = getTargetDirectory();
		serializeScenariosRun(ids, path);
	}
	dlg->deleteLater();
	switchToAdapt();
}


void SamView::serializeScenariosRun(const QStringList& scenarioIds, const QString& path)
{
	if (path.isEmpty()) return;
	QStringList scenarioPaths;
	foreach (const QString& id, scenarioIds)
	{
		QString resolvedPath = KStandardDirs::locateLocal("data", "simon/scenarios/"+id);
		if (!QFile::exists(resolvedPath))
			KMessageBox::information(this, i18n("Couldn't find scenario: %1", id));
		else scenarioPaths << resolvedPath;
	}

	modelCompilationAdapter->startAdaption(
			(ModelCompilationAdapter::AdaptionType) (ModelCompilationAdapter::AdaptLanguageModel),
			path+"lexicon", path+"model.grammar", 
			path+"simple.voca", path+"prompts", 
			scenarioPaths, ui.urPrompts->url().path());
}

void SamView::compileModel()
{
	ui.twMain->setCurrentIndex(2);

	ui.teBuildLog->clear();
	
	modelCompilationManager->startCompilation(
			(ModelCompilationManager::CompilationType)
			(ModelCompilationManager::CompileLanguageModel|ModelCompilationManager::CompileSpeechModel),
			ui.urHmmDefs->url().path(),
			ui.urTiedlist->url().path(),
			ui.urDict->url().path(),
			ui.urDFA->url().path(),
			
			ui.urBaseHmmDefs->url().path(),
			ui.urBaseTiedlist->url().path(),
			ui.urBaseMacros->url().path(),
			ui.urBaseStats->url().path(),

			ui.urPromptsBasePath->url().path(),
			ui.urLexicon->url().path(),
			ui.urGrammar->url().path(),
			ui.urVocabulary->url().path(),
			ui.urPrompts->url().path(),
			ui.urTreeHed->url().path(),
			ui.urWavConfig->url().path()
			);

	/*
	QString activeDir = KStandardDirs::locateLocal("appdata", "models/"+username+"/active/");
									 
	int baseModelType = synchronisationManager->getBaseModelType();
	switch (baseModelType)
	{
		case 0:
			//static model
			modelCompilationManager->startCompilation(
					(ModelCompilationManager::CompileLanguageModel),
					activeDir+"hmmdefs", activeDir+"tiedlist",
					activeDir+"model.dict", activeDir+"model.dfa",
					activeDir+"basehmmdefs", activeDir+"basetiedlist",
					activeDir+"basemacros", activeDir+"basestats",
					KStandardDirs::locateLocal("appdata", "models/"+username+"/samples/"),
					activeDir+"lexicon", activeDir+"model.grammar", activeDir+"simple.voca", 
					activeDir+"prompts", synchronisationManager->getTreeHedPath(), 
					synchronisationManager->getWavConfigPath());

			QFile::remove(activeDir+"hmmdefs");
			QFile::remove(activeDir+"tiedlist");
			QFile::copy(activeDir+"basehmmdefs", activeDir+"hmmdefs");
			QFile::copy(activeDir+"basetiedlist", activeDir+"tiedlist");
			break;
		case 1:
			//adapted base model
			modelCompilationManager->startCompilation(
					(ModelCompilationManager::CompilationType)
					(ModelCompilationManager::CompileLanguageModel|ModelCompilationManager::AdaptSpeechModel),
					activeDir+"hmmdefs", activeDir+"tiedlist",
					activeDir+"model.dict", activeDir+"model.dfa",
					activeDir+"basehmmdefs", activeDir+"basetiedlist",
					activeDir+"basemacros", activeDir+"basestats",
					KStandardDirs::locateLocal("appdata", "models/"+username+"/samples/"),
					activeDir+"lexicon", activeDir+"model.grammar", activeDir+"simple.voca", 
					activeDir+"prompts", synchronisationManager->getTreeHedPath(), 
					synchronisationManager->getWavConfigPath());
		
			break;

		case 2:
			//dynamic model
			modelCompilationManager->startCompilation(
					(ModelCompilationManager::CompilationType)
					(ModelCompilationManager::CompileLanguageModel|ModelCompilationManager::CompileSpeechModel),
					activeDir+"hmmdefs", activeDir+"tiedlist",
					activeDir+"model.dict", activeDir+"model.dfa",
					activeDir+"basehmmdefs", activeDir+"basetiedlist",
					activeDir+"basemacros", activeDir+"basestats",
					KStandardDirs::locateLocal("appdata", "models/"+username+"/samples/"),
					activeDir+"lexicon", activeDir+"model.grammar", activeDir+"simple.voca", 
					activeDir+"prompts", synchronisationManager->getTreeHedPath(), 
					synchronisationManager->getWavConfigPath());
			break;
	}
	*/
}

void SamView::slotModelAdaptionComplete()
{
	ui.twMain->setCurrentIndex(0);

	QString lexicon = modelCompilationAdapter->lexiconPath();
	QString grammar = modelCompilationAdapter->grammarPath();
	QString simpleVocab = modelCompilationAdapter->simpleVocabPath();
	QString prompts = modelCompilationAdapter->promptsPath();

	if (!lexicon.isEmpty()) ui.urLexicon->setUrl(KUrl(lexicon));
	if (!grammar.isEmpty()) ui.urGrammar->setUrl(KUrl(grammar));
	if (!simpleVocab.isEmpty()) ui.urVocabulary->setUrl(KUrl(simpleVocab));
	if (!prompts.isEmpty()) {
		ui.urPrompts->setUrl(KUrl(prompts));
		QFileInfo fi(prompts);
		QString path = fi.absolutePath();
		ui.urPromptsBasePath->setUrl(KUrl(path));

		QString promptsTestPath = path+QDir::separator()+"prompts_test";

		if (QFile::exists(promptsTestPath))
			ui.urTestPrompts->setUrl(KUrl(promptsTestPath));
		else
			ui.urTestPrompts->setUrl(KUrl(prompts));
		ui.urTestPromptsBasePath->setUrl(KUrl(path));
	}
}

void SamView::slotModelAdaptionAborted()
{
	KMessageBox::information(this, i18n("Adaption aborted"));
}

void SamView::slotModelAdaptionStatus(QString status, int progress)
{
	ui.pbAdaptProgress->setValue(progress);
	ui.teAdaptLog->append(status);
}

void SamView::slotModelAdaptionError(QString errorMessage)
{
	KMessageBox::error(this, i18n("Failed to adapt model:\n\n%1", errorMessage));
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
	ui.twMain->setCurrentIndex(4);
}


void SamView::testModel()
{
	ui.twMain->setCurrentIndex(3);

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
	foreach (QObject *acc, accs) 
		if (dynamic_cast<AccuracyDisplay*>(acc)) 
			acc->deleteLater();
	
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

		wordRecognitionRate += avg;

		AccuracyDisplay *acc = new AccuracyDisplay(word, rates.count(), rateCorrect, avg, ui.wgWordResultDetails);
		ui.vbWordResultDetails->addWidget(acc);
	}
	wordRecognitionRate /= (float) wordList.count();

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

		sentenceRecognitionRate += avg;

		AccuracyDisplay *acc = new AccuracyDisplay(sentence, rates.count(), rateCorrect, avg, ui.wgSentenceResultDetails);
		ui.vbSentenceResultDetails->addWidget(acc);
	}
	sentenceRecognitionRate /= (float) sentenceList.count();

	overallRecognitionRate = (sentenceRecognitionRate + wordRecognitionRate) / 2.0f;

	ui.pbRecognitionRate->setValue(round(overallRecognitionRate*100.0f));
	ui.pbRecognitionRate->setFormat(QString::number(overallRecognitionRate*100.0f)+" %");

	QAbstractItemModel *m = ui.tvFiles->model();
	if (m) m->deleteLater();
	ui.tvFiles->setModel(new FileResultModel(modelTest->getTestResults(), this));
}


void SamView::slotFileResultSelected(QModelIndex index)
{
	QString fileName = ui.tvFiles->model()->data(index, Qt::UserRole).toString();
	TestResult *t = modelTest->getTestResult(fileName);

	if (!t) return;

	RecognitionResultList results = t->getResults();
	
	QString resultInfo="";
	int i=1;
	foreach (const RecognitionResult& result, results) {
		resultInfo += i18n("Result %1 of %2\n=====================\n", i, results.count());
		resultInfo += result.toString();
		resultInfo += "\n\n";
		i++;
	}

	ui.teResultDetails->clear();
	ui.teResultDetails->append(resultInfo);
}

void SamView::slotEditSelectedSample()
{
	QModelIndex index = ui.tvFiles->currentIndex();
	if (!index.isValid())
		return;

	QString fileName = ui.tvFiles->model()->data(index, Qt::UserRole).toString();
	TestResult *t = modelTest->getTestResult(fileName);

	if (!t) return;

	QString originalFileName = modelTest->getOriginalFilePath(fileName);
	//copy to temp
	QString justFileName = originalFileName.mid(originalFileName.lastIndexOf(QDir::separator())+1);
	QString tempFileName = KStandardDirs::locateLocal("tmp", 
					"sam/internalsamuser/edit/"+justFileName);

	//if file could not be copied this is not a reason to display an error or to abort
	//because we could have already deleted the file
	QFile::copy(originalFileName, tempFileName);

	KDialog *d = new KDialog(0);
	RecWidget *rec = new RecWidget(i18n("Modify sample"), 
			t->getPrompt(), tempFileName, d);
	d->setMainWidget(rec);
	if (d->exec()) {
		if (!QFile::exists(tempFileName)) {
			//sample has been deleted
			//removing file from prompts
			QFile prompts(ui.urPrompts->url().path());
			QString tempPromptsPath = KStandardDirs::locateLocal("tmp", "sam/internalsamuser/edit/prompts");
			QFile temp(tempPromptsPath);
			if ((!prompts.open(QIODevice::ReadOnly)) ||
					(!temp.open(QIODevice::WriteOnly))){
				KMessageBox::error(this, i18n("Couldn't modify prompts file"));
			} else {
				while (!prompts.atEnd()) {
					QString line = QString::fromUtf8(prompts.readLine());
					if (!line.startsWith(justFileName.left(justFileName.lastIndexOf("."))+" ")) {
						temp.write(line.toUtf8());
					}
				}

				if ((!QFile::remove(ui.urPrompts->url().path())) ||
						(!QFile::copy(tempPromptsPath, ui.urPrompts->url().path()))) {
					KMessageBox::error(this, i18n("Couldn't overwrite prompts file"));
				}
				

				if (!QFile::remove(originalFileName)) {
					KMessageBox::error(this, i18n("Couldn't remove original sample:  %1.", originalFileName));
				}
			}
		} else {
			if (!QFile::exists(originalFileName)) {
				//we have to re-add this to the prompts
				QFile prompts(ui.urPrompts->url().path());
				if (!prompts.open(QIODevice::WriteOnly|QIODevice::Append)) {
					KMessageBox::error(this, i18n("Couldn't modify prompts file"));
				} else {
					prompts.write(QString("%1 %2").arg(justFileName).arg(t->getPrompt()).toUtf8());
				}
			} else if (!QFile::remove(originalFileName)) {
				KMessageBox::error(this, i18n("Couldn't remove original sample:  %1.", originalFileName));
			}

			//copy sample back
			if (!QFile::copy(tempFileName, originalFileName)) {
				KMessageBox::error(this, i18n("Couldn't copy sample from temporary path %1 to %2.", 
							tempFileName, originalFileName));
			}
		
			
		}
	}
	
	//remove temp sample
	QFile::remove(tempFileName);

	d->deleteLater();
}

SamView::~SamView()
{
}

