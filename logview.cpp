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

LogView::LogView(QWidget* parent): SystemWidget(tr("Protokoll"), QIcon(":/images/icons/text-editor.svg"), tr("Hier können Sie die letzten Aktionen von simon überprüfen"), parent)
{
}


LogView::~LogView()
{
}


