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
	@author Peter Grasch <bedahr@gmx.net>
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
	void mergeGrammarStructure(QStringList structures);

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
	DictionaryWorkingPage = 11,
	GrammarDescriptionPage = 12,
	GrammarSelectFilesPage = 13,
	GrammarWorkingPage = 14,
	GrammarSettingsPage = 15,
	SoundSettingsPage = 16,
	PasswordDescriptionPage = 17,
	PasswordSettingsPage = 18,
	JuliusdDescriptionPage = 19,
	JuliusdSettingsPage= 20,
	FinishedPage = 21
};

    FirstRunWizard(AddWordView *addwordView, QWidget* parent);

    ~FirstRunWizard();

};

#endif
