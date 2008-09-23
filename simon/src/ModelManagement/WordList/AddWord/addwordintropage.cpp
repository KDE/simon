//
// C++ Implementation: addwordintropage
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "addwordintropage.h"
#include <KLineEdit>
#include <KLocalizedString>

/**
 * \brief Constructor - also creates the GUI Elements
 * \author Peter Grasch
 * @param parent
 * The parent of the widget
 */
AddWordIntroPage::AddWordIntroPage(QWidget* parent): QWizardPage(parent)
{
	setTitle(i18n("Hinzufügen eines neuen Wortes"));

	ui.setupUi(this);

	registerField("wordNameIntro*", ui.leName);
}

