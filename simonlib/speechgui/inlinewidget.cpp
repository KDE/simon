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


#include "inlinewidget.h"
#include <QFont>

/**
 * \brief Constructor
 * \author Peter Grasch
 * @param title The title of the widget
 * @param icon The icon to display for the widget
 * @param desc A short description (mainly used as tooltip)
 * @param parent the parent of the widget
 */
InlineWidget::InlineWidget(QString title, KIcon icon, QString desc, QWidget* parent) 
	: QWidget(parent)
{
	this->title = title;
	this->icon = icon;
	this->desc = desc;
	guievents = new GuiEvents;
}


void InlineWidget::registerControl(QString trigger, QObject* receiver, const char* slot)
{
	guievents->registerControl(trigger, receiver, slot);
}


void InlineWidget::doAction(QString action)
{
	if(isShown())
	{
		guievents->doAction(action,(QObject*)this);
	}
}

QObjectList InlineWidget::getChildren(QObject *current)
{
	return current->children();
}


bool InlineWidget::isShown()
{
	if (this->isVisible())
		return true;
	return false;
}



/**
 * \brief Destructor
 * \author Peter Grasch
 */
InlineWidget::~InlineWidget()
{
}


/**
 * \brief Accept the InlineWidget
 * \author Peter Grasch
 */
void InlineWidget::accept()
{
	emit accepted();
}

/**
 * \brief Rejects the "dialog
 * \author Peter Grasch
 */
void InlineWidget::reject()
{
	emit reject();
}

/**
 * \brief Wrapper for the close method - emits closed()
 * \author Peter Grasch
 */
bool InlineWidget::close()
{
	emit closed();
	return QWidget::close();
}

/**
 * \brief Sets the visibility to the given bool emits shown() / hidden()
 * \author Peter Grasch
 * @param visible sets the widget tot his state 
 */
void InlineWidget::setVisible(bool visible)
{
	QWidget::setVisible(visible);
	if (visible)
		emit shown();
	else emit hidden();
}


/**
 * \brief "Executes" the "dialog"
 * \author Peter Grasch
 * @return accepted or rejected?
 */
bool InlineWidget::exec()
{
	emit execd();
	QWidget::show();
	return true;
}
