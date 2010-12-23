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

#ifndef SIMON_SPEECHCAL_H_4002119636CC42C68FE07273F9000A73
#define SIMON_SPEECHCAL_H_4002119636CC42C68FE07273F9000A73

#include <QObject>
#include <QHash>
#include <KDateTime>

#include <akonadi/collection.h>
#include <akonadi/item.h>

class SpeechCalView;
class KJob;

class SpeechCal : public QObject
{
Q_OBJECT
  public:
    SpeechCal();
    virtual ~SpeechCal();
    void exec();
    
  private slots:
    void slotCollectionsReceived(Akonadi::Collection::List);
    void slotItemsReceived(Akonadi::Item::List);
//     void itemFetchDone(KJob* job);
    
  private:
    SpeechCalView* view;
    KDateTime fromDate;
    KDateTime toDate;
    
//     QHash<Akonadi::Entity::Id /*collectionId*/, Akonadi::Item::List> headerItems;
//     QHash<Akonadi::Entity::Id /*collectionId*/, Akonadi::Item::List> retrievedItems;
    
    void setupCollections();
//     void dissect(Akonadi::Item::List list, int lowerBound = 0, int upperBound = -1);

};

#endif // SPEECHCAL_H
