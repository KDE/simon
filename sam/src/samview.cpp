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
#include <KStandardAction>
#include <KActionCollection>
#include <KAction>
#include <KIcon>


SamView::SamView(QWidget *parent, Qt::WFlags flags) : KXmlGuiWindow(parent, flags)
{
	ui.setupUi(this);

	KAction* recompile = new KAction(this);
	recompile->setText(i18n("Build Model"));
	recompile->setIcon(KIcon("view-refresh"));
	recompile->setShortcut(Qt::CTRL + Qt::Key_F5);
	actionCollection()->addAction("compileModel", recompile);
	connect(recompile, SIGNAL(triggered(bool)),
		this, SLOT(compileModel()));
	
	KStandardAction::quit(this, SLOT(close()),
			      actionCollection());

	setupGUI();
}

void SamView::compileModel()
{
	/*modelCompilationManager = new ModelCompilationManager(user, activeDir+"hmmdefs", activeDir+"tiedlist",
							activeDir+"model.dict", activeDir+"model.dfa", this);
	connect(modelCompilationManager, SIGNAL(modelCompiled()), this, SLOT(activeModelCompiled()));
	connect(modelCompilationManager, SIGNAL(activeModelCompilationAborted()), this, SLOT(activeModelCompilationAborted()));
	connect(modelCompilationManager, SIGNAL(status(const QString&, int, int)), this, SLOT(slotModelCompilationStatus(const QString&, int, int)));
	connect(modelCompilationManager, SIGNAL(error(const QString&)), this, SLOT(slotModelCompilationError(const QString&)));
	connect(modelCompilationManager, SIGNAL(classUndefined(const QString&)), this, 
			SLOT(slotModelCompilationClassUndefined(const QString&)));
	connect(modelCompilationManager, SIGNAL(wordUndefined(const QString&)), this, 
			SLOT(slotModelCompilationWordUndefined(const QString&)));
	connect(modelCompilationManager, SIGNAL(phonemeUndefined(const QString&)), this, 
			SLOT(slotModelCompilationPhonemeUndefined(const QString&)));
			*/
	
}

void SamView::modelInputFilesChanged()
{

}

SamView::~SamView()
{
}
