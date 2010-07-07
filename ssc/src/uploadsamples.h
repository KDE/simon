/*
 *   Copyright (C) 2010 Peter Grasch <grasch@simon-listens.org>
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

#ifndef SIMON_SSC_UPLOADMODEL_H_5B939A5C2BE44EB3A400FED68B80E791
#define SIMON_SSC_UPLOADMODEL_H_5B939A5C2BE44EB3A400FED68B80E791

#include <KDialog>
#include "ui_uploadsamples.h"

class UploadSamples : protected KDialog {

Q_OBJECT

private:
	Ui::UploadSamples ui;
	void initDisplay();

private slots:
	void upload();
	void remove();
	void edit();

public:
	UploadSamples(QWidget *parent=0);
	~UploadSamples();

	int exec();
	
};

#endif

