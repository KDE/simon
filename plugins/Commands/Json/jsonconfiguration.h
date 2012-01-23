/*
 *   Copyright (C) 2009 Grasch Peter <grasch@simon-listens.org>
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

#ifndef SIMON_JSONCONFIGURATION_H_4B4956DCAE204C49977297D20CB81F09
#define SIMON_JSONCONFIGURATION_H_4B4956DCAE204C49977297D20CB81F09

#include <simonscenarios/commandconfiguration.h>
#include "ui_jsonconfigurationdlg.h"

class JsonCommandManager;

class JsonConfiguration : public CommandConfiguration
{
  Q_OBJECT

  private:
    Ui::JsonConfigurationDlg ui;


  public slots:
    virtual bool deSerialize(const QDomElement&);
    virtual QDomElement serialize(QDomDocument *doc);
    virtual void defaults();

  public:
    explicit JsonConfiguration(Scenario *parent, const QVariantList &args = QVariantList());
    ~JsonConfiguration();

    QString host();
    int port();

    void destroy();

};
#endif
