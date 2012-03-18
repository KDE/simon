/*
 *   Copyright (C) 2011 Peter Grasch <grasch@simon-listens.org>
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


#include "welcomepage.h"
#include "version.h"
#include <KDebug>
#include <KLocalizedString>

WelcomePage::WelcomePage(QWidget* parent) : InlineWidget(i18n("Welcome"), KIcon("simon"), i18n("Welcome to Simon"), parent)
{
  ui.setupUi(this);
  ui.lbWelcome->setText(i18nc("%1: simon version", "Welcome to simon %1", simon_version));
  
  ui.wgVolumeWidget->enablePrompt(false);
  ui.wgVolumeWidget->init();
}

void WelcomePage::showEvent(QShowEvent* event)
{
  QWidget::showEvent(event);
  kDebug() << "Shown";
  ui.wgVolumeWidget->start();
}

void WelcomePage::hideEvent(QHideEvent* event)
{
  QWidget::hideEvent(event);
  kDebug() << "Hidden";
  ui.wgVolumeWidget->stop();
}
