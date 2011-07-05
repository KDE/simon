/*   Copyright (C) 2011 Grasch Peter <grasch@simon-listens.org>
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

#ifndef SIMONSTATUSWIDGET_H
#define SIMONSTATUSWIDGET_H

#include <QLabel>
#include <QTimer>

class SimonStatusWidget : public QLabel
{
Q_OBJECT
Q_CLASSINFO("A simon status widget", "org.simon-listens.SimonStatusWidget")

private:
  QTimer timeoutTimer;
  bool singleShotActive;
  
  void showActive();
  void showInactive();
  void disableSingleShot();
  
public:
  explicit SimonStatusWidget(QWidget* parent = 0, Qt::WindowFlags f = 0);
    
public slots:
    void setActive();
    void setInactive();
    
    void setActiveOnce(int timeout);
    void recognizedSomething();
};

#endif // SIMONSTATUSWIDGET_H
