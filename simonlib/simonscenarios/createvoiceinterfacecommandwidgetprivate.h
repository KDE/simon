/*
 *   Copyright (C) 2010 Peter Grasch <grasch@simon-listens.org>
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

#ifndef SIMON_CREATEVOICEINTERFACECOMMANDWIDGETPRIVATE_H_CC6688587FDE4557983A5AB5A69455BE
#define SIMON_CREATEVOICEINTERFACECOMMANDWIDGETPRIVATE_H_CC6688587FDE4557983A5AB5A69455BE

#include "createcommandwidget.h"
#include "ui_createvoiceinterfacecommandwidget.h"

class VoiceInterfaceCommandTemplate;

class CreateVoiceInterfaceCommandWidgetPrivate : public CreateCommandWidget
{
  Q_OBJECT

    private:
    Ui::CreateVoiceCommandWidget ui;
    VoiceInterfaceCommandTemplate* getCurrentTemplate();

  private slots:
    void initFromTemplate();

  public:
    Command* createCommand(const QString& name, const QString& iconSrc, const QString& description);

    bool init(Command* command);
    bool isComplete();

    explicit CreateVoiceInterfaceCommandWidgetPrivate(CommandManager *manager, QWidget *parent=0);

    ~CreateVoiceInterfaceCommandWidgetPrivate() {}

};
#endif
