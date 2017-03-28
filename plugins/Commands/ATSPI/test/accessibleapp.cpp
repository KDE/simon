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

#include "accessibleapp.h"
#include <QWidget>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMenu>
#include <QLabel>
#include <QLineEdit>
#include <QMenuBar>
#include <QDebug>

AccessibleApp::AccessibleApp() : btn(0), msg(0)
{
  show();
  menuBar();
}

void AccessibleApp::setUI(QWidget* w)
{
  QWidget *central = centralWidget();
  setCentralWidget(w);
  delete central;
}

void AccessibleApp::setupSingleButton()
{
  QWidget *newConfiguration(new QWidget(this));
  QVBoxLayout lay(newConfiguration);
  btn = new QPushButton("Testbutton", newConfiguration);
  lay.addWidget(btn);

  setUI(newConfiguration);
}

void AccessibleApp::clear()
{
  setUI(new QWidget(this));
  menuBar()->clear();
  delete msg;
  msg = 0;
}


void AccessibleApp::changeButtonText()
{
  btn->setText("Alternate Button");
}
void AccessibleApp::hideButton()
{
  btn->hide();
}
void AccessibleApp::showButton()
{
  btn->show();
}

void AccessibleApp::setupMenu()
{
  QMenu *fileMenu = new QMenu("File", this);
  fileMenu->addAction("Save");
  fileMenu->addAction("Quit");
  menuBar()->addMenu(fileMenu);
  QMenu *settingsMenu = new QMenu("Settings", this);
  settingsMenu->addAction("Configure Demo...");
  menuBar()->addMenu(settingsMenu);
}

void AccessibleApp::setupBuddy()
{
  QWidget *newConfiguration(new QWidget(this));
  QVBoxLayout lay(newConfiguration);
  QLabel *l = new QLabel("Buddy:", this);
  QLineEdit *le = new QLineEdit(this);
  l->setBuddy(le);
  lay.addWidget(l);
  lay.addWidget(le);

  setUI(newConfiguration);
}

void AccessibleApp::setupDialog()
{
  msg = new QMessageBox(QMessageBox::NoIcon, "Title", "Text", QMessageBox::NoButton, this);
  msg->addButton(new QPushButton("Ok", msg), QMessageBox::AcceptRole);
  msg->show();
}

void AccessibleApp::acceptDialog()
{
  msg->accept();
}


