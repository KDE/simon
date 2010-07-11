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

#ifndef SIMON_SELECTIONLISTVIEW_H_EF32AB08882C4BCA803C7B5C1522AFE2
#define SIMON_SELECTIONLISTVIEW_H_EF32AB08882C4BCA803C7B5C1522AFE2

#include "simonuicomponents_export.h"
#include <QListView>

/**
 \class SelectionListView
 \author Peter Grasch

 \brief Adds the selectionChanged() signal
*/

class QItemSelection;

class SIMONUICOMPONENTS_EXPORT SelectionListView : public QListView
{
  Q_OBJECT

    signals:
  void selectionChanged();

  public:
    explicit SelectionListView(QWidget* parent=0);

  protected slots:
    virtual void selectionChanged (const QItemSelection& , const QItemSelection& );
};
#endif
