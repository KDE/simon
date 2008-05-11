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
#include "word.h"

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
	DefineDefaultValues = 1,
	CheckList = 2,
	ExternalPrograms = 3,
	ModelDescription = 4,
	SelectSource = 5,
	NewModelDescription = 6,
	ModelSettingsPage = 7,
	CreateDictionaryPage =8,
	DictionarySelectTypePage = 9,
	DictionaryBOMPPage = 10,
	DictionaryWiktionaryPage = 11,
	DictionaryLexiconPage = 12,
	DictionaryWorkingPage = 13,
	GrammarDescriptionPage = 14,
	GrammarSelectFilesPage = 15,
	GrammarWorkingPage = 16,
	GrammarSettingsPage = 17,
	SoundSettingsPage = 18,
	PasswordDescriptionPage = 19,
	PasswordSettingsPage = 20,
	JuliusdDescriptionPage = 21,
	JuliusdSettingsPage= 22,
	FinishedPage = 23
};

    FirstRunWizard(QWidget* parent);

    ~FirstRunWizard();

};

#endif
