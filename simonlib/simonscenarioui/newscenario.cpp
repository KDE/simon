/* *   Copyright (C) 2008 Peter Grasch <peter.grasch@bedahr.org>
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

#include "newscenario.h"
#include "newauthor.h"
#include "version.h"

#include <simonscenarios/scenario.h>
#include <simonscenarios/author.h>
#include <simonscenariobase/versionnumber.h>

#include <QDateTime>

#include <KIcon>
#include <KMessageBox>
#include <KDialogButtonBox>

NewScenario::NewScenario(QWidget* parent) : KDialog(parent)
{
  QWidget *widget = new QWidget( this );
  ui.setupUi(widget);

  setMainWidget( widget );
  setCaption( i18n("Scenario") );

  connect(ui.leName, SIGNAL(textChanged(QString)), this, SLOT(setWindowTitleToScenarioName(QString)));
  connect(ui.leName, SIGNAL(textChanged(QString)), this, SLOT(checkIfComplete()));
  connect(ui.leMinVersion, SIGNAL(textChanged(QString)), this, SLOT(checkIfComplete()));
  connect(ui.cbLicense, SIGNAL(editTextChanged(QString)), this, SLOT(checkIfComplete()));

  connect(ui.pbAddAuthor, SIGNAL(clicked()), this, SLOT(addAuthor()));
  connect(ui.pbRemoveAuthor, SIGNAL(clicked()), this, SLOT(removeAuthor()));

  checkIfComplete();
  ui.pbAddAuthor->setIcon(KIcon("list-add"));
  ui.pbRemoveAuthor->setIcon(KIcon("list-remove"));
}


void NewScenario::checkIfComplete()
{
  bool complete  = ! ui.leName->text().isEmpty() &&
    ! ui.cbLicense->currentText().isEmpty() &&
    ! ui.leMinVersion->text().isEmpty() &&
    ! m_authors.isEmpty();

  enableButtonOk(complete);
}


QString NewScenario::createId()
{
  QString name = ui.leName->text();
  return Scenario::createId(name);
}


void NewScenario::setWindowTitleToScenarioName(QString name)
{
  if (!name.isEmpty())
    setCaption(i18nc("%1 is scenario name", "Scenario: %1", name));
  else setCaption(i18n("Scenario"));
}


void NewScenario::addAuthor()
{
  NewAuthor *newAuthor = new NewAuthor(this);
  Author *a = newAuthor->newAuthor();
  delete newAuthor;

  if (!a) return;

  m_authors << a;
  updateAuthorDisplay();
}


void NewScenario::removeAuthor()
{
  int index = ui.lwAuthors->currentRow();
  if (index == -1) return;

  Q_ASSERT(index <= m_authors.count());

  m_authors.removeAt(index);

  updateAuthorDisplay();
}


void NewScenario::updateAuthorDisplay()
{
  ui.lwAuthors->clear();

  foreach (Author *a, m_authors) {
    ui.lwAuthors->addItem(i18nc("Author (contact information)", "%1 (%2)", a->name(), a->contact()));
  }
  checkIfComplete();
}


int NewScenario::exec()
{
  return KDialog::exec();
}


Scenario* NewScenario::newScenario()
{
  m_authors.clear();
  ui.leMinVersion->setText(simon_version);
  if (exec()) {
    //creating
    Scenario *s = new Scenario(createId());
    VersionNumber *minVersion = getMinimumVersion();
    VersionNumber *maxVersion = new VersionNumber(0, ui.leMaxVersion->text());
    if (!s->create(ui.leName->text(), ui.pbIcon->icon(), ui.sbScenarioVersion->value(), minVersion, maxVersion, ui.cbLicense->currentText(), m_authors)) {
      m_authors.clear();
      KMessageBox::sorry(this, i18n("Scenario could not be created"));
      s->deleteLater();
      return 0;
    }
    else {
      if (!s->save()) {
        m_authors.clear();
        KMessageBox::sorry(this, i18n("New scenario could not be saved"));
        s->deleteLater();
        return 0;
      }
      else {
        m_authors.clear();
        return s;
      }
    }
  }
  else {
    qDeleteAll(m_authors);
    m_authors.clear();
    return 0;
  }
}


void NewScenario::displayScenario(Scenario *s)
{
  ui.leName->setText(s->name());
  ui.leMinVersion->setText(s->simonMinVersion()->toString());
  ui.sbScenarioVersion->setValue(s->version());
  VersionNumber *max = s->simonMaxVersion();
  QString maxStrVersion = "";
  if (max)
    maxStrVersion = max->toString();
  ui.leMaxVersion->setText(maxStrVersion);
  ui.cbLicense->setEditText(s->license());

  ui.pbIcon->setIcon(s->iconSrc());

  m_authors = s->authors();
  updateAuthorDisplay();
}


VersionNumber* NewScenario::getMinimumVersion()
{
  VersionNumber *minVersion = new VersionNumber(0, ui.leMinVersion->text());
  VersionNumber *currentVersion = new VersionNumber(0, simon_version);
  if (*currentVersion < *minVersion) {
    delete minVersion;
    minVersion= currentVersion;
  } else
  delete currentVersion;

  return minVersion;
}


Scenario* NewScenario::editScenario(Scenario *s)
{
  if (!s) return s;

  bool success = s->init();

  kDebug() << "Scenario initialized: " << success;
  if (!success)
    KMessageBox::sorry(this, i18n("Original scenario could not be read.\n\n"
      "If you continue, all data (vocabulary, grammar, commands, training "
      "texts) of the scenario will be lost!"));

  displayScenario(s);
  if (exec()) {
    VersionNumber *minVersion = getMinimumVersion();

    VersionNumber *maxVersion = new VersionNumber(0, ui.leMaxVersion->text());
    if (!s->update(ui.leName->text(), ui.pbIcon->icon(), ui.sbScenarioVersion->value(), minVersion, maxVersion, ui.cbLicense->currentText(), m_authors)) {
      KMessageBox::sorry(this, i18n("Scenario could not be updated"));
      success = false;
    }
    else if (!s->save()) {
      KMessageBox::sorry(this, i18n("New scenario could not be saved"));
      success = false;
    }
  }
  m_authors.clear();

  if (success == false) return 0;

  return s;
}


NewScenario::~NewScenario()
{
}
