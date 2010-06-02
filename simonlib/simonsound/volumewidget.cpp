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

#include "volumewidget.h"
#include "soundconfig.h"
#include "ui_volumewidget.h"
#include "soundconfig.h"
#include <KLocalizedString>

VolumeWidget::VolumeWidget(QWidget *parent) : QWidget(parent),
	ui(new Ui::VolumeWidgetUi())
{
	ui->setupUi(this);
	setPrompt(SoundConfiguration::volumePrompt());
}

void VolumeWidget::setPrompt(const QString& prompt)
{
	ui->tePrompt->setFont(SoundConfiguration::promptFont());
	ui->tePrompt->setText(prompt);

	if (ui->tePrompt->toPlainText().isEmpty())
		ui->tePrompt->setPlainText(i18n("Please say a few sentences. Try to pronounce them clearly but naturally and speak in a normal volume."));
}

VolumeWidget::~VolumeWidget()
{
}

