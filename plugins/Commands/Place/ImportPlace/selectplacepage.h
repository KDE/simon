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


#ifndef SIMON_SELECTPLACEPAGE_H_F9E2066E505F4530AAC69899C0D1DD4A
#define SIMON_SELECTPLACEPAGE_H_F9E2066E505F4530AAC69899C0D1DD4A
#include <QWizardPage>
#include <kurl.h>

#include "ui_selectplacedlg.h"
#include "importplacewizard.h"

/**
 *	@class SelectPlacePage
 *	@brief Allows the user to select a local or remote place
 *
 *	@version 0.1
 *	@date 29.05.2008
 *	@author Peter Grasch
 */
class SelectPlacePage : public QWizardPage{

Q_OBJECT

private:
	Ui::SelectPlaceDlg ui;
private slots:
	void buildRemoteUrl();
	void parseRemoteUrl();
	
public:
	SelectPlacePage(QWidget *parent=0);
	QString getName() const;
	KUrl getUrl() const;
	void initializePage();
	bool isComplete() const;

};

#endif
