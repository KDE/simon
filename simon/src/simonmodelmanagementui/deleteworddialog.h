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

#ifndef SIMON_DELETEWORDDIALOG_H_D9E8F7F149E24D5FB0E88132943CE4E8
#define SIMON_DELETEWORDDIALOG_H_D9E8F7F149E24D5FB0E88132943CE4E8

#include <QDialog>
#include "ui_removeword.h"

/**
 \class DeleteWordDialog
 \author Peter Grasch
 \date 13.12.2007
 \brief Shows a dialog which lets the user decide if he just wants to move the word to the shadow list or delete it
 \version 0.1
*/
class Word;

class DeleteWordDialog : public QDialog
{

  private:
    Ui::DlgRemoveWord ui;
  public:

    enum DeletionType
    {
      MoveToUnused  = 0x00,
      MoveToShadow  = 0x01,
      SoftDelete  = 0x02,
      HardDelete  = 0x03
    };

    explicit DeleteWordDialog(QWidget* parent=0, Qt::WindowFlags f=0);

    int exec(Word *word, bool isShadowed);

    DeletionType getDeletionType();

};
#endif
