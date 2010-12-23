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

SpeechCal::SpeechCal() : view( new SpeechCalView() )
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
  
//   headerItems.clear();
//   retrievedItems.clear();
  
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
//     itemFetcher->fetchScope(). setContentMimeTypes(QStringList() << "application/x-vnd.akonadi.calendar.event");
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
  
  foreach (const Akonadi::Item& i, items)
  {
    EventPtr event;
    if (!i.isValid()) 
      continue;
    
    try
    {
      event = i.payload<EventPtr>();
    }
    catch (std::exception& e)
    {
      Logger::log(i18n("Fetched event has wrong type and could not be deserialized"), Logger::Error);
      continue;
    }

    KDateTime startDate = event->dtStart();
    KDateTime endDate = event->dtEnd();
    if ((startDate > toDate) || (endDate < fromDate))
    continue;

    kDebug() << "Relevant: " << event->summary();
  }
  
  //at least one item in it and all are sharing the same collection
//   Akonadi::Entity::Id id = items[0].storageCollectionId();
//   headerItems.insert(id, items);
  
//   dissect(items);
}

/*
void SpeechCal::dissect(Akonadi::Item::List list, int lowerBound, int upperBound)
{
  if (list.isEmpty())
  {
    Logger::log(i18n("Ignoring dissect request on empty list"));
    return;
  }
  int size = list.count();
  if (upperBound != -1)
    size = upperBound;
  
  size -= lowerBound;
  int middleIndex = lowerBound + (size / 2);
  
  Akonadi::ItemFetchJob *fetchJob = new Akonadi::ItemFetchJob(list[middleIndex], this);
  fetchJob->fetchScope().fetchFullPayload();
  
  connect( fetchJob, SIGNAL( result( KJob* ) ), SLOT( itemFetchDone( KJob* ) ) );

}
void SpeechCal::itemFetchDone(KJob* job)
{
//   kDebug() << "Fetched full item";
  Akonadi::ItemFetchJob *fetchJob = static_cast<Akonadi::ItemFetchJob*>( job );
  if ( job->error() ) {
    Logger::log(i18n("Couldn't fetch item: %1", job->errorString()), Logger::Error);
    kError() << job->errorString();
    return;
  }

  if ( fetchJob->items().isEmpty() ) {
    kWarning() << "Job did not retrieve any items";
    Logger::log(i18n("Fetch didn't find any items: %1", job->errorString()), Logger::Warning);
    return;
  }

  Akonadi::Item event = fetchJob->items().first();
  if ( !event.isValid() ) {
      kWarning() << "Item not valid";
      Logger::log(i18n("Fetched item is invalid"), Logger::Warning);
      return;
  }

  EventPtr realEvent;
  try
  {
    realEvent = event.payload<EventPtr>();
  }
  catch (std::exception& e)
  {
    Logger::log(i18n("Fetched event has wrong type and could not be deserialized"), Logger::Error);
    return;
  }
  
  KDateTime startDate = realEvent->dtStart();
  KDateTime endDate = realEvent->dtEnd();
  
  kDebug() << realEvent->summary() << startDate << endDate << fromDate << toDate;

  Akonadi::Item::List headers = headerItems.value(event.storageCollectionId());
  bool internalCorruption = false;
  if (endDate < fromDate)
  {
    kDebug() << "Removing earlier...";
    //remove all earlier ids and re-start dissect on rest of list
    internalCorruption = true;
    foreach (const Akonadi::Item& i, headers)
    {
      headers.removeAll(i);
      if (i.id() == event.id())
      {
	internalCorruption = false;
	break;
      }
    }
  }
  
  if (startDate > toDate)
  {
    kDebug() << "Removing later...";
    //remove all later ids
    int i=0;
    while ((i < headers.count()) && (headers[i].id() != event.id()))
      i++;
    if (i == headers.count())
      internalCorruption = true;
    else {
      for (int j=i; j < headers.count(); j++)
	headers.removeAt(j);
    }
  }
  
  if (internalCorruption)
  {
    headerItems.remove(event.storageCollectionId());
    Logger::log(i18n("Internal corruption; ID not available in header of collection."), Logger::Error);
  } else {
    headerItems.insert(event.storageCollectionId(), headers);
    dissect(headers);
  }
}
*/


SpeechCal::~SpeechCal()
{

}

