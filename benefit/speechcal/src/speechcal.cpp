/*   Copyright (C) 2010 Grasch Peter <grasch@simon-listens.org>
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

#include "speechcal.h"
#include "speechcalview.h"
#include "calendarmodel.h"
#include <simonlogging/logger.h>
#include <exception>

#include <QSharedPointer>
#include <KLocale>

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

typedef QSharedPointer<KCalCore::Event> EventPtr;

SpeechCal::SpeechCal() : view( new SpeechCalView() ), calendar(new CalendarModel(this))
{
  if (!Logger::init())
  {
    kError() << "Couldn't initialize logging";
    qApp->exit( -1 );
    return;
    
  }
  if ( !Akonadi::Control::start( view ) ) {
    Logger::log(i18n("Failed to contact akonadi.", Logger::Error));
    qApp->exit( -1 );
    return;
  }
  setupCollections();
  view->displayModel(calendar);
}

void SpeechCal::exec()
{
  view->show();
}

void SpeechCal::setupCollections()
{
  // Show events starting up to 30 mins ago to account for delays
  fromDate = KDateTime(QDateTime::currentDateTime().addSecs(-1800));
  toDate = KDateTime(QDateTime::currentDateTime().addDays(1)); //next 24 hours
  
  Akonadi::CollectionFetchJob *job = new Akonadi::CollectionFetchJob( Akonadi::Collection::root(), Akonadi::CollectionFetchJob::Recursive, this );
  connect(job, SIGNAL(collectionsReceived(Akonadi::Collection::List)), this, SLOT(slotCollectionsReceived(Akonadi::Collection::List)));
  job->fetchScope().setContentMimeTypes(QStringList() << "application/x-vnd.akonadi.calendar.event");
}

void SpeechCal::slotCollectionsReceived(Akonadi::Collection::List list)
{
  foreach (const Akonadi::Collection &collection, list)
  {
    kDebug() << "Received collection: " << collection.name();
    Logger::log(i18n("Received collection: %1", collection.name()));
    Akonadi::ItemFetchJob *itemFetcher = new Akonadi::ItemFetchJob(collection, this);
    itemFetcher->fetchScope().fetchFullPayload();
    connect(itemFetcher, SIGNAL(itemsReceived(Akonadi::Item::List)), this, SLOT(slotItemsReceived(Akonadi::Item::List)));
  }
}

void SpeechCal::slotItemsReceived(Akonadi::Item::List items)
{
  if (items.isEmpty())
  {
    kDebug() << "Ignoring empty fetch result";
    return;
  }
  Logger::log(i18n("Retrieved %1 items from collection", items.count()));
  
  QList< QSharedPointer<KCalCore::Event> > relevantItems;
  foreach (const Akonadi::Item& i, items)
  {
    QSharedPointer<KCalCore::Event> event;
    if (!i.isValid()) 
      continue;
    
    try
    {
      event = i.payload< QSharedPointer<KCalCore::Event> >();
    }
    catch (std::exception& e)
    {
      Logger::log(i18n("Fetched event has wrong type and could not be deserialized (Payload: %1)", QString::fromAscii(i.payloadData())), Logger::Error);
      continue;
    }

    KDateTime startDate = event->dtStart();
    KDateTime endDate = event->dtEnd();
    if ((startDate > toDate) || (endDate < fromDate))
    continue;

    relevantItems << event;
  }
  Logger::log(i18n("Retrieved %1 relevant items", relevantItems.count()));
  calendar->initialize(relevantItems);
}


SpeechCal::~SpeechCal()
{

}

