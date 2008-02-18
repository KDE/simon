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
#include "importlocalwizardpage.h"

#include <QLineEdit>
#include <QVariant>
#include <QFileDialog>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QDir>


/**
 * \brief Constructor
 * \author Peter Grasch
 * @param parent 
 * Parent of the wizardpage
 */
ImportLocalWizardPage::ImportLocalWizardPage(QWidget *parent) : QWizardPage(parent)
{
	
	setTitle(tr("Importieren aus Textdatei"));
	QLabel *label = new QLabel(this);
	label->setText(tr("Hier können Sie Textdateien importieren. Sollte ihr Text in\neinem anderen Format (z.B.: einem Word Dokument)\nvorliegen, kännen Sie ihn in der spezifischen Applikation\nöffnen, als Text speichern und diese Datei hier angeben.\n\nBitte wählen Sie nun die Datei aus die Sie gerne\nimportieren möchten:"));
	
	QVBoxLayout *layout = new QVBoxLayout(this);
	
	QGridLayout *glInput = new QGridLayout();
	
	QHBoxLayout *fileLay = new QHBoxLayout();
	QLabel *lbPath = new QLabel();
	lbPath->setText("Datei:");
	QLineEdit* lePath = new QLineEdit(this);
	lePath->setReadOnly(true);
	QPushButton *pbSelectPath = new QPushButton(this);
	pbSelectPath->setIcon(QIcon(":/images/icons/document-open.svg"));
	
	fileLay->addWidget(lePath);
	fileLay->addWidget(pbSelectPath);
	
	QLabel *lbName = new QLabel(this);
	lbName->setText(tr("Name des Textes: "));
	QLineEdit *leName = new QLineEdit(this);
	
	glInput->addWidget(lbName, 0,0);
	glInput->addWidget(leName, 0,1);
	
	glInput->addWidget(lbPath, 1,0);
	glInput->addLayout(fileLay, 1,1);
	
	layout->addWidget(label);
	layout->addLayout(glInput);
	
	this->setLayout(layout);
	connect(pbSelectPath, SIGNAL(clicked()), this, SLOT(setLocalSourceFile()));
	
	registerField("Filename*", lePath);
	registerField("Textname*", leName);
}


/**
 * \brief Sets the local source file
 * \author Peter Grasch
 * Shows a dialog that prompts the user to open a Textfile 
 * (Filter: *.txt)
 */
void ImportLocalWizardPage::setLocalSourceFile()
{

	QStringList files = QFileDialog::getOpenFileNames(this, tr("Zu importierende Textdateien öffnen"), QDir::currentPath(), tr("Textdateien (*.txt)"));
	if (files.count() == 0) return;

	setField("Filename", files.at(0));
}
