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
#include "ImportGrammar/importgrammarwizard.h"
#include "grammarmanager.h"
#include "MergeTerminals/mergeterminalswizard.h"
#include "RenameTerminal/renameterminalwizard.h"
#include <QTableWidgetItem>
#include <QMessageBox>

GrammarSettings::GrammarSettings(QWidget* parent): SystemWidget(tr("Grammatikeinstellungen"), QIcon(":/images/icons/signature.svg"), tr("Grammatik bearbeiten"), parent)
{
	ui.setupUi(this);
	help = tr("Hier können Sie die Grammatikkonstrukte die von simon erkannt werden anpassen.");

	this->importGrammarWizard = new ImportGrammarWizard(this);

	this->renameTerminalWizard = new RenameTerminalWizard(this);

	connect(importGrammarWizard, SIGNAL(grammarCreated(QStringList)), this, SLOT(mergeGrammar(QStringList)));
	connect(importGrammarWizard, SIGNAL(finished(int)), ui.pbImportTexts, SLOT(toggle()));
		
	connect(ui.twSentences, SIGNAL(currentCellChanged(int, int, int, int)), this, SLOT(sentenceSelectionChanged(int, int, int, int)));
	connect(ui.pbAddSentence, SIGNAL(clicked()), this, SLOT(addSentence()));
	connect(ui.pbDeleteSentence, SIGNAL(clicked()), this, SLOT(deleteSelectedSentence()));
	
	
	this->mergeTerminalsWizard = new MergeTerminalsWizard(this);
	
	connect(ui.pbImportTexts, SIGNAL(toggled(bool)), this, SLOT(showImportWizard(bool)));
	connect(ui.pbMerge, SIGNAL(toggled(bool)), this, SLOT(showMergeWizard(bool)));
	connect(mergeTerminalsWizard, SIGNAL(finished(int)), ui.pbMerge, SLOT(animateClick()));
	connect(renameTerminalWizard, SIGNAL(finished(int)), ui.pbRename, SLOT(animateClick()));

	connect(mergeTerminalsWizard, SIGNAL(finished(int)), this, SLOT(reset()));
	connect(renameTerminalWizard, SIGNAL(finished(int)), this, SLOT(reset()));

	connect(ui.twSentences, SIGNAL(cellChanged(int, int)), this, SIGNAL(changed()));
	
	connect (ui.pbRename, SIGNAL(toggled(bool)), this, SLOT(showRenameWizard(bool)));
}

void GrammarSettings::askForSave()
{
	if (QMessageBox::question(this, tr("Grammatik speichern"), tr("Sie möchten eine Aktion ausführen, die eine gespeicherte Grammatik benötigt.\n\nWenn Sie Ihre aktuellen Änderungen beibehalten möchten, müssen Sie jetzt Ihre Grammatik speichern.(Ansonsten wird mit der zuletzt gespeicherten Grammatik weitergearbeitet)\n\nWollen Sie das jetzt tun?"), QMessageBox::Yes| QMessageBox::No) == QMessageBox::Yes)
		apply();
}

void GrammarSettings::showRenameWizard(bool show)
{
	if (show)
	{
		askForSave();
		this->renameTerminalWizard->restart();
		this->renameTerminalWizard->show();
	}else this->renameTerminalWizard->hide();
	
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

QStringList GrammarSettings::getCurrentTerminals()
{
	QStringList terms;
	QString sent;
	for (int i=0; i < ui.twSentences->rowCount(); i++)
	{
		sent = ui.twSentences->item(i,0)->text();
		QStringList termtemp = sent.split(" ");
		for (int j=0; j < termtemp.count(); j++)
		{
			if (!terms.contains(termtemp[j])) terms << termtemp[j];
		}
	}
	return terms;
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
		askForSave();
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

/**
 * \brief Returns if true if we filled out the required fields
 * @return always true - there are no mandatory fields here
 */
bool GrammarSettings::isComplete()
{
	return true;
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
	GrammarManager *grammarManager = GrammarManager::getInstance();
	QStringList sentences = getCurrentStructures();
	grammarManager->setStructures(sentences);
	grammarManager->save();
	return true;
}

bool GrammarSettings::init()
{
	ui.twSentences->setRowCount(0);
	GrammarManager *grammarManager = GrammarManager::getInstance();
	QStringList terminals = grammarManager->getTerminals();

	QStringList sentences = grammarManager->getAllStructures();
	insertSentences(sentences);
	
	ui.twSentences->resizeColumnsToContents();
	return true;
}

bool GrammarSettings::reset()
{
	ui.twSentences->clearContents();
	ui.twSentences->setRowCount(0);
	return init();
}



GrammarSettings::~GrammarSettings()
{
    importGrammarWizard->deleteLater();
    renameTerminalWizard->deleteLater();
    mergeTerminalsWizard->deleteLater();
}


