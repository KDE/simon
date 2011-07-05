/*
 *   Copyright (C) 2008 Peter Grasch <grasch@simon-listens.org>
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

#ifndef SIMON_FILTERCOMMANDMANAGER_H_04892F29B6DA47D6814F4E7C23D1DAD8
#define SIMON_FILTERCOMMANDMANAGER_H_04892F29B6DA47D6814F4E7C23D1DAD8

#include <simonscenarios/commandmanager.h>

#include <QVariantList>

class KAction;
class FilterConfiguration;

/**
 *	@class FilterCommandManager
 *	@brief Manager of the Filter
 *
 *	@version 0.1
 *	@date 22.08.2009
 *	@author Peter Grasch
 */
class FilterCommandManager : public CommandManager
{
  Q_OBJECT

  protected:
    bool isActive;
    bool stageOne;
    bool trigger(const QString& triggerName, bool silent);
    void updateAction();
    FilterConfiguration* configuration();

  private slots:
    void leaveStageOne();

  public slots:
    void toggle();
    void activateFilter();
    void deactivateFilter();
    void deactivateOnce();
  public:
    const QString preferredTrigger() const;
    const QString iconSrc() const;

    const QString name() const;
    bool deSerializeConfig(const QDomElement& elem);

    /**
     * @brief Constructor
     *
     *	@author Peter Grasch
     */
    FilterCommandManager(QObject* parent, const QVariantList& args);

    ~FilterCommandManager();

  private:
    KAction *activateAction;

};
#endif
