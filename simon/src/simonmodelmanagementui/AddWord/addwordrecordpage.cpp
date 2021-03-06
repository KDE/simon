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

#include "addwordrecordpage.h"
#include "../TrainSamples/trainsamplepage.h"
#include <simonsound/recwidget.h>
#include <simonscenarios/trainingmanager.h>
#include <QDate>
#include <QTime>
#include <QDir>
#include <QVBoxLayout>
#include <QVariant>

#include <KMessageBox>
#include <KStandardDirs>
#include <KLocalizedString>

/**
 * \brief Constructor - also creates the GUI Elements
 * \author Peter Grasch
 * @param parent
 * The parent of the widget
 */
AddWordRecordPage::AddWordRecordPage(const QString& fieldName_, int pageNr_, int pageMax_, QWidget* parent)
: QWizardPage(parent),
page(0),
pageNr(pageNr_),
pageMax(pageMax_),
fieldName(fieldName_),
stickSample(false)
{
  QVBoxLayout *lay = new QVBoxLayout(this);
  setLayout(lay);
}


bool AddWordRecordPage::isComplete() const
{
  return true;
}


void AddWordRecordPage::cleanupPage()
{
  page->cleanupPage();
}


bool AddWordRecordPage::validatePage()
{
  return page->validatePage();
}

PromptsTable AddWordRecordPage::getPrompts() const
{
  return page->getPrompts();
}

/**
 * \brief Sets the recwidgets-titles to the examples of the previous page (pulled out of the fields wordExamples1,2)
 * \author Peter Grasch
 */
void AddWordRecordPage::initializePage()
{
  QString prompt = field(fieldName).toString();
  if (page && (page->getPrompt() != prompt)) {
    layout()->removeWidget(page);

    if (!stickSample)
      page->cleanUp();

    page->deleteLater();
    page = 0;
  }
  if (!page)
    page = new TrainSamplePage(prompt, pageNr, pageMax, i18n("Training"), this);

  setTitle(page->title());
  connect(page, SIGNAL(completeChanged()), this, SIGNAL(completeChanged()));
  layout()->addWidget(page);

  stickSample = false;
}


void AddWordRecordPage::cleanUp()
{
  if (!page) return;

  if (!stickSample)
    page->cleanUp();
  stickSample=false;
}


/**
 * \brief Destructor;
 * \author Peter Grasch
 */
AddWordRecordPage::~AddWordRecordPage()
{
}
