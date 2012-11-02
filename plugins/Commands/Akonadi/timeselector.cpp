/*   Copyright (C) 2010 Grasch Peter <peter.grasch@bedahr.org>
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

#include "timeselector.h"
#include "akonadicommandmanager.h"

TimeSelector::TimeSelector(QWidget* parent, Qt::WindowFlags f): QWidget(parent, f)
{
  ui.setupUi(this);
}

void TimeSelector::setTime(int seconds)
{
  kDebug() << "Requesting time: " << seconds;
  AkonadiCommand::RelativeDurationDimension dimension;
  int value;
  getRelativeTime(seconds, dimension, value);
  kDebug() << "Result: " << dimension << value;
  setTime(dimension, value);
}

void TimeSelector::getRelativeTime(int seconds, AkonadiCommand::RelativeDurationDimension& dimension, int& value)
{
  if (seconds % (24*60*60) == 0)
  {
    dimension = AkonadiCommand::Days;
    value = seconds /  (24*60*60);
    return;
  }
  if (seconds % (60*60) == 0)
  {
    dimension = AkonadiCommand::Hours;
    value = seconds /  (60*60);
    return;
  }
  if (seconds % (60) == 0)
  {
    dimension = AkonadiCommand::Minutes;
    value = seconds /  (60);
    return;
  }
  
  dimension = AkonadiCommand::Seconds;
  value = seconds;
}

int TimeSelector::getTime() const
{
  int value;
  switch (ui.cbDurationType->currentIndex())
  {
    case 0: //seconds
      value = ui.sbRelativeDuration->value();
      break;
    case 1: //minutes
      value = ui.sbRelativeDuration->value()*60;
      break;
    case 2: //hours
      value = ui.sbRelativeDuration->value()*60*60;
      break;
    default: //days
      value = ui.sbRelativeDuration->value()*60*60*24;
      break;
  }
  return value;
}
void TimeSelector::setTime(AkonadiCommand::RelativeDurationDimension dimension, int value)
{
  ui.sbRelativeDuration->setValue(value);
  switch (dimension)
  {
    case AkonadiCommand::Seconds:
      ui.cbDurationType->setCurrentIndex(0);
      break;
    case AkonadiCommand::Minutes:
      ui.cbDurationType->setCurrentIndex(1);
      break;
    case AkonadiCommand::Hours:
      ui.cbDurationType->setCurrentIndex(2);
      break;
    case AkonadiCommand::Days:
      ui.cbDurationType->setCurrentIndex(3);
      break;
  }
}
