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
#include <speechmodelcompilation/modelcompilationmanager.h>
#include <KStandardAction>
#include <KActionCollection>
#include <KAction>
#include <KIcon>
#include <KStandardDirs>
#include <KMessageBox>


SamView::SamView(QWidget *parent, Qt::WFlags flags) : KXmlGuiWindow(parent, flags)
{
	ui.setupUi(this);

	KAction* getPathsFromSimon = new KAction(this);
	getPathsFromSimon->setText(i18n("Modify simons model"));
	getPathsFromSimon->setIcon(KIcon("simon"));
	actionCollection()->addAction("getPathsFromSimon", getPathsFromSimon);
	connect(getPathsFromSimon, SIGNAL(triggered(bool)),
		this, SLOT(getBuildPathsFromSimon()));
	
	KAction* recompile = new KAction(this);
	recompile->setText(i18n("Build Model"));
	recompile->setIcon(KIcon("view-refresh"));
	recompile->setShortcut(Qt::CTRL + Qt::Key_F5);
	actionCollection()->addAction("compileModel", recompile);
	connect(recompile, SIGNAL(triggered(bool)),
		this, SLOT(compileModel()));

	connect(ui.pbCompileModel, SIGNAL(clicked()), this, SLOT(compileModel()));

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
	
}

void SamView::getBuildPathsFromSimon()
{
	ui.urHmmDefs->setUrl(KUrl(KStandardDirs::locate("data", "simon/model/hmmdefs")));
	ui.urTiedlist->setUrl(KUrl(KStandardDirs::locate("data", "simon/model/tiedlist")));
	ui.urDict->setUrl(KUrl(KStandardDirs::locate("data", "simon/model/model.dict")));
	ui.urDFA->setUrl(KUrl(KStandardDirs::locate("data", "simon/model/model.dfa")));
	ui.urPromptsBasePath->setUrl(KUrl(KStandardDirs::locate("data", "simon/model/training.data/")));
	ui.urLexicon->setUrl(KUrl(KStandardDirs::locate("data", "simon/model/lexicon")));
	ui.urGrammar->setUrl(KUrl(KStandardDirs::locate("data", "simon/model/model.grammar")));
	ui.urVocabulary->setUrl(KUrl(KStandardDirs::locate("data", "simon/model/model.voca")));
	ui.urPrompts->setUrl(KUrl(KStandardDirs::locate("data", "simon/model/prompts")));
	ui.urTreeHed->setUrl(KUrl(KStandardDirs::locate("data", "simon/model/tree1.hed")));
	ui.urWavConfig->setUrl(KUrl(KStandardDirs::locate("data", "simon/model/wav_config")));
}

void SamView::compileModel()
{
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
}

void SamView::slotModelCompilationWordUndefined(const QString& undefinedWord)
{

}

void SamView::slotModelCompilationPhonemeUndefined(const QString& undefinedPhoneme)
{

}


void SamView::retrieveCompleteBuildLog()
{
	ui.teBuildLog->clear();
	ui.teBuildLog->append(modelCompilationManager->getGraphicBuildLog());
}


SamView::~SamView()
{
}
