/*
 *   Copyright (C) 2009 Mario Strametz <strmam06@htl-kaindorf.ac.at>
 *   Copyright (C) 2009 Grasch Peter <peter.grasch@bedahr.org>
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

#include "keyboardmodifybuttondialog.h"
#include <simoninfo/simoninfo.h>
#include <QWidget>
#include <KMessageBox>
#include <QKeySequence>
#include <kdeversion.h>

KeyboardModifyButtonDialog::KeyboardModifyButtonDialog(QWidget *parent) : KDialog(parent)
{
  QWidget *w = new QWidget(this);
  ui.setupUi(w);

  setMainWidget( w );

  setCaption(i18n("Add key"));

  #if KDE_IS_VERSION(4,1,60)
  ui.ksShortcut->setCheckForConflictsAgainst(KKeySequenceWidget::None);
  #endif
}


bool KeyboardModifyButtonDialog::allFieldsEntered()
{
  return (!ui.leName->text().isEmpty() && !ui.leTrigger->text().isEmpty() &&
    (((ui.cbValueType->currentIndex() == 0) && !ui.leTextValue->text().isEmpty()) ||
    ((ui.cbValueType->currentIndex() == 1) && (!ui.ksShortcut->keySequence().isEmpty()))));
}


Keyboard::ButtonType KeyboardModifyButtonDialog::getCurrentlySelectedButtonType()
{
  Keyboard::ButtonType type;
  switch (ui.cbValueType->currentIndex()) {
    case 0: type = Keyboard::TextButton;
    break;
    case 1: type = Keyboard::ShortcutButton;
    break;
    default: type = Keyboard::NullButton;         //error
    break;
  }
  return type;
}


QString KeyboardModifyButtonDialog::getCurrentValue()
{
  QString value;
  Keyboard::ButtonType type = getCurrentlySelectedButtonType();
  switch (type) {
    case Keyboard::TextButton:
      value = ui.leTextValue->text();
      break;
    case Keyboard::ShortcutButton:
      value = ui.ksShortcut->keySequence().toString();
      break;
    default: value.clear();                   //error
    break;
  }
  return value;
}


KeyboardButton* KeyboardModifyButtonDialog::addButton()
{
  ui.leName->setFocus();
  if(exec()) {
    if (!allFieldsEntered()) {
      KMessageBox::sorry(this, i18n("All fields are mandatory"));
      return false;
    }

    Keyboard::ButtonType type = getCurrentlySelectedButtonType();;
    if (type == Keyboard::NullButton) return 0;

    QString value = getCurrentValue();
    if (value.isNull()) return 0;

    return (new KeyboardButton(ui.leName->text(), ui.leTrigger->text(), type, value));
  }

  return 0;
}


bool KeyboardModifyButtonDialog::setCurrentlySelectedButtonType(Keyboard::ButtonType type)
{
  switch (type) {
    case Keyboard::TextButton:
      ui.cbValueType->setCurrentIndex(0);
      ui.swValueType->setCurrentIndex(0);
      break;
    case Keyboard::ShortcutButton:
      ui.cbValueType->setCurrentIndex(1);
      ui.swValueType->setCurrentIndex(1);
      break;
    default: return false;                        //error
    break;
  }
  return true;
}


bool KeyboardModifyButtonDialog::setCurrentValue(const QString& value)
{
  Keyboard::ButtonType type = getCurrentlySelectedButtonType();
  switch (type) {
    case Keyboard::TextButton:
      ui.leTextValue->setText(value);
      break;
    case Keyboard::ShortcutButton:
      ui.ksShortcut->setKeySequence(QKeySequence(value));
      break;
    default: return false;                        //error
    break;
  }

  return true;
}


bool KeyboardModifyButtonDialog::displayButton(KeyboardButton *button)
{
  ui.leName->setText(button->getTriggerShown());
  ui.leTrigger->setText(button->getTriggerReal());
  setCurrentlySelectedButtonType(button->getValueType());
  setCurrentValue(button->getValue());
  return true;
}


bool KeyboardModifyButtonDialog::editButton(KeyboardButton* button)
{
  ui.leName->setFocus();
  if (!displayButton(button)) {
    KMessageBox::sorry(this, i18n("Invalid button"));
    return false;
  }

  if(exec()) {
    if (!allFieldsEntered()) {
      KMessageBox::sorry(this, i18n("All fields are mandatory"));
      return false;
    }

    Keyboard::ButtonType type = getCurrentlySelectedButtonType();;
    if (type == Keyboard::NullButton) return false;

    QString value = getCurrentValue();
    if (value.isNull()) return false;

    button->setTriggerShown(ui.leName->text());
    button->setTriggerReal(ui.leTrigger->text());
    button->setButtonType(type);
    button->setValue(value);
  }

  return true;
}


int KeyboardModifyButtonDialog::exec()
{
  return KDialog::exec();
}


KeyboardModifyButtonDialog::~KeyboardModifyButtonDialog()
{
}
