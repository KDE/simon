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

#include "createplacecommandwidget.h"
#include "placecommand.h"
#include "selectplacedialog.h"

CreatePlaceCommandWidget::CreatePlaceCommandWidget(CommandManager *manager, QWidget* parent) : CreateCommandWidget(manager, parent)
{
  ui.setupUi(this);
  ui.urUrl->setMode(KFile::Directory | KFile::File | KFile::ExistingOnly);
  setWindowIcon(PlaceCommand::staticCategoryIcon());
  setWindowTitle(PlaceCommand::staticCategoryText());

  connect(ui.urUrl, SIGNAL(textChanged(QString)), this, SIGNAL(completeChanged()));
  connect(ui.cbImportPlace, SIGNAL(clicked()), this, SLOT(selectPlace()));
}


void CreatePlaceCommandWidget::selectPlace()
{
  SelectPlaceDialog *select = new SelectPlaceDialog(this);
  Command *c = select->selectPlace();

  if (c)
    emit propagateCreatedCommand(c);

  select->deleteLater();
}


bool CreatePlaceCommandWidget::isComplete()
{
  return !(ui.urUrl->url().isEmpty());
}


bool CreatePlaceCommandWidget::init(Command* command)
{
  Q_ASSERT(command);

  PlaceCommand *placeCommand = dynamic_cast<PlaceCommand*>(command);
  if (!placeCommand) return false;

  ui.urUrl->setUrl(placeCommand->getURL());
  return true;
}


Command* CreatePlaceCommandWidget::createCommand(const QString& name, const QString& iconSrc, const QString& description)
{
  return new PlaceCommand(name, iconSrc, description, ui.urUrl->url());
}


CreatePlaceCommandWidget::~CreatePlaceCommandWidget()
{
}
