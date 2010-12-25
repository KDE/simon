/*
 *   Copyright (C) 2010 Peter Grasch <grasch@simon-listens.org>
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
#include "akonadicommandmanager.h"
#include "akonadiconfiguration.h"
#include "akonadicommand.h"
#include "createakonadicommandwidget.h"
#include <eventsimulation/eventhandler.h>
#include <simonactions/actionmanager.h>
#include <simonlogging/logger.h>
#include <KLocalizedString>
#include <KAction>
#include <akonadi/control.h>
#include <akonadi/monitor.h>
#include <akonadi/collection.h>
#include <akonadi/itemfetchjob.h>
#include <akonadi/itemfetchscope.h>
#include <kcalcore/incidence.h>
#include <kcalcore/event.h>

K_PLUGIN_FACTORY( AkonadiCommandPluginFactory,
registerPlugin< AkonadiCommandManager >();
)

K_EXPORT_PLUGIN( AkonadiCommandPluginFactory("simonakonadicommand") )

AkonadiCommandManager::AkonadiCommandManager(QObject* parent, const QVariantList& args) : 
    CommandManager((Scenario*) parent, args)
{
  connect(&checkScheduleTimer, SIGNAL(timeout()), this, SLOT(checkSchedule()));
  
  akonadiMonitor = new Akonadi::Monitor(this);
  akonadiMonitor->setMimeTypeMonitored(KCalCore::Event::eventMimeType(), true);
  connect(akonadiMonitor, SIGNAL(itemAdded(Akonadi::Item,Akonadi::Collection)), this, SLOT(setupSchedule()));
  connect(akonadiMonitor, SIGNAL(itemChanged(Akonadi::Item,QSet<QByteArray>)), this, SLOT(setupSchedule()));
  connect(akonadiMonitor, SIGNAL(itemRemoved(Akonadi::Item)), this, SLOT(setupSchedule()));
  connect(akonadiMonitor, SIGNAL(itemMoved(Akonadi::Item,Akonadi::Collection,Akonadi::Collection)), this, SLOT(setupSchedule()));
}

void AkonadiCommandManager::checkSchedule()
{
  checkScheduleTimer.stop();
  QMap< QDateTime, QString >::iterator i = schedule.begin();
  while ((i != schedule.end()) && (i.key() < QDateTime::currentDateTime()))
  {
    QString command = *i;
    schedule.remove(i.key());
    i++;
    kDebug() << "Executing: " << command;
    QStringList parts = command.split("//");
    if (parts.count() != 2)
    {
      kWarning() << "Bad command format: " << command;
      Logger::log(i18n("Invalid akonadi command format: %1", command), Logger::Warning);
      continue;
    }
    bool succ = ActionManager::getInstance()->triggerCommand(parts[0], parts[1]);
    if (!succ)
      Logger::log(i18n("Couldn't execute command: %1", command), Logger::Warning);
  }
  checkScheduleTimer.start(1000);
}


void AkonadiCommandManager::setupSchedule()
{
  kDebug() << "Setting up schedule";
  schedule.clear();
  
  if (!getAkonadiConfiguration()->executeAkonadiRequests())
    return;
  
  Akonadi::ItemFetchJob *itemFetcher = new Akonadi::ItemFetchJob(
    Akonadi::Collection(getAkonadiConfiguration()->getCollection()), this);
  itemFetcher->fetchScope().fetchFullPayload();
  connect(itemFetcher, SIGNAL(finished(KJob*)), this, SLOT(itemsReceived(KJob*)));
}

void AkonadiCommandManager::itemsReceived(KJob* job)
{
  Akonadi::ItemFetchJob *fetchJob = static_cast<Akonadi::ItemFetchJob*>( job );
  if ( job->error() ) {
      Logger::log(i18n("Akonadi item job returned error: %1", job->errorString()), Logger::Error);
      kError() << job->errorString();
      return;
  }

  Akonadi::Item::List items = fetchJob->items();
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
    if (endDate.dateTime() < QDateTime::currentDateTime())
      continue;

    QString summary = (*event).summary();
    if (!summary.startsWith(getAkonadiConfiguration()->akonadiRequestPrefix()))
      continue;
    schedule.insert(startDate.dateTime(), 
		    summary.remove(getAkonadiConfiguration()->akonadiRequestPrefix()).trimmed());
  }
  Logger::log(i18n("Retrieved %1 relevant items", schedule.count()));
}


bool AkonadiCommandManager::shouldAcceptCommand(Command *command)
{
  return (dynamic_cast<AkonadiCommand*>(command) != 0);
}

const QString AkonadiCommandManager::iconSrc() const
{
  return "akonadi";
}


const QString AkonadiCommandManager::name() const
{
  return i18n("Akonadi");
}

void AkonadiCommandManager::parseConfiguration()
{
  if (getAkonadiConfiguration()->executeAkonadiRequests())
    checkScheduleTimer.start(1000);
  else
    checkScheduleTimer.stop();
  
  //setup monitoring
  Akonadi::Collection::List monitoredCollections = akonadiMonitor->collectionsMonitored();
  foreach (Akonadi::Collection c, monitoredCollections)
    akonadiMonitor->setCollectionMonitored(c, false);
  akonadiMonitor->setCollectionMonitored(Akonadi::Collection(getAkonadiConfiguration()->getCollection()), true);
  
  setupSchedule();
}

AkonadiConfiguration* AkonadiCommandManager::getAkonadiConfiguration()
{
  return static_cast<AkonadiConfiguration*>(getConfigurationPage());
}

bool AkonadiCommandManager::deSerializeConfig(const QDomElement& elem)
{
  if (!config) config->deleteLater();
  config = new AkonadiConfiguration(this, parentScenario);
  
  if ( !Akonadi::Control::start( config ) ) {
    Logger::log(i18n("Failed to contact akonadi.", Logger::Error));
    return false;
  }
  
  bool succ = config->deSerialize(elem);
  
  parseConfiguration();
  return succ;
}

CreateCommandWidget* AkonadiCommandManager::getCreateCommandWidget(QWidget *parent)
{
  return new CreateAkonadiCommandWidget(this, parent);
}

Akonadi::Entity::Id AkonadiCommandManager::getCollection()
{
  return getAkonadiConfiguration()->getCollection();
}

QString AkonadiCommandManager::akonadiRequestPrefix()
{
  return getAkonadiConfiguration()->akonadiRequestPrefix();
}


DEFAULT_DESERIALIZE_COMMANDS_PRIVATE_C(AkonadiCommandManager, AkonadiCommand);

AkonadiCommandManager::~AkonadiCommandManager()
{
  delete akonadiMonitor;
}
