//
// C++ Interface: inlinewidget
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef INLINEWIDGET_H
#define INLINEWIDGET_H

#include <QWidget>
#include <QIcon>

#include "simonwidget.h"

class QString;
class QIcon;
class QSettings;

/**
 * \class InlineWidget
 * \brief Provides a widget to display inline in the simon mainwindow
 * \note Please use this instead of the QDialog class whenever feasable!
 * \author Peter Grasch
 * \version 0.1
 * \date 10.8.2007
*/
class InlineWidget : public QWidget, public SimonWidget
{
Q_OBJECT

private:
	QString title, desc;
	QIcon icon;
	GuiEvents *guievents;
	//SimonControl *control;

signals:
	void closed();
	void rejected();
	void hidden();
	void execd();
	void shown();
	void accepted();

public:
	InlineWidget(QString title, QIcon icon, QString desc, QWidget* parent=0);
		
	~InlineWidget();

	void setVisible(bool visible);

	QString getTitle() const {return title; }
	QIcon getIcon() const { return icon; }
	QString getDesc() const { return desc; }
	bool isShown();

public slots:
	void accept();
	void reject();
	bool close();
	bool exec();

	void registerControl(QString trigger, QObject* receiver, const char* slot);
	void doAction(QString action);

	QObjectList getChildren(QObject *current);

};

#endif
