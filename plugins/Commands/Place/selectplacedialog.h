/*
 *   Copyright (C) 2008 Peter Grasch <peter.grasch@bedahr.org>
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

#ifndef SIMON_SELECTPLACEDIALOG_H_F9E2066E505F4530AAC69899C0D1DD4A
#define SIMON_SELECTPLACEDIALOG_H_F9E2066E505F4530AAC69899C0D1DD4A

#include <QDialog>
#include <QUrl>
#include <KDELibs4Support/KDE/KDialog>

#include "ui_selectplacedlg.h"

class PlaceCommand;

/**
 *	@class SelectPlaceDialog
 *	@brief Allows the user to select a local or remote place
 *
 *	@version 0.1
 *	@date 29.05.2008
 *	@author Peter Grasch
 */
class SelectPlaceDialog : public KDialog
{

  Q_OBJECT

    private:
    Ui::SelectPlaceDlg ui;

  private slots:
    void buildRemoteUrl();
    void parseRemoteUrl();
    QString getName() const;
    QUrl getUrl() const;
    void initializeDialog();
    bool isComplete() const;
    void checkComplete();

  public:
    SelectPlaceDialog(QWidget *parent=0);
    PlaceCommand* selectPlace();

};
#endif
