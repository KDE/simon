/*
 *  Copyright (C) 2010 Peter Grasch <grasch@simon-listens.org>
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

#ifndef SIMON_SPEECHCALVIEW_H_4002119636CC42C68FE07273F9000A73
#define SIMON_SPEECHCALVIEW_H_4002119636CC42C68FE07273F9000A73

#include <QWidget>
#include <kxmlguiwindow.h>
#include "ui_main.h"

class CalendarModel;

/**
 * @short Main view
 * @author Peter Grasch <grasch@simon-listens.org>
 * @version 0.1
 */
class SpeechCalView :  public KXmlGuiWindow
{
  Q_OBJECT
  public:
    /**
     * Default constructor
     */
    SpeechCalView(QWidget *parent=0, Qt::WFlags flags=0);

    /**
     * Destructor
     */
    virtual ~SpeechCalView();
    
    void displayModel(CalendarModel* model);
    
  private:
    Ui::MainWindow ui;
};

#endif

