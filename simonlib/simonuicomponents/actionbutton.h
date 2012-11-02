/*
 *   Copyright (C) 2012 Peter Grasch <peter.grasch@bedahr.org>
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


#ifndef SIMON_ACTIONBUTTON_H_279F7C1C95C94FFEA3A7CC7B1B1A2B9F
#define SIMON_ACTIONBUTTON_H_279F7C1C95C94FFEA3A7CC7B1B1A2B9F

#include <KPushButton>
#include "simonuicomponents_export.h"

class QAction;

class SIMONUICOMPONENTS_EXPORT ActionButton : public KPushButton
{
  Q_OBJECT
public:
  ActionButton(QWidget *parent=0);
  void setAction(QAction *action);
  
private slots:
  void updateUi();
  
private:
  QAction *m_action;
};

#endif
