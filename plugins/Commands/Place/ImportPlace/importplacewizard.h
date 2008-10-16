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


#ifndef IMPORTPLACEWIZARD_H
#define IMPORTPLACEWIZARD_H

#include <QWizard>


class IntroPlacePage;
class SelectPlacePage;
class Command;

/**
 * \class ImportPlaceWizard
 * \author Susanne Tschernegg
 * \date 03.09.2007
 * \version 0.1
 * \brief With this wizard the user can add a local or a remote place to the commands.
 */
class ImportPlaceWizard : public QWizard
{

    Q_OBJECT
		
	private:
        SelectPlacePage* selectPlacePage;

	QWizardPage* createIntroPlacePage();
	SelectPlacePage* createSelectPlacePage();

        QWizardPage* createFinishedPage();

public:
        ImportPlaceWizard(QWidget* parent=0);

        ~ImportPlaceWizard();

private slots:
	void createCommand(int);
signals:
   void commandCreated(Command* command);

};

#endif
