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

#ifndef SIMON_IMPORTDICTVIEW_H_8B2839D12DBF4B519ACA4DF76D63ED5B
#define SIMON_IMPORTDICTVIEW_H_8B2839D12DBF4B519ACA4DF76D63ED5B

#include <simonuicomponents/simonwizard.h>
#include <simonscenarios/word.h>
#include <simonscenarios/vocabulary.h>

/**
 * \class ImportDictView
 * \brief Provides a dialog for importing dictonaries
 * \author Peter Grasch
 * \version 0.1
 */
class QWizardPage;
class ImportDictSelectSourcePage;
class ImportDictBOMPPage;
class ImportDictBOMPDownloadPage;
class ImportDictWorkingPage;

class ImportDictView : public SimonWizard
{
  Q_OBJECT
    private:
    ImportDictWorkingPage *workingPage;

  public slots:
    QWizardPage* createIntroPage();
    ImportDictSelectSourcePage*
      createSelectSourcePage();
    ImportDictBOMPPage* createImportBOMPPage();
    ImportDictBOMPDownloadPage* createImportBOMPDownloadPage();

    QWizardPage* createImportLexiconPage();
    QWizardPage* createImportPLSPage();
    QWizardPage* createImportSPHINXPage();
    QWizardPage* createImportJuliusVocabularyPage();
    ImportDictWorkingPage* createImportDictWorkingPage();
    QWizardPage* createFinishedPage();

    QList<Word*>* importDict(Vocabulary::VocabularyType& type);

  public:
    enum ImportDictWizardPages
    {
      IntroPage=0,
      SelectTypePage=1,
      BompPage=2,
      BompDownloadPage=3,
      LexiconPage=4,
      PLSPage=5,
      SPHINXPage=6,
      JuliusVocabulary=7,
      WorkingPage=8,
      FinishedPage=9
    };
    ImportDictView(QWidget *parent=0);
    ~ImportDictView();

};
#endif
