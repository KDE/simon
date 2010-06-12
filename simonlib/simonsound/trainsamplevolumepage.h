/*
 *   Copyright (C) 2009 Peter Grasch <grasch@simon-listens.org>
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

#ifndef SIMON_TRAINSAMPLEVOLUMEPAGE_H_4F90334EEE9C4EB697B9A2431C488A0F
#define SIMON_TRAINSAMPLEVOLUMEPAGE_H_4F90334EEE9C4EB697B9A2431C488A0F

#include <QWizardPage>
#include "simonsound_export.h"

class QCheckBox;
namespace Ui
{
	class TrainSampleVolumePage;
}

class SIMONSOUND_EXPORT TrainSampleVolumePage : public QWizardPage {

private:
	Ui::TrainSampleVolumePage *ui;

public:
	TrainSampleVolumePage(QWidget *parent=NULL);
	bool validatePage();
	void initializePage();
    	~TrainSampleVolumePage();

};

#endif

