/*
 *   Copyright (C) 2013 Peter Grasch <peter.grasch@bedahr.org>
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

#ifndef SIMON_DICTATIONREPLACEMENTUI_H_DF2B936F15A84FD8962EF68E1DAFE24F
#define SIMON_DICTATIONREPLACEMENTUI_H_DF2B936F15A84FD8962EF68E1DAFE24F


#include <KDialog>
#include "ui_dictationreplacementui.h"

class Replacement;

class DictationReplacementUi: public KDialog
{
  Q_OBJECT

public:
  explicit DictationReplacementUi(QWidget *parent=0);

  Replacement* add();
  void init(Replacement* r);

private:
  Ui::DictationReplacementUi ui;
};
#endif
