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


#ifndef SIMON_IMPORTLEXICONPAGE_H_F49A09B6297C4796A6FD9D58F6E7E8AC
#define SIMON_IMPORTLEXICONPAGE_H_F49A09B6297C4796A6FD9D58F6E7E8AC

#include <QWizardPage>
#include "ui_importdictlexiconpage.h"
#include "importdictview.h"
/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class ImportLexiconPage : public QWizardPage
{
Q_OBJECT
private:
	Ui::ImportDictLexiconPage ui;
public:
    ImportLexiconPage(QWidget* parent);
	int nextId() const { return ImportDictView::WorkingPage; }
	bool isComplete() const;
	void initializePage();

    ~ImportLexiconPage();

};

#endif
