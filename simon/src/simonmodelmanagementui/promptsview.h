/*
 *   Copyright (C) 2012 Peter Grasch <grasch@simon-listens.org>
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

#ifndef SIMON_PROMPTSVIEW_H_47AEB5F97F5246E8BAC891DD0B65687A
#define SIMON_PROMPTSVIEW_H_47AEB5F97F5246E8BAC891DD0B65687A

#include <KDialog>
#include "simonmodelmanagementui_export.h"

class PromptsViewPrivate;

class SIMONMODELMANAGEMENTUI_EXPORT PromptsView : public KDialog
{
  Q_OBJECT

  public:
    PromptsView(QWidget *parent=0);

  private:
    PromptsViewPrivate *d;
};
#endif
