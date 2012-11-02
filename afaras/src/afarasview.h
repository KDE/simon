/*
 *  Copyright (C) 2011 Peter Grasch <peter.grasch@bedahr.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2,
 *  or (at your option) any later version, as published by the Free
 *  Software Foundation
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details
 *
 *  You should have received a copy of the GNU General Public
 *  License along with this program; if not, write to the
 *  Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef SIMON_AFARASVIEW_H_4002119636CC42C68FE07273F9000A73
#define SIMON_AFARASVIEW_H_4002119636CC42C68FE07273F9000A73

#include <KXmlGuiWindow>

#include "ui_main.h"
#include <QStringList>
#include <QPair>

/**
 * @short Main view
 * @author Peter Grasch <peter.grasch@bedahr.org>
 * @version 0.1
 */

class RecWidget;
class QKeyEvent;

class AfarasView :  public KXmlGuiWindow
{
  Q_OBJECT
  public:
    /**
     * Default constructor
     */
    AfarasView(QWidget *parent, Qt::WFlags flags);

    /**
     * Destructor
     */
    virtual ~AfarasView();

  private slots:
    void start();
    void stop();
    void nextSample();
    void previousSample();
    void blackListSample(bool list);
    void inputFilesChanged();
    void showConfig();

  protected:
    void keyPressEvent ( QKeyEvent * event );

  private:
    Ui::MainWindow ui;
    RecWidget *recorder;
    
    int currentIndex;

    QList< QPair<QString,QString> > prompts;
    QStringList blackListedRecordings;
    
    void setupRecorder();
    void save();
    QString currentSample() const;
};

#endif

