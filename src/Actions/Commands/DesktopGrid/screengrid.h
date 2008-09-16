//
// C++ Interface: screengrid
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef SCREENGRID_H
#define SCREENGRID_H

#include <QWidget>

class KPushButton;
class QGridLayout;
class QLabel;
/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class ScreenGrid : public QWidget
{
Q_OBJECT

private slots:
	void regionSelected();

private:
	QGridLayout *buttons;

	QLabel *background;
	QPixmap deskShot;

	QPixmap makeFakeTransparency();


public:
    ScreenGrid(QWidget* parent=0);
    void keyPressEvent(QKeyEvent *event);
    void setButtonFontSize(KPushButton *btn);

    ~ScreenGrid();

};

#endif
