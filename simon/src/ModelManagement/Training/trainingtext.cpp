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


#include "trainingtext.h"

/**
 * @brief Constructor
 *
 *	@author Peter Grasch
 * \param QString name
 * The name of the text
 * \param QStringList pages
 * The pages of the text
 * \param float relevance
 * The relevance of the text - the higher the better it'd be to train the text
 */
TrainingText::TrainingText(  QString name, QString path, QStringList pages )
{
	this->name = name;
	this->path = path;
	this->pages = pages;
}


/**
 * @brief Destructor
 *
 *	@author Peter Grasch
 */
TrainingText::~TrainingText()
{
}


