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

#include "juliuscontrol.h"
#include <KLocalizedString>

#ifdef FALSE
#undef FALSE
#endif
#ifdef TRUE
#undef TRUE
#endif

extern "C" {
	#include <julius/julius.h>
}

// extern Jconf* j_config_load_args_new(int argc, char** argv);

JuliusControl::JuliusControl(QObject *parent) : RecognitionControl(parent)
{

}

bool JuliusControl::initializeRecognition()
{
	Jconf *jconf = j_jconf_new();
	
// 	delete jconf;


	emit recognitionError(i18n("Not implemented"));
	return false;
}

JuliusControl::~JuliusControl()
{
	
}
