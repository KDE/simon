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

#include "createtextmacrocommandwidget.h"
#include "textmacrocommand.h"

CreateTextMacroCommandWidget::CreateTextMacroCommandWidget(CommandManager *manager, QWidget *parent) : CreateCommandWidget(manager, parent)
{
  ui.setupUi(this);

  setWindowIcon(TextMacroCommand::staticCategoryIcon());
  setWindowTitle(TextMacroCommand::staticCategoryText());

  connect(ui.teMacroText, SIGNAL(textChanged()), this, SIGNAL(completeChanged()));
}


bool CreateTextMacroCommandWidget::isComplete()
{
  return !(ui.teMacroText->toPlainText().isEmpty());
}


bool CreateTextMacroCommandWidget::init(Command* command)
{
  Q_ASSERT(command);

  TextMacroCommand *textmacroCommand = dynamic_cast<TextMacroCommand*>(command);
  if (!textmacroCommand) return false;

  ui.teMacroText->setPlainText(textmacroCommand->getText());
  return true;
}


Command* CreateTextMacroCommandWidget::createCommand(const QString& name, const QString& iconSrc, const QString& description)
{
  return new TextMacroCommand(name, iconSrc, description, ui.teMacroText->toPlainText());
}


CreateTextMacroCommandWidget::~CreateTextMacroCommandWidget()
{
}
