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

#include "importdictview.h"

#include "importdictintropage.h"
#include "importdictworkingpage.h"
#include "importdictselectsourcepage.h"
#include "importdictbomppage.h"
#include "importdictbompdownloadpage.h"
#include "importlexiconpage.h"
#include "importdictplspage.h"
#include "importdictsphinxpage.h"
#include "importdictjuliuspage.h"

#include <QLabel>
#include <QVBoxLayout>
#include <KStandardDirs>

/**
 * \brief Constructor
 * \author Peter Grasch
 * Generates a new ImportDictView object and sets up the signal/slots
 * \param parent
 * The parent of the window.
 */
ImportDictView::ImportDictView(QWidget* parent) : SimonWizard(parent),
workingPage(createImportDictWorkingPage())
{
  addPage(createIntroPage());
  addPage(createSelectSourcePage());
  addPage(createImportBOMPPage());
  addPage(createImportBOMPDownloadPage());
  addPage(createImportLexiconPage());
  addPage(createImportPLSPage());
  addPage(createImportSPHINXPage());
  addPage(createImportJuliusVocabularyPage());

  connect(workingPage, SIGNAL(done()), this, SLOT(next()));
  connect(workingPage, SIGNAL(failed()), this, SLOT(back()));
  addPage(workingPage);
  addPage(createFinishedPage());
  setWindowTitle(i18n("Importing Dictionary"));
  setBanner("importdict");
}


/**
 * \brief Creates the page to import a simon lexicon
 * \author Peter Grasch
 * @return The wizardpage
 */
QWizardPage* ImportDictView::createImportLexiconPage()
{
  return new ImportLexiconPage(this);
}


/**
 * \brief Creates the page to import a pls lexicon
 * \author Peter Grasch
 * @return The wizardpage
 */
QWizardPage* ImportDictView::createImportPLSPage()
{
  return new ImportDictPLSPage(this);
}


/**
 * \brief Creates the page to import a sphinx lexicon
 * \author Peter Grasch
 * @return The wizardpage
 */
QWizardPage* ImportDictView::createImportSPHINXPage()
{
  return new ImportDictSPHINXPage(this);
}


/**
 * \brief Creates the page to import a julius vocabulary
 * \author Peter Grasch
 * @return The wizardpage
 */
QWizardPage* ImportDictView::createImportJuliusVocabularyPage()
{
  return new ImportDictJuliusPage(this);
}


/**
 * \brief Creates the intro page
 * \author Peter Grasch
 *
 * This page contains a short written introduction on what is coming up
 *
 * @return the created qwizardpage
 */
QWizardPage* ImportDictView::createIntroPage()
{
  return new ImportDictIntroPage(this);
}


/**
 * \brief Creates a new ImportDictSelectSourcePage and returns it
 * \author Peter Grasch
 * @return the created page
 */
ImportDictSelectSourcePage* ImportDictView::createSelectSourcePage()
{
  return new ImportDictSelectSourcePage(this);
}


/**
 * \brief Creates a new ImportBOMPPage and returns it
 * \author Peter Grasch
 * @return the created page
 */
ImportDictBOMPPage* ImportDictView::createImportBOMPPage()
{
  return new ImportDictBOMPPage(this);
}


/**
 * \brief Creates a new ImportBOMDownloadPPage and returns it
 * \author Peter Grasch
 * @return the created page
 */
ImportDictBOMPDownloadPage* ImportDictView::createImportBOMPDownloadPage()
{
  return new ImportDictBOMPDownloadPage(this);
}


/**
 * \brief Creates a new ImportDictWorkingPage and returns it
 * \author Peter Grasch
 * @return the created page
 */
ImportDictWorkingPage* ImportDictView::createImportDictWorkingPage()
{
  ImportDictWorkingPage *page = new ImportDictWorkingPage(this);
  connect(this, SIGNAL(rejected()), page, SLOT(abort()));
  return page;
}


/**
 * \brief Creates a new QWizardPage, builds the gui, and returns it
 * \author Peter Grasch
 * @return the created page
 */
QWizardPage* ImportDictView::createFinishedPage()
{
  QWizardPage *finished = new QWizardPage(this);
  finished->setTitle(i18n("Dictionary imported"));
  QLabel *lbFinished = new QLabel(finished);
  lbFinished->setText(i18n("The dictionary has been imported successfully.\n\nThank you for improving Simon."));
  lbFinished->setWordWrap(true);
  QVBoxLayout *lay = new QVBoxLayout(finished);
  lay->addWidget(lbFinished);
  finished->setLayout(lay);
  return finished;
}


QList<Word*> ImportDictView::importDict(Vocabulary::VocabularyType& type)
{
  if (exec()) {
    type = (Vocabulary::VocabularyType) field("targetType").toInt();
    return workingPage->getCurrentWordList();
  }
  return QList<Word*>();
}


/**
 * \brief Destructor
 * \author Peter Grasch
 */
ImportDictView::~ImportDictView()
{
}
