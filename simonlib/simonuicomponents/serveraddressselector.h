/*
 *   Copyright (C) 2008 Peter Grasch <peter.grasch@bedahr.org>
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

#ifndef SIMON_SERVERADDRESSSELECTOR_H_248323A2BAC9490395A6BC14E290B274
#define SIMON_SERVERADDRESSSELECTOR_H_248323A2BAC9490395A6BC14E290B274

#include <QWidget>
#include "simonuicomponents_export.h"

class KLineEdit;
class QToolButton;

class SIMONUICOMPONENTS_EXPORT ServerAddressSelector : public QWidget
{
  Q_OBJECT
    private:
    KLineEdit *leServerAddress;
    QToolButton *pbSelectServerAddress;

  private slots:
    void displayAddDialog();
  public:
    ServerAddressSelector(QWidget *parent);

    KLineEdit* lineEdit();

    ~ServerAddressSelector();

};
#endif
