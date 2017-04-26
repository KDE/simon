/*
 *   Copyright (C) 2010 Peter Grasch <peter.grasch@bedahr.org>
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

#include "akonadicommand.h"
#include <simonlogging/logger.h>
#include <QSharedPointer>
#include <QDomDocument>
#include <QDomElement>
#include <QVariant>
#include <QIcon>
#include <KLocalizedString>
#include <QDebug>
#include <AkonadiCore/attribute.h>
#include <AkonadiCore/item.h>
#include <AkonadiCore/itemcreatejob.h>
#include <AkonadiCore/collection.h>
#include <kcalcore/incidence.h>
#include <kcalcore/event.h>
#include "akonadicommandmanager.h"
#include "timeselector.h"

const QString AkonadiCommand::staticCategoryText()
{
  return i18n("Akonadi");
}


const QString AkonadiCommand::getCategoryText() const
{
  return AkonadiCommand::staticCategoryText();
}


const QIcon AkonadiCommand::staticCategoryIcon()
{
  return QIcon::fromTheme("akonadi");
}


const QIcon AkonadiCommand::getCategoryIcon() const
{
  return AkonadiCommand::staticCategoryIcon();
}

void AkonadiCommand::getRelativeTime(AkonadiCommand::RelativeDurationDimension& dimension, int& value) const
{
  return TimeSelector::getRelativeTime(m_relativeDuration, dimension, value);
}

const QMap<QString,QVariant> AkonadiCommand::getValueMapPrivate() const
{
  QMap<QString,QVariant> out;
  out.insert(i18n("The associated command"), m_command);
  
  QString strType;
  switch (m_type)
  {
    case AkonadiCommand::Relative:
      strType = i18nc("Timer type", "Relative");
      break;
    case AkonadiCommand::Absolute:
      strType = i18nc("Timer type", "Absolute");
      break;
  }
  out.insert(i18n("Associated command"), m_command);
  out.insert(i18n("Timer type"), strType);

  out.insert(i18n("Absolute time"), m_executeAt.toString());

  AkonadiCommand::RelativeDurationDimension dimension;
  int value;
  getRelativeTime(dimension, value);

  KLocalizedString strDimension;
  switch (dimension)
  {
    case AkonadiCommand::Seconds:
      strDimension = ki18ncp("Time dimension", "%1 Second", "%1 Seconds");
      break;

    case AkonadiCommand::Minutes:
      strDimension = ki18ncp("Time dimension", "%1 Minute", "%1 Minutes");
      break;

    case AkonadiCommand::Hours:
      strDimension = ki18ncp("Time dimension", "%1 Hour", "%1 Hours");
      break;

     case AkonadiCommand::Days:
       strDimension = ki18ncp("Time dimension", "%1 Day", "%1 Days");
       break;
   }

  out.insert(i18n("Relative time"), strDimension.subs(value).toString());
  return out;
}


bool AkonadiCommand::triggerPrivate(int *state)
{
  Q_UNUSED(state);
  
  qDebug() << "Triggering...";
  KDateTime executionTime = calculateExecutionTime();
  AkonadiCommandManager *manager = static_cast<AkonadiCommandManager*>(Command::parent());
  Akonadi::Item item(KCalCore::Event::eventMimeType());
  
  QSharedPointer<KCalCore::Event> event(new KCalCore::Event());
  event->setSummary(manager->akonadiRequestPrefix()+' '+m_commandType+"//"+m_command);
  event->setDtStart(executionTime);
  event->setDtEnd(executionTime);
  
  item.setPayload< QSharedPointer<KCalCore::Event> >(event);
  Akonadi::Collection collection = Akonadi::Collection(manager->getCollection());
  Akonadi::ItemCreateJob *createJob = new Akonadi::ItemCreateJob(item, collection);
  
  connect(createJob, SIGNAL(finished(KJob*)), this, SLOT(storeJobFinished(KJob*)));
  Q_UNUSED(createJob);
  return true;
}

void AkonadiCommand::storeJobFinished(KJob* job)
{
  Akonadi::ItemCreateJob *createJob= static_cast<Akonadi::ItemCreateJob*>( job );
  Q_UNUSED(createJob);
  if ( job->error() ) {
      Logger::log(i18n("Failed to create Akonadi item: %1", job->errorString()), Logger::Error);
      qWarning() << job->errorString();
  }
}



QDomElement AkonadiCommand::serializePrivate(QDomDocument *doc, QDomElement& commandElem)
{
  QDomElement subCommandElem = doc->createElement("subCommand");
  subCommandElem.setAttribute("type", m_commandType);
  subCommandElem.setAttribute("trigger", m_command);
  commandElem.appendChild(subCommandElem);
  
  QDomElement timerElem = doc->createElement("timer");
  timerElem.setAttribute("type", (int) m_type);
  QDomElement absoluteElem = doc->createElement("absolute");
  absoluteElem.appendChild(doc->createTextNode(m_executeAt.toString()));
  timerElem.appendChild(absoluteElem);
  QDomElement relativeElem = doc->createElement("relative");
  relativeElem.appendChild(doc->createTextNode(QString::number(m_relativeDuration)));
  timerElem.appendChild(relativeElem);
  
  commandElem.appendChild(timerElem);
  
  return commandElem;
}

KDateTime AkonadiCommand::calculateExecutionTime()
{
  if (m_type == AkonadiCommand::Relative)
  {
    return KDateTime(QDateTime::currentDateTime().addSecs(m_relativeDuration));
  } else {
    return m_executeAt;
  }
}

bool AkonadiCommand::deSerializePrivate(const QDomElement& commandElem)
{
  qDebug() << "deserializing...";
  QDomElement subCommandElem = commandElem.firstChildElement("subCommand");
  m_command = subCommandElem.attribute("trigger");
  m_commandType = subCommandElem.attribute("type");
  
  QDomElement timerElem = commandElem.firstChildElement("timer");
  bool ok = true;
  int timerType = timerElem.attribute("type").toInt(&ok);
  qDebug() << timerElem.attribute("type") << timerType << ok;
  if (!ok) return false;
  
  m_type = (AkonadiCommand::TimerType) timerType;
  
  QDomElement absoluteElem = timerElem.firstChildElement("absolute");
  m_executeAt = KDateTime::fromString(absoluteElem.text());
  QDomElement relativeElem = timerElem.firstChildElement("relative");
  m_relativeDuration = relativeElem.text().toInt();
  
  return true;
}


STATIC_CREATE_INSTANCE_C(AkonadiCommand);
