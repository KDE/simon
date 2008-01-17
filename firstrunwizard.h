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

#include "simonwizard.h"
#include "word.h"
#include "addwordview.h"

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

class FirstRunWizard : public SimonWizard
{

Q_OBJECT

private slots:
	void setWordListManager(WordListManager*);
	void setTrainingManager(TrainingManager*);
	void importDict(WordList *);

private:
	WordListManager *wordListManager;
	TrainingManager *trainingManager;
	GrammarManager *grammarManager;
	FirstRunImportGrammarWorkingPage *firstRunImportGrammarWorkingPage;
	ImportDictWorkingPage *importDictWorkingPage;
	SystemWidgetPage *grammarSettingsPage;
    AddWordView *addWordView;

	QWizardPage *createIntroPage();
	QWizardPage *createCheckList();

	QWizardPage *createExternalProgramsPage();


	QWizardPage *createLanguageModelDescription();
	QWizardPage *createLanguageModelSelectSource();
	QWizardPage *createNewModelDescription();
	QWizardPage *createModelSettings();

	QWizardPage *createCreateDictionaryPage();
	
	QWizardPage *createImportDictSelectTypePage();
	QWizardPage *createImportDictBOMPPage();
	QWizardPage *createImportDictWiktionaryPage();
	QWizardPage *createImportDictLexiconPage();
	QWizardPage *createImportDictWorkingPage();

	QWizardPage *createGrammarDescriptionPage();
	QWizardPage *createSelectFilesPage();
	QWizardPage *createGrammarWorkingPage();

	QWizardPage *createGrammarSettings();

	QWizardPage* createSoundSettingsPage();

	QWizardPage* createPasswordDescriptionPage();
	QWizardPage* createPasswordSettingsPage();

	QWizardPage* createJuliusdDescriptionPage();
	QWizardPage* createJuliusdSettingsPage();


	QWizardPage* createConfigureTriggersPage();
	QWizardPage* createDefineDefaultValuesPage();

	QWizardPage *createFinishedPage();
public:

enum FirstRunWizardPages {
	IntroPage = 0,
	CheckList = 1,
	ExternalPrograms = 2,
	ModelDescription = 3,
	SelectSource = 4,
	NewModelDescription = 5,
	ModelSettingsPage = 6,
	CreateDictionaryPage =7,
	DictionarySelectTypePage = 8,
	DictionaryBOMPPage = 9,
	DictionaryWiktionaryPage = 10,
	DictionaryLexiconPage = 11,
	DictionaryWorkingPage = 12,
	GrammarDescriptionPage = 13,
	GrammarSelectFilesPage = 14,
	GrammarWorkingPage = 15,
	GrammarSettingsPage = 16,
	SoundSettingsPage = 17,
	PasswordDescriptionPage = 18,
	PasswordSettingsPage = 19,
	JuliusdDescriptionPage = 20,
	JuliusdSettingsPage= 21,
	FinishedPage = 22
};

    FirstRunWizard(AddWordView *addwordView, QWidget* parent);

    ~FirstRunWizard();

};

#endif
