/*
 *   Copyright (C) 2008 Phillip Goriup <goriup@simon-listens.org>
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



#ifndef SIMON_SIMONSPINBOX_H_6C367508FBAF4560BD03EAB09B0BB436
#define SIMON_SIMONSPINBOX_H_6C367508FBAF4560BD03EAB09B0BB436

#include <KIntNumInput>
#include "speechgui_export.h"

class SPEECHGUI_EXPORT SimonSpinBox : public KIntNumInput
{	
	
	Q_OBJECT
	
private:
	
	
public:
	SimonSpinBox( QWidget * parent  = 0);
	~SimonSpinBox();

public slots:
	void changeValue(QString text);

	
};


#endif
