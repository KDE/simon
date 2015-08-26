/*
 *   Copyright (C) 2010 Peter Grasch <peter.grasch@bedahr.org>
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

#ifndef SIMON_CREATEVOICEINTERFACECOMMANDWIDGET_H_B31776DBF0984C7A931EDE9CFB539489
#define SIMON_CREATEVOICEINTERFACECOMMANDWIDGET_H_B31776DBF0984C7A931EDE9CFB539489

#include "simonmodelmanagement_export.h"
#include "createcommandwidget.h"

#include <QWidget>

class Command;
class CommandManager;
class QIcon;
class CreateVoiceInterfaceCommandWidgetPrivate;

/**
 *	@class CreateVoiceInterfaceCommandWidget
 *	@brief Provides an abstract base-class for the widget that is going to be displayed in the New/Modify-Command-Dialog
 *
 *	@version 0.1
 *	@date 20.05.2008
 *	@author Peter Grasch
 */
class MODELMANAGEMENT_EXPORT CreateVoiceInterfaceCommandWidget : public CreateCommandWidget
{
  Q_OBJECT

    signals:
  void completeChanged();
  void commandSuggested(Command*);

  private:
    CreateVoiceInterfaceCommandWidgetPrivate *d;
  public:
    Command* createCommand(const QString& name, const QString& iconSrc, const QString& description);

    bool init(Command* command);
    bool isComplete();

    explicit CreateVoiceInterfaceCommandWidget(CommandManager *manager, QWidget *parent=0);

    virtual ~CreateVoiceInterfaceCommandWidget() {}

};
#endif
