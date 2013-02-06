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

#ifndef SIMON_ACCESSIBLEAPP_H_7A7B9100FF5245329569C1B540119C37
#define SIMON_ACCESSIBLEAPP_H_7A7B9100FF5245329569C1B540119C37

#include <QMainWindow>

class QPushButton;
class QMessageBox;

class AccessibleApp : public QMainWindow
{
  Q_OBJECT
public:
  AccessibleApp();
  void setupSingleButton();
  void changeButtonText();
  void showButton();
  void hideButton();
  void setupMenu();
  void setupBuddy();
  void setupDialog();
  void acceptDialog();
  void clear();

private:
  QPushButton *btn;
  QMessageBox *msg;
  void setUI(QWidget *w);
};

#endif
