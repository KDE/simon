/*
 *   Copyright (C) 2008 Phillip Goriup <goriup@simon-listens.org>
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

#ifndef SIMON_TEXTLISTWIDGET_H_B252B690AAEE4A4D883FF6DB45979D2F
#define SIMON_TEXTLISTWIDGET_H_B252B690AAEE4A4D883FF6DB45979D2F

#include <KListWidget>
#include <KDebug>
#include "simonuicomponents_export.h"

class SIMONUICOMPONENTS_EXPORT  TextListWidget : public KListWidget
{
  Q_OBJECT
  /**
   * \brief Define the current text as a property for use in wizards
   */
    Q_PROPERTY(QString currentText READ currentText)
    Q_PROPERTY(QVariant currentUserData READ currentUserData)

    public:
    TextListWidget(QWidget *parent = 0);
    ~TextListWidget();

    /**
     * \brief Quick access function to retrieve the currently selected text
     * @return The currently selected text (or a new QString object) if there is none selected
     */
    QString currentText() {
      QListWidgetItem *selected = currentItem();
      if (!selected) return QString();
      return selected->text();
    }

    QVariant currentUserData() {
      if (!currentItem()) return QVariant();
      return currentItem()->data(Qt::UserRole);
    }
};
#endif
