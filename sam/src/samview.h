/*
 *  Copyright (C) 2009 Peter Grasch <grasch@simon-listens.org>
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


#ifndef SAMVIEW_H
#define SAMVIEW_H

#include <QWidget>
#include <QProcess>
#include <kxmlguiwindow.h>

#include "ui_main.h"

class KProcess;
class KAction;
class QCloseEvent;
class KCMultiDialog;
/**
 * This is the main view class for sam.  Most of the non-menu,
 * non-toolbar, and non-statusbar (e.g., non frame) GUI code should go
 * here.
 *
 * @short Main view
 * @author Peter Grasch <grasch@simon-listens.org>
 * @version 0.1
 */

class SamView :  public KXmlGuiWindow
{
    Q_OBJECT
public:
    /**
     * Default constructor
     */
    SamView(QWidget *parent, Qt::WFlags flags);

    /**
     * Destructor
     */
    virtual ~SamView();

private slots:
    void compileModel();
    void modelInputFilesChanged();

private:
    Ui::MainWindow ui;

};

#endif // samVIEW_H
