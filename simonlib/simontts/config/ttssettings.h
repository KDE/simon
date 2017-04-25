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

#ifndef SIMON_TTSSETTINGS_H_D92C3FEDF32B415094257DB891621383
#define SIMON_TTSSETTINGS_H_D92C3FEDF32B415094257DB891621383

#include "ui_simonttsconfigurationdlg.h"
#include <QVariantList>
#include <KCModule>

class RecordingSetCollection;
class QSortFilterProxyModel;

/**
  @author Peter Grasch <peter.grasch@bedahr.org>
*/
class TTSSettings : public KCModule
{
  Q_OBJECT
  private:
    Ui::TTSConfiguration ui;

    RecordingSetCollection *sets;

    QSortFilterProxyModel *recordings;

    QString translateBackend(const QString& backend);
    void setupSets();
    void displaySets(int changeToIndex=-1);

    int getCurrentlySelectedSet();
    QString getCurrentlySelectedRecording();
    QString getCurrentlySelectedPath();

  private slots:
    void slotChanged();

    void addSet();
    void renameSet();
    void removeSet();
    void exportSet();
    void importSet();
    void addRecording();
    void editRecording();
    void removeRecording(); 

    void displayCurrentSet();

  public:
    explicit TTSSettings(QWidget* parent, const QVariantList& args=QVariantList());

    void load();
    void save();
    void defaults();

    ~TTSSettings();

};
#endif
