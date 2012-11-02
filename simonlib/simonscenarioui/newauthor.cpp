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

#include "newauthor.h"

#include <simonscenarios/author.h>

#include <KMessageBox>
#include <KDialogButtonBox>

NewAuthor::NewAuthor(QWidget* parent) : KDialog(parent)
{
  QWidget *widget = new QWidget( this );
  ui.setupUi(widget);

  setMainWidget( widget );
  setCaption( i18n("Author") );

  connect(ui.leName, SIGNAL(textChanged(QString)), this, SLOT(checkIfComplete()));
  connect(ui.leContact, SIGNAL(textChanged(QString)), this, SLOT(checkIfComplete()));

  checkIfComplete();
}


Author* NewAuthor::newAuthor()
{
  if (!exec()) return 0;

  return new Author(0, ui.leName->text(), ui.leContact->text());
}


void NewAuthor::checkIfComplete()
{
  bool complete = !ui.leName->text().isEmpty() && !ui.leContact->text().isEmpty();
  enableButtonOk(complete);
}


NewAuthor::~NewAuthor()
{
}
