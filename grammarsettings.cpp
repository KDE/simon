//
// C++ Implementation: grammarsettings
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "grammarsettings.h"
#include "importgrammarwizard.h"
#include "grammarmanager.h"
#include "mergeterminalswizard.h"
#include <QTableWidgetItem>
#include <QMessageBox>

GrammarSettings::GrammarSettings(QWidget* parent, GrammarManager *grammarManager): SystemWidget(tr("Grammatikeinstellungen"), QIcon(":/images/icons/signature.svg"), tr("Grammatik bearbeiten"), parent)
{
	ui.setupUi(this);
	help = tr("Hier können Sie die Grammatikkonstrukte die von simon erkannt werden anpassen.");
	this->grammarManager = grammarManager;

	this->importGrammarWizard = new ImportGrammarWizard(grammarManager->getWordListManager(), this);
	connect(importGrammarWizard, SIGNAL(grammarCreated(QStringList)), this, SLOT(mergeGrammar(QStringList)));
	
	connect(importGrammarWizard, SIGNAL(finished(int)), ui.pbImportTexts, SLOT(animateClick()));
		
	connect(ui.twSentences, SIGNAL(currentCellChanged(int, int, int, int)), this, SLOT(sentenceSelectionChanged(int, int, int, int)));
	connect(ui.pbAddSentence, SIGNAL(clicked()), this, SLOT(addSentence()));
	connect(ui.pbDeleteSentence, SIGNAL(clicked()), this, SLOT(deleteSelectedSentence()));
	
	
	this->mergeTerminalsWizard = new MergeTerminalsWizard(grammarManager, this);
	
	connect(ui.pbImportTexts, SIGNAL(toggled(bool)), this, SLOT(showImportWizard(bool)));
	connect(ui.pbMerge, SIGNAL(toggled(bool)), this, SLOT(showMergeWizard(bool)));
}

void GrammarSettings::mergeGrammar(QStringList grammar)
{
	QStringList toInsert;
	
	QStringList currentStructures = getCurrentStructures();
	for (int i=0; i < grammar.count(); i++)
	{
		if (!currentStructures.contains(grammar[i]))
			toInsert << grammar[i];
	}
	insertSentences(toInsert);
}

void GrammarSettings::insertSentences(QStringList sentences)
{
	int currentRowCount = ui.twSentences->rowCount();
	ui.twSentences->setRowCount(currentRowCount+sentences.count());
	for (int i=0; i < sentences.count(); i++)
	{
		ui.twSentences->setItem(i+currentRowCount, 0, new QTableWidgetItem(sentences[i]));
	}
}

void GrammarSettings::addSentence()
{
	ui.twSentences->setRowCount(ui.twSentences->rowCount()+1);
	int newRow =  ui.twSentences->rowCount()-1;
	ui.twSentences->setItem(newRow, 0, new QTableWidgetItem());
	ui.twSentences->setRangeSelected(QTableWidgetSelectionRange(0, 0, ui.twSentences->rowCount()-1,0),false);
	ui.twSentences->setRangeSelected(QTableWidgetSelectionRange(newRow, 0, newRow,0),true);
	ui.twSentences->setCurrentCell(newRow,0);
	ui.twSentences->editItem(ui.twSentences->item(newRow,0));
}

void GrammarSettings::showImportWizard(bool show)
{
	if (show) {
		importGrammarWizard->restart();
		importGrammarWizard->show();
	} else importGrammarWizard->hide();
}

void GrammarSettings::showMergeWizard(bool show)
{
	if (show) {	
		mergeTerminalsWizard->restart();
		mergeTerminalsWizard->show();
	} else mergeTerminalsWizard->hide();
}

void GrammarSettings::sentenceSelectionChanged(int row, int col, int oldrow, int oldcol)
{
	if ((oldrow != -1) && (ui.twSentences->item(oldrow,0)->text().trimmed().isEmpty()))
	{
		if (QMessageBox::question(this, tr("Leerer Satz"), tr("Wollen Sie den leeren Satz löschen?\n\n(Es gibt keine leeren Sätze)"), QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes)
		{
			QCoreApplication::processEvents();
			ui.twSentences->setRangeSelected(QTableWidgetSelectionRange(0, 0, ui.twSentences->rowCount()-1,0),false);
			ui.twSentences->setRangeSelected(QTableWidgetSelectionRange(oldrow, 0, oldrow,0),true);
			ui.twSentences->setCurrentCell(oldrow,0);
			ui.twSentences->editItem(ui.twSentences->item(oldrow,0));
		} else
			ui.twSentences->removeRow(oldrow);
	}
	if (row != -1) ui.pbDeleteSentence->setEnabled(true);
}

void GrammarSettings::deleteSelectedSentence()
{
	int row = ui.twSentences->currentRow();
	if (QMessageBox::question(this, tr("Selektiereten Satz löschen"), tr("Wollen Sie den Satz\n\n\"%1\"\n\nwirklich löschen?").arg(ui.twSentences->item(row,0)->text()), QMessageBox::Yes | QMessageBox::No)==QMessageBox::Yes)
	{
		ui.twSentences->item(row, 0)->setText("_____"); //ugly hack, but if we wouldn't do that,
		//and we would want to delete a (now) empty sentence (i.e. the user deleted its content and
		//wants to remove it now, qt would (while removing the row) set the focus on a different cell.
		//this would then trigger the currentCellChanged() signal which would in terms trigger
		//our sentenceSelectionChanged slot which checks if the slot we just left is empty.
		//in that case that would would evaluate as "true" and simon would ask the user if he wants
		//to delete the (empty) sentence.
		//this is not only unnessecairy but also causes a segmentation fault if the user selects yes
		//(because we then try to delete the row twice)
		
		ui.twSentences->removeRow(row);
	}
}

QStringList GrammarSettings::getCurrentStructures()
{
	QStringList sentences;

	for (int i=0; i < ui.twSentences->rowCount(); i++)
	{
		sentences << ui.twSentences->item(i,0)->text();
	}
	
	return sentences;
}

bool GrammarSettings::apply()
{
	QStringList sentences = getCurrentStructures();
	grammarManager->setStructures(sentences);
	grammarManager->save();
	return true;
}

bool GrammarSettings::init()
{
	QStringList terminals = grammarManager->getTerminals();
	ui.twTerminals->setRowCount(terminals.count());
	for (int i=0; i < terminals.count(); i++)
	{
		ui.twTerminals->setItem(i, 0, new QTableWidgetItem(terminals[i]));
	}

	QStringList sentences = grammarManager->getAllStructures();
	insertSentences(sentences);
	
	ui.twTerminals->resizeColumnsToContents();
	ui.twSentences->resizeColumnsToContents();
	return true;
}

bool GrammarSettings::reset()
{
	ui.twSentences->clearContents();
	ui.twSentences->setRowCount(0);
	ui.twTerminals->clearContents();
	ui.twTerminals->setRowCount(0);
	return init();
}



GrammarSettings::~GrammarSettings()
{
}


