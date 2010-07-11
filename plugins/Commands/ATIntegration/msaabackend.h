/*
 *   Copyright (C) 2008 Susanne Tschernegg <tschernegg@simon-listens.org>
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

#ifndef SIMON_MSAABACKEND_H_FF3078EF80914CBD943C9D5B1D6B7BC7
#define SIMON_MSAABACKEND_H_FF3078EF80914CBD943C9D5B1D6B7BC7

#include "atbackend.h"

#include "ato.h"
#include "atobject.h"
#include <QStringList>

/**
  @author Peter Grasch <bedahr@gmx.net>
*/
class MSAABackend : public ATBackend
{
  Q_OBJECT

    private:
    QStringList windowNameList;

  public:
    MSAABackend ( QObject* parent );
    void startMonitoring();
    void stopMonitoring();

    QStringList getWindowNames();
    QString getWindowClassName(QString windowName);
    QString getForegroundWindowName();            //HWND GetForegroundWindow(VOID);
    ATOLocation getLocation(QString windowName);

    ~MSAABackend();

};
#endif
