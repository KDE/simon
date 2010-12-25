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

#ifndef SIMON_AKONADICOMMANDMANAGER_H_7A7B9100FF5245329569C1B540119C37
#define SIMON_AKONADICOMMANDMANAGER_H_7A7B9100FF5245329569C1B540119C37

#include <simonscenarios/commandmanager.h>
#include <QVariantList>
#include <QObject>
#include <QMap>
#include <QTimer>
#include <akonadi/entity.h>

class AkonadiConfiguration;
class KJob;

namespace Akonadi
{
  class Monitor;
}

class AkonadiCommandManager : public CommandManager
{
  Q_OBJECT

  private:
    QMap<QDateTime, QString> schedule;
    QTimer checkScheduleTimer;
    Akonadi::Monitor *akonadiMonitor;
    
    AkonadiConfiguration* getAkonadiConfiguration();
    
  private slots:
    void itemsReceived(KJob* job);
    void checkSchedule();
    void setupSchedule();

  protected:
    bool shouldAcceptCommand(Command *command);

  public:
    const QString preferredTrigger() const { return QString(); }
    const QString iconSrc() const;
    const QString name() const;
    bool deSerializeConfig(const QDomElement& elem);
    void parseConfiguration();

    CreateCommandWidget* getCreateCommandWidget(QWidget *parent);
    
    Akonadi::Entity::Id getCollection();
    QString akonadiRequestPrefix();

    DEFAULT_DESERIALIZE_COMMANDS_PRIVATE_H;

    /**
     * @brief Constructor
     *
     *	@author Peter Grasch
     */
    AkonadiCommandManager(QObject* parent, const QVariantList& args);

    ~AkonadiCommandManager();

};
#endif
