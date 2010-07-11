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

#ifndef SIMON_EDITWORDDIALOG_H_D9E8F7F149E24D5FB0E88132943CA4E8
#define SIMON_EDITWORDDIALOG_H_D9E8F7F149E24D5FB0E88132943CA4E8

#include <KDialog>
#include "ui_editword.h"

/**
 \class EditWordDialog
 \author Peter Grasch
 \date 2.1.2010
 \version 0.1
*/
class Word;

class EditWordDialog : public KDialog
{

  Q_OBJECT
    private:
    Ui::EditWordView ui;

  private slots:
    void addTerminal();
  public:

    explicit EditWordDialog(QWidget* parent=0, Qt::WindowFlags f=0);

    int exec(Word *word);

};
#endif
