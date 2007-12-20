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
#include <QFont>

/**
 * \brief Constructor
 * \author Peter Grasch
 * @param title The title of the widget
 * @param icon The icon to display for the widget
 * @param desc A short description (mainly used as tooltip)
 * @param parent the parent of the widget
 */
InlineWidget::InlineWidget(QString title, QIcon icon, QString desc, QWidget* parent) 
	: QWidget(parent)
{
	this->title = title;
	this->icon = icon;
	this->desc = desc;
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
