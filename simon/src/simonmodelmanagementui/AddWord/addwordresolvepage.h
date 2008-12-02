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


#ifndef ADDWORDRESOLVEPAGE_H
#define ADDWORDRESOLVEPAGE_H

#include <QWizardPage>
#include "ui_resolvewordpage.h"
#include <speechmodelbase/word.h>
/**
	\class AddWordResolvePage
	\version 0.1
	\brief Displays a few suggestions and let the user select the terminal and the pronunciation of the new word
	\date 20.12.2007
	@author Peter Grasch
*/
class WordListManager;
class GrammarManager;

class AddWordResolvePage : public QWizardPage
{
Q_OBJECT
private:
	QString suggestedSampa;
	bool terminalDirty;
	Ui::ResolveWordPage ui;
	bool alreadyTriedToConvinceAboutCapitalization;
	WordListManager *wordListManager;
	GrammarManager *grammarManager;
private slots:
	void suggest();
	void createExamples();
	void addTerminal();
	void fetchSimilar();
	void setTerminalDirty() { terminalDirty=true; }
public:
    AddWordResolvePage(QWidget* parent=0);

	const QString getPronunciation() { return ui.leSampa->text(); }
	const QString getName() { return ui.leWord->text(); }
	const QString getTerminal() { return ui.cbType->currentText(); }

	void initializePage();
	void displayWords(WordList *words);
	bool validatePage();

};

#endif
