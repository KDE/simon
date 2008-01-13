//
// C++ Implementation: importgrammarselectfilespage
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "importgrammarselectfilespage.h"
#include <QFileDialog>

//todo: document
//ambiguous words and words with more than one meaning are still ignored when using the "Also include unknown constructs" option; This is not a bug!
ImportGrammarSelectFilesPage::ImportGrammarSelectFilesPage(QWidget* parent): QWizardPage(parent)
{
	setTitle(tr("Eingabedateien"));
	ui.setupUi(this);
	ui.leFiles->setVisible(false);
	
	connect(ui.lwFiles, SIGNAL(currentRowChanged(int)), this, SLOT(rowChanged(int)));
	connect(ui.pbAddFile, SIGNAL(clicked()), this, SLOT(addFile()));
	connect(ui.pbDeleteFile, SIGNAL(clicked()), this, SLOT(removeFile()));
	
	registerField("files*", ui.leFiles);
	registerField("includeUnknown", ui.cbIncludeUnknown);
}

void ImportGrammarSelectFilesPage::cleanupPage()
{
	ui.lwFiles->clear();
}

void ImportGrammarSelectFilesPage::addFile()
{
	QStringList files = QFileDialog::getOpenFileNames(this, tr("Datei(en) auswählen"), ".", tr("Textdateien")+" (*.txt)");
	ui.lwFiles->addItems(files);
	setField("files", getFiles().join("||"));
}

void ImportGrammarSelectFilesPage::removeFile()
{
	ui.lwFiles->takeItem(ui.lwFiles->currentRow());
	setField("files", getFiles().join("||"));
}

void ImportGrammarSelectFilesPage::rowChanged(int row)
{
	if (row != -1) ui.pbDeleteFile->setEnabled(true);
	else ui.pbDeleteFile->setEnabled(false);
}

QStringList ImportGrammarSelectFilesPage::getFiles()
{
	QStringList files;
	for (int i=0; i < ui.lwFiles->count(); i++)
	{
		files << ui.lwFiles->item(i)->text();
	}
	return files;
}

ImportGrammarSelectFilesPage::~ImportGrammarSelectFilesPage()
{
}
