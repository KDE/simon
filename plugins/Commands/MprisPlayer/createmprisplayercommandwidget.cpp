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
#include "mprisconstants.h"

#include <QStringList>

CreateMprisPlayerCommandWidget::CreateMprisPlayerCommandWidget(CommandManager *manager, QWidget *parent)
    : CreateCommandWidget(manager, parent)
{
    ui.setupUi(this);

    setWindowIcon(MprisPlayerCommand::staticCategoryIcon());
    setWindowTitle(MprisPlayerCommand::staticCategoryText());

    ui.cbCommand->addItem(i18n("Play / Pause"),QVariant(PlayPause));
    ui.cbCommand->addItem(i18n("Play"), QVariant(Play));
    ui.cbCommand->addItem(i18n("Pause"), QVariant(Pause));
    ui.cbCommand->addItem(i18n("Stop"), QVariant(Stop));
    ui.cbCommand->addItem(i18n("Next"), QVariant(Next));
    ui.cbCommand->addItem(i18n("Previous"), QVariant(Previous));
    ui.cbCommand->addItem(i18n("Volume Up"), QVariant(VolumeUp));
    ui.cbCommand->addItem(i18n("Volume Down"), QVariant(VolumeDown));
    ui.cbCommand->addItem(i18n("Seek Ahead"), QVariant(SeekAhead));
    ui.cbCommand->addItem(i18n("Seek Back"), QVariant(SeekBack));
}

CreateMprisPlayerCommandWidget::~CreateMprisPlayerCommandWidget()
{
}

Command *CreateMprisPlayerCommandWidget::createCommand(const QString &name, const QString &iconSrc, const QString &description)
{
    CommandRole role = static_cast<CommandRole>(ui.cbCommand->itemData(ui.cbCommand->currentIndex()).toInt());
    return new MprisPlayerCommand(name, iconSrc, description, role);
}

bool CreateMprisPlayerCommandWidget::init(Command *command)
{
    Q_ASSERT(command);

    MprisPlayerCommand *mpCommand = dynamic_cast<MprisPlayerCommand*>(command);
    if (!mpCommand) {
        return false;
    }

    ui.cbCommand->setCurrentIndex(ui.cbCommand->findData(QVariant(mpCommand->role())));

    return true;
}

bool CreateMprisPlayerCommandWidget::isComplete()
{
    return true;
}
