//
// C++ Interface: firstrunwizard
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef FIRSTRUNWIZARD_H
#define FIRSTRUNWIZARD_H

#include <QWizard>
#include "../../ModelManagement/WordList/word.h"

/**
 \class FirstRunWizard
 \author Peter Grasch
 \version 0.1
 \date 2.1.2008
 \brief "Setup" replacement - does all the nescessairy configuration to use simon
*/
class QWizardPage;
class WordListManager;
class TrainingManager;
class FirstRunImportGrammarWorkingPage;
class ImportDictWorkingPage;
class SystemWidgetPage;
class AddWordView;
class GrammarManager;

class FirstRunWizard : public QWizard
{

Q_OBJECT

private slots:
	void importDict(WordList *);

private:
	ImportDictWorkingPage *importDictWorkingPage;

	QWizardPage *createIntroPage();
	QWizardPage *createCheckList();

	QWizardPage *createExternalProgramsPage();


	QWizardPage *createLanguageModelDescription();
	QWizardPage *createLanguageModelSelectSource();
	QWizardPage *createNewModelDescription();
	QWizardPage *createModelSettings();
	
	QWizardPage *createImportDictSelectTypePage();
	QWizardPage *createImportDictBOMPPage();
	QWizardPage *createImportDictWiktionaryPage();
	QWizardPage *createImportDictLexiconPage();
	QWizardPage *createImportDictWorkingPage();

	QWizardPage *createGrammarDescriptionPage();

	QWizardPage *createGrammarSettings();

	QWizardPage* createSoundSettingsPage();

	QWizardPage* createPasswordDescriptionPage();
	QWizardPage* createPasswordSettingsPage();

	QWizardPage* createJuliusdDescriptionPage();
	QWizardPage* createJuliusdSettingsPage();

	QWizardPage* createConfigureCommandsPage();

	QWizardPage *createFinishedPage();
	void initDefaultValues();

public:

enum FirstRunWizardPages {
	IntroPage = 0,
	CheckList = 1,
	ExternalPrograms = 2,
	ModelDescription = 3,
	SelectSource = 4,
	NewModelDescription = 5,
	ModelSettingsPage = 6,
	DictionarySelectTypePage = 7,
	DictionaryBOMPPage = 8,
	DictionaryWiktionaryPage = 9,
	DictionaryLexiconPage = 10,
	DictionaryWorkingPage = 11,
	GrammarDescriptionPage = 12,
	GrammarSettingsPage = 13,
	SoundSettingsPage = 14,
	PasswordDescriptionPage = 15,
	PasswordSettingsPage = 16,
	JuliusdDescriptionPage = 17,
	JuliusdSettingsPage= 18,
	FinishedPage = 19
};

    FirstRunWizard(QWidget* parent);

    ~FirstRunWizard();

};

#endif
