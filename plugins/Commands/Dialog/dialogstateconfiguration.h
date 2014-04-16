/*
 *   Copyright (C) 2009-2010 Grasch Peter <peter.grasch@bedahr.org>
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

#ifndef SIMON_DIALOGSTATECONFIGURATION_H
#define SIMON_DIALOGSTATECONFIGURATION_H

#include <simondialogengine/dialogturn.h>

#include <QWidget>

class DialogCommandManager;
class DialogState;

namespace Ui {
  class DialogStateView;
}

class DialogStateConfiguration : public QWidget
{
  Q_OBJECT
  signals:
    void deleted();

  private:
    Ui::DialogStateView* ui;
    DialogState* state;
    DialogCommandManager* commandManager;

    DialogTurn* getCurrentTurn();

  private slots:
    void editState();
    void removeState();
    void stateChanged();
    void stateDestroyed();
    void addTurn();
    void editTurn();
    void removeTurn();
    void setCurrentTurn();

  public slots:
    void init();

  public:
    DialogStateConfiguration(QWidget* parent, DialogCommandManager* _commandManager, DialogState* _state);
    ~DialogStateConfiguration();

};
#endif
