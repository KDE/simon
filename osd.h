//
// C++ Interface: osd
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef OSD_H
#define OSD_H

#include <QSplashScreen>
#include <QString>
#include <QPainter>
#include <QPixmap>
#include <QTimer>
#include <QLabel>
#include <QPalette>
#include <QApplication>
#include <QDesktopWidget>
#include <QMouseEvent>

/**
 *	@class OSD
 *	@brief On Screen Dispay - displays quick messages on the screen
 *
 *	Uses a passive (and non-modal) dialog to display quick messages 
 *	on the screen
 *
 *	@version 0.1
 *	@date 23.01.2006
 *	@author Peter Grasch
 *	@todo Maybe implement to close the message when clicking on it
 */
class OSD : public QWidget
{
	Q_OBJECT
private:
	QTimer *timer;
	QLabel *message;
public:
	OSD(QString message, int timeout, int alignment=Qt::AlignBottom);
	void paintEvent(QPaintEvent *e);
	

    ~OSD();

};

#endif
