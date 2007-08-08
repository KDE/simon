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
#include "logger.h"

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
 * @todo Ignores the alignment-argument for now
 */
OSD::OSD(QString message, int timeout, int alignment)
	: QWidget(0, Qt::WindowStaysOnTopHint|Qt::ToolTip|Qt::FramelessWindowHint)
{
	Logger::log(tr("[INF] ")+message);
	
	this->timer = new QTimer();
	
	QDesktopWidget* tmp = QApplication::desktop();
	
	int x,y;
	
	x=(tmp->width()/2) - 125;
	y=(tmp->height())-130;
	
	this->setGeometry(x, y,250,60);
	connect(this->timer, SIGNAL(timeout()), this, SLOT(close()));
	
	QPalette pal = palette();
	pal.setColor(QPalette::WindowText, QColor(175, 190, 200, 255));
	setPalette(pal);
	
	this->message = new QLabel(this);
	this->message->setGeometry(57, 10, 185, 40);
	this->message->setAlignment(Qt::AlignCenter | Qt::AlignLeft);
	this->message->setText(message);
	this->message->setWordWrap(true);
	this->message->show();
	
	this->timer->start(timeout);
	
	show();
}

/**
 * @brief Overwritten paintEvent
 *
 * Overloads the Widgets paint-Functio to draw the background-Pixmap
 *
 * @author Peter Grasch
 */
void OSD::paintEvent( QPaintEvent *e )
{
	QPainter painter(this);
	QPixmap bg = QPixmap(":/images/osd.png");
	painter.drawPixmap(0,0,bg);
}

OSD::~OSD()
{
}


