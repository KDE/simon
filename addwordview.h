/***************************************************************************
 *   Copyright (C) 2006 by Peter Grasch   *
 *   bedahr@gmx.net   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


/**
 *	@class AddWordView
 *	@brief The wizard to add a new word to the Model
 *
 *	This wizard is using a step-by-step guide through the procedure
 *	to add a new word to the Language Model.
 *
 *	@version 0.1
 *	@date 08.01.2006
 *	@author Peter Grasch
 *	@todo Adding the word to the model
*/
 
#ifndef ADDWORDVIEW_H
#define ADDWORDVIEW_H

#include <QWizard>
#include "recwidget.h"


class QWizardPage;

class AddWordView : public QWizard
{
	Q_OBJECT

	private:
		RecWidget *rec1;
		RecWidget *rec2;
		void hideEvent(QHideEvent *event) { 
			emit hidden(); return QWidget::hideEvent(event); }
	
	signals:
		void addedWord();
		void hidden();
		
	public slots:
		
		void finish(int done);

		QWizardPage* createWelcomePage();
		QWizardPage* createRecordPage();
		QWizardPage* createFinishedPage();
		
		
	public:
		AddWordView(QWidget *parent=0);
		~AddWordView()
		{
		}
};

#endif
