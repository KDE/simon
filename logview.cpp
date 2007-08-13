//
// C++ Implementation: logview
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "logview.h"
#include <QMessageBox>

LogView::LogView(QWidget* parent): SystemWidget(tr("Protokoll"), QIcon(":/images/icons/text-editor.svg"), tr("Hier können Sie die letzten Aktionen von simon überprüfen"), parent)
{
	ui.setupUi(this);
}

void LogView::enter()
{
// 	QMessageBox::information(this, "Hallo", "Bin grad reingangen");
}

void LogView::leave()
{
// 	QMessageBox::information(this, "Hallo", "Bin grad ausigangen");
}

LogView::~LogView()
{
	
}


bool LogView::apply()
{
	return true;
}
bool LogView::init()
{
	connect(this, SIGNAL(shown()), this, SLOT(enter()));
	connect(this, SIGNAL(hidden()), this, SLOT(leave()));
	return true;
}
bool LogView::reset()
{
	return true;
}

