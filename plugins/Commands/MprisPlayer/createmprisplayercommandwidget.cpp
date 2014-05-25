/*
 *   Copyright (C) 2014 Ashish Madeti <ashishmadeti@gmail.com>
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

#include "createmprisplayercommandwidget.h"
#include "mprisplayercommand.h"

#include <QStringList>

CreateMprisPlayerCommandWidget::CreateMprisPlayerCommandWidget(QString serviceName, CommandManager *manager, QWidget *parent)
    : CreateCommandWidget(manager, parent),
      m_serviceName(serviceName)
{
    ui.setupUi(this);

    setWindowIcon(MprisPlayerCommand::staticCategoryIcon());
    setWindowTitle(MprisPlayerCommand::staticCategoryText());

    connect(ui.cbDBusMethod, SIGNAL(currentTextChanged(QString)), this, SLOT(completeChanged()));
}

CreateMprisPlayerCommandWidget::~CreateMprisPlayerCommandWidget()
{
}

Command *CreateMprisPlayerCommandWidget::createCommand(const QString &name, const QString &iconSrc, const QString &description)
{
    return new MprisPlayerCommand(name, iconSrc, description, m_serviceName,
                                  "/org/mpris/MediaPlayer2", "org.mpris.MediaPlayer2.Player",
                                  ui.cbDBusMethod->currentText(), QStringList());
}

bool CreateMprisPlayerCommandWidget::init(Command *command)
{
    Q_ASSERT(command);

    MprisPlayerCommand *mpCommand = dynamic_cast<MprisPlayerCommand*>(command);
    if (!mpCommand) {
        return false;
    }

    ui.cbDBusMethod->setCurrentIndex(ui.cbDBusMethod->findText(mpCommand->method()));

    return true;
}

bool CreateMprisPlayerCommandWidget::isComplete()
{
    return true;
}
