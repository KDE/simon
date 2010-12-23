/* 
 *  Copyright (C) 2010 Peter Grasch <grasch@simon-listens.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2,
 *  or (at your option) any later version, as published by the Free
 *  Software Foundation
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details
 *
 *  You should have received a copy of the GNU General Public
 *  License along with this program; if not, write to the
 *  Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "speechcalview.h"
#include <akonadi/control.h>
#include <akonadi/collection.h>
#include <akonadi/itemmodel.h>
#include <akonadi/collectionfetchjob.h>
#include <akonadi/collectionfetchscope.h>
#include <akonadi/itemfetchjob.h>
#include <akonadi/itemfetchscope.h>
#include <akonadi/changerecorder.h>
#include <kcalcore/incidence.h>
#include <kcalcore/event.h>


SpeechCalView::SpeechCalView(QWidget* parent, Qt::WFlags flags) : KXmlGuiWindow(parent, flags)
{
  ui.setupUi(this);
  if ( !Akonadi::Control::start( this ) ) {
    kWarning() << "Couldn't start akonadi control";
    qApp->exit( -1 );
    return;
  }
}

SpeechCalView::~SpeechCalView()
{
}