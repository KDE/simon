/*   Copyright (C) 2009-2010 Grasch Peter <peter.grasch@bedahr.org>
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

#include "turnconfiguration.h"
#include <simondialogengine/dialogturn.h>

#include <simonscenarios/scenario.h>

#include "createdialogcommandwidget.h"
#include "createtransitiondialog.h"

#include <QVariantList>
#include <QList>
#include <QString>
#include <QDialog>
#include <QTableView>
#include <QThread>
#include <QApplication>

#include <kgenericfactory.h>
#include <KAboutData>
#include <KInputDialog>
#include <KMessageBox>
#include <KStandardDirs>

TurnConfiguration::TurnConfiguration(DialogState* _state, QWidget* parent) :
  state(_state)
{
  ui.setupUi(this);
}

void TurnConfiguration::addPrompt()
{
}

void TurnConfiguration::removePrompt()
{
}

void TurnConfiguration::editPrompt()
{
}

void TurnConfiguration::textSilenceChanged()
{
}

void TurnConfiguration::textAnnounceRepeatChanged()
{
}

void TurnConfiguration::addExtractor()
{
}

void TurnConfiguration::removeExtractor()
{
}

void TurnConfiguration::init()
{
}

bool TurnConfiguration::deSerialize(const QDomElement&)
{
  return true;
}

QDomElement TurnConfiguration::serialize(QDomDocument *doc)
{
  return QDomElement();
}

void TurnConfiguration::defaults()
{
}

TurnConfiguration::~TurnConfiguration()
{
}

QString TurnConfiguration::getRepeatAnnouncement() const
{
  return QString();
}

QStringList TurnConfiguration::getRepeatTriggers() const
{
  return QStringList();
}

bool TurnConfiguration::getRepeatOnInvalidInput() const
{
  return true;
}
