//
// C++ Implementation: osd
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
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
#include <KLocalizedString>
#include "../Logging/logger.h"

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
OSD::OSD(QString message, int timeout, QIcon *icon)
	: QLabel(0, Qt::WindowStaysOnTopHint|Qt::ToolTip|Qt::FramelessWindowHint)
{
	setPixmap(QPixmap(":/images/osd.png"));
	Logger::log(i18n("[INF] ")+message);
	
	this->timer = new QTimer();
	
	QDesktopWidget* tmp = QApplication::desktop();

	int x,y;
	
	x=(tmp->width()/2) - 175;
	y=(tmp->height())-120;
	
	this->setGeometry(x, y,350,80);
	connect(this->timer, SIGNAL(timeout()), this, SLOT(deleteLater()));
	
	QPalette pal = palette();
	pal.setColor(QPalette::WindowText, QColor(255, 255, 255, 255));
	setPalette(pal);
	
	this->message = new QLabel(this);
	this->message->setGeometry(86, 10, 255, 60);
	this->message->setAlignment(Qt::AlignCenter | Qt::AlignLeft);
	this->message->setText(message);
	this->message->setWordWrap(true);
	this->message->show();
	
	this->timer->start(timeout);

	this->icon = new QLabel(this);
	this->icon->setGeometry(16, 16, 48, 48);
	if (!icon)
	{
		this->icon->setPixmap(QPixmap(":/images/tray.png"));
	}
	else {
		this->icon->setPixmap(icon->pixmap(48));
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

/**
 * @brief Overwritten paintEvent
 *
 * Overloads the Widgets paint-Function to draw the background-Pixmap
 *
 * @author Peter Grasch
 */
// void OSD::paintEvent( QPaintEvent *e )
// {
// 	QPainter painter(this);
// 	QPixmap bg = QPixmap(":/images/osd.png");
// 	painter.drawPixmap(0,0,bg);
// 	QWidget::paintEvent(e);
// }

/**
 * \brief Destructor
 * \author Peter Grasch
 */
OSD::~OSD()
{
    timer->deleteLater();
    message->deleteLater();
    icon->deleteLater();
}


