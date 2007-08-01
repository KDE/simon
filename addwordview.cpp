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
 
#include "addwordview.h"
#include <QWizardPage>
#include <QMessageBox>
#include <QLineEdit>
#include "logger.h"
#include "addwordintropage.h"
#include "addwordrecordpage.h"

/**
 * @brief Constructor
 *
 * Constructs a new AddWordView Object.
 * Initializes the UI and sets it to the first page
 *
 *	@author Peter Grasch
 * @param QWidget *parent
 * Parent of the Dialog; Default: 0
 * @param Qt::Wflags f
 * Qt Windowflags - default 0
*/

AddWordView::AddWordView(QWidget *parent)
	: QWizard (parent)
{
	oldId = 0;
	this->addPage(createWelcomePage());
	this->addPage(createRecordPage());
	this->addPage(createFinishedPage());

	connect( this, SIGNAL(currentIdChanged( int )), this, 
			SLOT(pageChanged(int)));
	
	connect(this, SIGNAL(finished( int )), this, SLOT(finish( int )));

	setWindowTitle("Wort hinzufuegen");
	setPixmap(QWizard::WatermarkPixmap, QPixmap(":/images/addword.png"));
}

void AddWordView::pageChanged(int id)
{
	
	oldId = id;
}

QWizardPage* AddWordView::createWelcomePage()
{
	return new AddWordIntroPage(this);
}

QWizardPage* AddWordView::createRecordPage()
{
	return new AddWordRecordPage(this);
}


QWizardPage* AddWordView::createFinishedPage()
{
	QWizardPage *finished = new QWizardPage(this);
	finished->setTitle("Hinzufuegen des Wortes");
	QLabel *label = new QLabel(finished);
	label->setText("Es wurden alle benoetigten Daten gesammelt.\n\nSimon kann das neue Wort jetzt lernen.\nBitte ueberpruefen Sie, bevor Sie hier\nbestaetigen, ob die Aufnahmen nicht von\nHintergrundgeraeuschen beeintraechtigt werden.\n\nKlicken Sie auf \"Fertigstellen\" um den Wizard \nabzuschlieszen.");
	QVBoxLayout *layout = new QVBoxLayout(finished);
	layout->addWidget(label);
	finished->setLayout(layout);
	
	return finished;
}




/**
 * \brief Writes the word into the files and cleans up the wizard
 * 
 * \author Peter Grasch
 */
void AddWordView::finish(int done)
{
	((AddWordRecordPage*) this->page(1))->cleanUp();
	if (!done) return;
	
	QString word = ((AddWordIntroPage*) this->page(0))->getName();
	
	Logger::log(tr("Fuege neues Wort zum Modell hinzu..."));
	Logger::log(tr("Neues Wort lautet: ")+word);
	//finishs up
	
	//cleaning up
	Logger::log(tr("Word added: ")+word);
	
	restart();
}


AddWordView::~AddWordView()
{
}
