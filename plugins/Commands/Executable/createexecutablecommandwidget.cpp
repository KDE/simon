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

#include "createexecutablecommandwidget.h"
#include "executablecommand.h"
#include "simonuicomponents/selectprogramdialog.h"
#include <KLineEdit>

CreateExecutableCommandWidget::CreateExecutableCommandWidget(CommandManager *manager, QWidget* parent) : CreateCommandWidget(manager, parent)
{
  ui.setupUi(this);

  ui.urWorkingDirectory->setMode(KFile::Directory | KFile::ExistingOnly | KFile::LocalOnly);

  setWindowIcon(ExecutableCommand::staticCategoryIcon());
  setWindowTitle(ExecutableCommand::staticCategoryText());

  #ifdef Q_OS_WIN32
  ui.cbImportProgram->hide();
  ui.cbManual->animateClick();
  ui.cbManual->hide();
  #endif

  connect(ui.urExecutable, SIGNAL(textChanged(QString)), this, SIGNAL(completeChanged()));
  connect(ui.urExecutable, SIGNAL(urlSelected(KUrl)), this, SLOT(urlSelected(KUrl)));

  connect(ui.cbImportProgram, SIGNAL(clicked()), this, SLOT(selectProgram()));
}


void CreateExecutableCommandWidget::urlSelected(const KUrl& urlSelected)
{
  //wrap url in quotes
  ui.urExecutable->lineEdit()->setText("\""+urlSelected.path()+"\"");
}


bool CreateExecutableCommandWidget::isComplete()
{
  return !(ui.urExecutable->url().isEmpty());
}


void CreateExecutableCommandWidget::selectProgram()
{
  SelectProgramDialog *select = new SelectProgramDialog(this);

  if (select->selectCommand())
  {
      Command *c = new ExecutableCommand(select->getName(), select->getIcon(), select->getDescription(),
                                         select->getExecPath(), select->getWorkingDirectory());

      if (c)
        emit propagateCreatedCommand(c);
  }

  select->deleteLater();
}


bool CreateExecutableCommandWidget::init(Command* command)
{
  Q_ASSERT(command);

  ExecutableCommand *execCommand = dynamic_cast<ExecutableCommand*>(command);
  if (!execCommand) return false;

  ui.urExecutable->setUrl(KUrl(execCommand->getExecutable()));
  ui.urWorkingDirectory->setUrl(execCommand->getWorkingDirectory());
  return true;
}


Command* CreateExecutableCommandWidget::createCommand(const QString& name, const QString& iconSrc, const QString& description)
{
  return new ExecutableCommand(name, iconSrc, description, ui.urExecutable->url().path(),
    ui.urWorkingDirectory->url());
}


CreateExecutableCommandWidget::~CreateExecutableCommandWidget()
{
}
