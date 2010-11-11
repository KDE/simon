/*
 *   Copyright (C) 2008 Peter Grasch <grasch@simon-listens.org>
 *   Copyright (C) 2010 Manfred Scheucher <deadalps@gmail.com>
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

#ifndef SIMON_CREATESHORTCUTCOMMANDWIDGET_H_83B3250DFAD6484386C5F490245A21C3
#define SIMON_CREATESHORTCUTCOMMANDWIDGET_H_83B3250DFAD6484386C5F490245A21C3

#include <QWidget>
#include <simonscenarios/createcommandwidget.h>
#include "ui_createshortcutcommandwidget.h"
#include <eventsimulation/shortcutmode.h>

class Command;
class CommandManager;

/**
 *	@class CreateShortcutCommandWidget
 *	@brief Provides a widget to modify the specific attributes of an ShortcutCommand
 *
 *	@version 0.1
 *	@date 8.10.2008
 *	@author Peter Grasch
 */
class CreateShortcutCommandWidget : public CreateCommandWidget
{
  Q_OBJECT

    private:
    Ui::CreateShortcutCommandWidget ui;
    
    EventSimulation::ShortcutMode getSelectedShortcutMode();
    int getShortcutModeIndex(EventSimulation::ShortcutMode mode);

  private slots:
  #ifdef Q_OS_WIN
    void applySpecialShortcut();
  #endif

  public:
    Command* createCommand(const QString& name, const QString& iconSrc, const QString& description);

    bool init(Command* command);
    bool isComplete();

    /**ShortcutCommand
     * @brief Constructor
     *
     *	@author Peter Grasch
     */
    explicit CreateShortcutCommandWidget(CommandManager *manager, QWidget *parent=0);

    virtual ~CreateShortcutCommandWidget();

};
#endif
