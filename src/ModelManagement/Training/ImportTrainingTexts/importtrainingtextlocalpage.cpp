//
// C++ Implementation: importlocalwizardpage
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "importtrainingtextlocalpage.h"

#include <KLineEdit>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <KPushButton>
#include <QDir>


/**
 * \brief Constructor
 * \author Peter Grasch
 * @param parent 
 * Parent of the wizardpage
 */
ImportTrainingTextLocalPage::ImportTrainingTextLocalPage(QWidget *parent) : QWizardPage(parent)
{
	ui.setupUi(this);
	setTitle(tr("Importieren aus Textdatei"));
	
	registerField("importTrainingTextLFilename*", ui.lePath, "currentUrl", SIGNAL(urlChanged(QString)));
	registerField("importTrainingTextLTextname*", ui.leName);
}


