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

class QPushButton;
class QGridLayout;
class QLabel;
/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class ScreenGrid : public QWidget
{
Q_OBJECT

signals:
	void click(int x, int y);
private slots:
	void regionSelected();
private:
	QGridLayout *buttons;

// #ifdef FAKETRANSPARENCY
	QLabel *background;
	QPixmap deskShot;
// #endif

	QPixmap makeFakeTransparency();
public:
    ScreenGrid(QWidget* parent=0);
    void keyPressEvent(QKeyEvent *event);
    void setButtonFontSize(QPushButton *btn);

    ~ScreenGrid();

};

#endif
