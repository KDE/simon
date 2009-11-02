/*
 *   Copyright (C) 2008 Peter Grasch <grasch@simon-listens.org>
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

#ifndef SCREENGRID_H
#define SCREENGRID_H

#include <QWidget>
#include <QList>
#include <simonactions/greedyreceiver.h>

class KPushButton;
class QGridLayout;
class QLabel;
class DesktopGridConfiguration;
/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class ScreenGrid : public QWidget, public GreedyReceiver
{
Q_OBJECT

private slots:
	void regionSelected();

private:
	DesktopGridConfiguration *config;
	QList<KPushButton*> btns;
	QGridLayout *buttons;

	QLabel *background;
	QPixmap deskShot;

	QPixmap makeFakeTransparency();
	static QStringList numberIdentifiers;

public slots:
	bool greedyTrigger(const QString& input);


public:
    ScreenGrid(DesktopGridConfiguration* _config, QWidget* parent=0);
    void keyPressEvent(QKeyEvent *event);
    void setButtonFontSize(KPushButton *btn);

    ~ScreenGrid();

};

#endif
