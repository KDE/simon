/*
 *   Copyright (C) 2009-2010 Grasch Peter <peter.grasch@bedahr.org>
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

#ifndef SIMON_TURNCONFIGURATION_H_4B4956DCAE204C49977297D20CB81F09
#define SIMON_TURNCONFIGURATION_H_4B4956DCAE204C49977297D20CB81F09

#include <simonscenarios/commandconfiguration.h>
#include "ui_turncreateeditview.h"
#include <KSharedConfig>
#include <QPointer>
#include <QPoint>
#include <QSize>

class DialogTurn;

class TurnConfiguration : public KDialog
{
  Q_OBJECT
  public:
    QDialog::DialogCode code;

  private:
    DialogTurn* turn;
    Ui::TurnCreateEditView ui;

  private slots:
    void addPrompt();
    void removePrompt();
    void editPrompt();

    void addExtractor();
    void removeExtractor();

    void displaySelectedText();
    void updateTextSelector();
    void save();
    void forget();
    void displayCurrentTurn();

  public slots:
    void init();

    virtual bool deSerialize(const QDomElement&);
    virtual QDomElement serialize(QDomDocument *doc);
    virtual void defaults();

  public:
    TurnConfiguration(DialogTurn* _turn, QWidget *parent);
    ~TurnConfiguration();

};
#endif
