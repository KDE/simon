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


// #include <qstandardpaths.h>

#include "osd.h"
#include <QString>
#include <QIcon>
#include <QPixmap>
#include <QTimer>
#include <QLabel>
#include <QPalette>
#include <QApplication>
#include <QDesktopWidget>
#include <QMouseEvent>
#include <QStandardPaths>
#include <KI18n/klocalizedstring.h>

/**
 * @brief Constructor
 *
 * Creates a new OSD object. It
 * 	initializes the timer (member)
 *		sets its Timeout to the argument
 *		links the Timeout-event to close() function of QWidget
 *		starts the timer
 * 	initializes the Label (member)
 * 		sets its Text according to the argument
 * 		stets its size and position
 *	sets the Widget geometry
 *		uses a QDesktopWidget to get the desktops geometry
 *	sets the Palette of the Widget to simon-Colors
 *
 * @author Peter Grasch
 */
OSD::OSD(QString message_, int timeout, QIcon* icon_)
: QLabel(0, Qt::WindowStaysOnTopHint|Qt::ToolTip|Qt::FramelessWindowHint),
timer(new QTimer(this)),
message(new QLabel(this)),
icon(new QLabel(this))
{
  setPixmap(QPixmap(QStandardPaths::locate(QStandardPaths::GenericDataLocation, "simoninfo/themes/default/osd.png")));

  QDesktopWidget* tmp = QApplication::desktop();

  int x,y;

  x=(tmp->width()/2) - 175;
  y=(tmp->height())-120;

  this->setGeometry(x, y,350,80);
  connect(this->timer, SIGNAL(timeout()), this, SLOT(deleteLater()));

  QPalette pal = palette();
  pal.setColor(QPalette::WindowText, QColor(255, 255, 255, 255));
  setPalette(pal);

  this->message->setGeometry(86, 10, 255, 60);
  this->message->setAlignment(Qt::AlignCenter | Qt::AlignLeft);
  this->message->setText(message_);
  this->message->setWordWrap(true);
  this->message->show();

  this->timer->start(timeout);

  this->icon->setGeometry(16, 16, 48, 48);

  if (icon_) {
    this->icon->setPixmap(icon_->pixmap(48));
  }
  else {
    this->icon->setPixmap(QIcon::fromTheme("simon").pixmap(QSize(48,48)));
  }

  show();
}


/**
 * \brief Closes the info on mouse-press
 * @param event The mousevent to comply with qts definition
 */
void OSD::mouseReleaseEvent(QMouseEvent *event)
{
  QWidget::mouseReleaseEvent(event);
  deleteLater();
}

