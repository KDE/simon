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



#ifndef SIMON_SIMONCOMBOBOX_H_F7EF9341BFB44C079F9D1C5D7F72D530
#define SIMON_SIMONCOMBOBOX_H_F7EF9341BFB44C079F9D1C5D7F72D530

#include <KComboBox>
#include "speechgui_export.h"

class SPEECHGUI_EXPORT SimonComboBox : public KComboBox
{	
	
	Q_OBJECT
	
private:
	
	
public:
	SimonComboBox( QWidget * parent  = 0);
	~SimonComboBox();

public slots:
	void schowPopUp();

	
};


#endif
