//
// C++ Implementation: inlinewidget
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "inlinewidget.h"
#include <QSettings>

InlineWidget::InlineWidget(QString title, QIcon icon, QString desc, QWidget* parent) 
	: QWidget(parent)
{
	this->title = title;
	this->icon = icon;
	this->desc = desc;
}


InlineWidget::~InlineWidget()
{
}


void InlineWidget::accept()
{
	emit accepted();
}

void InlineWidget::reject()
{
	emit reject();
}

bool InlineWidget::close()
{
	emit closed();
	return QWidget::close();
}

void InlineWidget::setVisible(bool visible)
{
	emit settingVisible(visible);
	QWidget::setVisible(visible);
}

void InlineWidget::hide()
{
	emit hidden();
	QWidget::hide();
}

void InlineWidget::show()
{
	emit shown();
	QWidget::show();
}


bool InlineWidget::exec()
{
	emit execd();
	QWidget::show();
}