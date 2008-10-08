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

CreateExecutableCommandWidget::CreateExecutableCommandWidget(QWidget *parent) : CreateCommandWidget(parent)
{
	ui.setupUi(this);
	
	setWindowIcon(ExecutableCommand::staticCategoryIcon());
	setWindowTitle(ExecutableCommand::staticCategoryText());
	
	connect(ui.urExecutable, SIGNAL(textChanged(const QString&)), this, SLOT(checkIfComplete()));
}

void CreateExecutableCommandWidget::checkIfComplete()
{
	emit complete(!(ui.urExecutable->url().isEmpty()));
}

bool CreateExecutableCommandWidget::init(Command* command)
{
	Q_ASSERT(command);
	
	ExecutableCommand *execCommand = dynamic_cast<ExecutableCommand*>(command);
	if (!execCommand) return false;
	
	ui.urExecutable->setPath(execCommand->getExecutable());
	ui.urWorkingDirectory->setUrl(execCommand->getWorkingDirectory());
	return true;
}

Command* CreateExecutableCommandWidget::createCommand(const QString& name, const QString& iconSrc)
{
	return new ExecutableCommand(name, iconSrc, ui.urExecutable->url().path(), 
					ui.urWorkingDirectory->url());
}

CreateExecutableCommandWidget::~CreateExecutableCommandWidget()
{
}
