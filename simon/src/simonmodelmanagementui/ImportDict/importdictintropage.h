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


#ifndef SIMON_IMPORTDICTINTROPAGE_H_F83B50A389784692B464FE3A7EEE5112
#define SIMON_IMPORTDICTINTROPAGE_H_F83B50A389784692B464FE3A7EEE5112

#include <QWizardPage>
class QString;

/**
 * \class ImportDictIntroPage
 * \brief WizardPage to select a Intro Dictionary
 * \author Peter Grasch
 * \date 10.8.2007
 * \version 0.1
 */
class ImportDictIntroPage : public QWizardPage
{
Q_OBJECT
public:
    ImportDictIntroPage(QWidget* parent);

    ~ImportDictIntroPage();

};

#endif

