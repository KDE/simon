/*
 *  Copyright (C) 2010 Peter Grasch <peter.grasch@bedahr.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2,
 *  or (at your option) any later version, as published by the Free
 *  Software Foundation
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details
 *
 *  You should have received a copy of the GNU General Public
 *  License along with this program; if not, write to the
 *  Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "samui.h"
#include <KCmdLineArgs>
#include <QDebug>
#include <KWidgetsAddons/KMessageBox>

void SamUi::error(const QString& message)
{
  if (batchMode()) 
  {
    qWarning() << message;
  } else
    KMessageBox::sorry(reinterpret_cast<QWidget*>(this), message);
}

void SamUi::fatalError(const QString& message)
{
  if (batchMode()) 
  {
    qWarning() << message;
    exit(1);
  } else
    KMessageBox::error(reinterpret_cast<QWidget*>(this), message);
}

bool SamUi::batchMode()
{
  return KCmdLineArgs::parsedArgs()->isSet("b");
}
