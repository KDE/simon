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
#include <QObject>
#include <QIcon>

class QString;
class QIcon;
class QSettings;

/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class InlineWidget : public QWidget
{
Q_OBJECT

private:
	QString title, desc;
	QIcon icon;

signals:
	void closed();
	void rejected();
	void hidden();
	void execd();
	void shown();
	void accepted();
	void settingVisible(bool);

public:
    InlineWidget(QString title, QIcon icon, QString desc, QWidget* parent=0);

    ~InlineWidget();

	void setVisible(bool visible);

	QString getTitle() const {return title; }
	QIcon getIcon() const { return icon; }
	QString getDesc() const { return desc; }

public slots:
    void accept();
    void reject();
    bool close();
    bool exec();
    void hide();
    void show();

};

#endif
