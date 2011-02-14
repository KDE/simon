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
#include <KGlobal>

#include <akonadi/control.h>
#include <akonadi/collection.h>
#include <akonadi/monitor.h>
#include <akonadi/collectionfetchjob.h>
#include <akonadi/collectionfetchscope.h>
#include <akonadi/itemfetchjob.h>
#include <akonadi/itemfetchscope.h>

#include <kcalcore/incidence.h>
#include <kcalcore/event.h>


SpeechCal::SpeechCal() : calendar(new CalendarModel(this))
{
  view = new SpeechCalView(calendar, this);
  
  displayDate = KDateTime(QDateTime::currentDateTime());
  
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
  
  monitor = new Akonadi::Monitor(this);
  monitor->setMimeTypeMonitored(KCalCore::Event::eventMimeType(), true);
  connect(monitor, SIGNAL(collectionAdded (const Akonadi::Collection &, const Akonadi::Collection &)),
	  this, SLOT(setupCollections()));
  connect(monitor, SIGNAL(collectionChanged (const Akonadi::Collection &collection)), this, SLOT(setupCollections()));
  connect(monitor, SIGNAL(itemAdded(Akonadi::Item,Akonadi::Collection)), this, SLOT(retrieveEvents()));
  connect(monitor, SIGNAL(itemChanged(Akonadi::Item,QSet<QByteArray>)), this, SLOT(retrieveEvents()));
  connect(monitor, SIGNAL(itemRemoved(Akonadi::Item)), this, SLOT(retrieveEvents()));
  connect(monitor, SIGNAL(itemMoved(Akonadi::Item,Akonadi::Collection,Akonadi::Collection)), this, SLOT(retrieveEvents()));
  
  setupCollections();
}

void SpeechCal::nextDay()
{
  displayDate = displayDate.addDays(1);
  retrieveEvents();
}
void SpeechCal::previousDay()
{
  displayDate = displayDate.addDays(-1);
  retrieveEvents();
}


void SpeechCal::exec()
{
  view->show();
}

void SpeechCal::setupCollections()
{
  Akonadi::CollectionFetchJob *job = new Akonadi::CollectionFetchJob( Akonadi::Collection::root(), Akonadi::CollectionFetchJob::Recursive, this );
  connect(job, SIGNAL(finished(KJob*)), this, SLOT(collectionJobFinished(KJob*)));
  job->fetchScope().setContentMimeTypes(QStringList() << KCalCore::Event::eventMimeType());
}

void SpeechCal::collectionJobFinished(KJob* job)
{
  Akonadi::CollectionFetchJob *fetchJob = static_cast<Akonadi::CollectionFetchJob*>( job );
  if ( job->error() ) {
      Logger::log(i18n("Job returned error: %1", job->errorString()), Logger::Error);
      kError() << job->errorString();
      return;
  }
  collectionsReceived(fetchJob->collections());
}

void SpeechCal::itemJobFinished(KJob* job)
{
  Akonadi::ItemFetchJob *fetchJob = static_cast<Akonadi::ItemFetchJob*>( job );
  if ( job->error() ) {
      Logger::log(i18n("Job returned error: %1", job->errorString()), Logger::Error);
      kError() << job->errorString();
      return;
  }

  itemsReceived(fetchJob->items());
}


void SpeechCal::collectionsReceived(Akonadi::Collection::List list)
{
  collectionList = list;
  retrieveEvents(); 
}
void SpeechCal::retrieveEvents()
{
  calendar->clear();
  view->updateDisplay(i18n("Loading..."));
  foreach (const Akonadi::Collection &collection, collectionList)
  {
    kDebug() << "Received collection: " << collection.name();
    Logger::log(i18n("Received collection: %1", collection.name()));
    Akonadi::ItemFetchJob *itemFetcher = new Akonadi::ItemFetchJob(collection, this);
    itemFetcher->fetchScope().fetchFullPayload();
    connect(itemFetcher, SIGNAL(finished(KJob*)), this, SLOT(itemJobFinished(KJob*)));
  }
}

void SpeechCal::itemsReceived(Akonadi::Item::List items)
{
  if (items.isEmpty())
  {
    kDebug() << "Ignoring empty fetch result";
    return;
  }
  Logger::log(i18n("Retrieved %1 items from collection", items.count()));


  QList< QSharedPointer<KCalCore::Event> > consideredItems;
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

    if (event->recurs()) {
      KCalCore::Recurrence *r = event->recurrence();
      KCalCore::DateTimeList list = r->timesInInterval(KDateTime(displayDate.date()), KDateTime(displayDate.addDays(1).date()));
      foreach (const KDateTime& recurrenceTime, list) {
        QSharedPointer<KCalCore::Event> copy(new KCalCore::Event(*event));
        KCalCore::Duration d = copy->duration();
        copy->setDtStart(recurrenceTime);
        copy->setDuration(d);
        consideredItems << copy;
      }
    } else
      consideredItems << event;
  }

  foreach (QSharedPointer<KCalCore::Event> event, consideredItems) {
    KDateTime startDate = event->dtStart();
    KDateTime endDate = event->dtEnd();
    if ((startDate.date() > displayDate.date()) || (endDate.date() < displayDate.date())) {
      continue;
    }

    //insert sorted
    int j=0;
    for (; j < relevantItems.count(); j++)
    {
      if (relevantItems[j]->dtStart() > event->dtStart())
      {
        relevantItems.insert(j, event);
        break;
      }
    }
    if (j == relevantItems.count())
      relevantItems << event;
  }
  Logger::log(i18n("Retrieved %1 relevant items", relevantItems.count()));
  calendar->addItems(relevantItems);
  updateView();
}

void SpeechCal::updateView()
{
  if (!view) return;
  
  QString dateString;
  dateString = KGlobal::locale()->formatDate(displayDate.date(), KLocale::LongDate);
  view->updateDisplay(dateString);
}

void SpeechCal::quit()
{
  view = 0;
  deleteLater();
}

SpeechCal::~SpeechCal()
{
  delete view;
  delete calendar;
  delete monitor;
}

