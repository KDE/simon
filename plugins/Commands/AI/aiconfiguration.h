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

#ifndef SIMON_AICONFIGURATION_H_5E8C8114E79B4A54A9028B5E857F3F4A
#define SIMON_AICONFIGURATION_H_5E8C8114E79B4A54A9028B5E857F3F4A

#include <simonscenarios/commandconfiguration.h>
#include "ui_aiconfigurationdlg.h"
#include <KSharedConfig>
#include <QPointer>

class AICommandManager;

class AIConfiguration : public CommandConfiguration
{
  Q_OBJECT

    private:
    Ui::AIConfigurationDlg ui;
    AICommandManager *manager;

    QString storedAimlSet;

  public:
    AIConfiguration(Scenario *parent, const QVariantList &args = QVariantList());
    ~AIConfiguration();

    void setManager(AICommandManager *manager);
    virtual bool deSerialize(const QDomElement&);
    virtual QDomElement serialize(QDomDocument *doc);
    virtual void defaults();

    QString aimlSet() { return storedAimlSet; }
};
#endif
