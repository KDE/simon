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

#include <QLabel>

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
 */
class QMouseEvent;
class QTimer;
class QLabel;
class QMouseEvent;
class KIcon;

class OSD : public QLabel
{
	Q_OBJECT
private:
	QTimer *timer;
	QLabel *message;
	QLabel *icon;

protected:
	void mouseReleaseEvent ( QMouseEvent * );
public:
	OSD(QString message, int timeout, KIcon *icon=NULL);
// 	void paintEvent(QPaintEvent *e);
	

    ~OSD();

};

#endif
