/*
 *   Copyright (C) 2008 Peter Grasch <peter.grasch@bedahr.org>
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

#include "importgrammarwizard.h"
#include <QWizardPage>
#include <QLabel>
#include <QHBoxLayout>
#include <KStandardDirs>
#include "importgrammarselectinputpage.h"
#include "importgrammarworkingpage.h"

ImportGrammarWizard::ImportGrammarWizard(QWidget* parent): SimonWizard(parent)
{
  setWindowTitle(i18n("Import Sentence Structures"));
  setBanner("importgrammar");
  addPage(createIntroPage());
  addPage(createSelectInputPage());
  addPage(createWorkingPage());
  addPage(createFinishedPage());
}


QWizardPage* ImportGrammarWizard::createIntroPage()
{
  QWizardPage *intro = new QWizardPage(this);
  QHBoxLayout *lay = new QHBoxLayout(intro);
  QLabel *desc = new QLabel(intro);
  lay->addWidget(desc);
  intro->setLayout(lay);

  desc->setWordWrap(true);
  intro->setTitle(i18n("Welcome to the Import of your Grammar"));
  desc->setText(i18n("This wizard will allow you to select textfiles or input text from which Simon will "
    "then create your "
    "personal grammar profile.\n\nTo do this each of the files / the input text will be read by a "
    "grammar parser which will look up every single word in your dictionary-"
    "corpus. If the word is found its terminal will be added to the sentence. "
    "This way the parsers will find valid grammatical sentences on terminal "
    "level.\n\nThis process requires a large shadow-lexicon containing terminal information or targeted input "
    "material."));

  return intro;
}


QWizardPage* ImportGrammarWizard::createSelectInputPage()
{
  return (QWizardPage*) new ImportGrammarSelectInputPage(this);
}


QWizardPage* ImportGrammarWizard::createWorkingPage()
{
  ImportGrammarWorkingPage *working = new ImportGrammarWorkingPage(this);
  connect(working, SIGNAL(grammarCreated()), this, SLOT(next()));
  connect(button(QWizard::CancelButton), SIGNAL(clicked()), working, SLOT(cancel()));
  return working;
}


QWizardPage* ImportGrammarWizard::createFinishedPage()
{
  QWizardPage *finished = new QWizardPage(this);
  QHBoxLayout *lay = new QHBoxLayout(finished);
  QLabel *desc = new QLabel(finished);
  lay->addWidget(desc);
  finished->setLayout(lay);

  desc->setWordWrap(true);
  finished->setTitle(i18n("Import completed"));
  desc->setText(i18n("The files have been imported.\n\nPlease check the structures for errors.\n\nThank "
    "you for improving Simon!"));

  return finished;
}


ImportGrammarWizard::~ImportGrammarWizard()
{
}
