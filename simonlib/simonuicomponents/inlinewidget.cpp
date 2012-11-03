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

#include "inlinewidget.h"
#include <QFont>

/**
 * \brief Constructor
 * \author Peter Grasch
 * @param title The title of the widget
 * @param icon The icon to display for the widget
 * @param desc A short description (mainly used as tooltip)
 * @param parent the parent of the widget
 */
InlineWidget::InlineWidget(QString title_, KIcon icon_, QString desc_, QWidget* parent)
: QWidget(parent),
title(title_),
desc(desc_),
icon(icon_)
{
}

bool InlineWidget::isShown()
{
  if (this->isVisible())
    return true;
  return false;
}

/**
 * \brief Accept the InlineWidget
 * \author Peter Grasch
 */
void InlineWidget::accept()
{
  emit accepted();
}


/**
 * \brief Rejects the "dialog
 * \author Peter Grasch
 */
void InlineWidget::reject()
{
  emit rejected();
}

/**
 * \brief Wrapper for the close method - emits closed()
 * \author Peter Grasch
 */
bool InlineWidget::close()
{
  emit closed();
  return QWidget::close();
}

/**
 * \brief Sets the visibility to the given bool emits shown() / hidden()
 * \author Peter Grasch
 * @param visible sets the widget tot his state
 */
void InlineWidget::setVisible(bool visible)
{
  QWidget::setVisible(visible);
  if (visible)
    emit shown();
  else emit hidden();
}
