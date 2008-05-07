//
// C++ Implementation: SimonFileSystemSelector
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include <QToolButton>
#include "simonfilesystemselector.h"
#include <QHBoxLayout>
#include <QStringList>
#include <QFileInfo>
#include "simonlineedit.h"

SimonFileSystemSelector::SimonFileSystemSelector(QString defaultSrc, QString dlgCaption, 
						QString dir,  QFileDialog::Options options,
						QString filter, QFileDialog::FileMode type,
						QWidget *parent) : QWidget(parent)
{
	QHBoxLayout *layout = new QHBoxLayout(this);
	edit = new SimonLineEdit(this);
	edit->setText(defaultSrc);

	button = new QToolButton(this);
	layout->addWidget(edit);
	layout->addWidget(button);
	this->setLayout(layout);

// 	QFileIconProvider iconProvider
	button->setText("...");
	this->defaultSrc = defaultSrc;
	this->dlgCaption = dlgCaption;
	this->dir = dir;
	this->options = options;
	this->filter = filter;
// 	if ((type == QFileDialog::AnyFile) || (type == QFileDialog::ExistingFile))
// 	{
// 		button->setIcon(iconProvider.icon(QFileIconProvider::File));
// 	} else {
// 		button->setIcon(iconProvider.icon(QFileIconProvider::Folder));
// 	}
	button->setIcon(QIcon(":/images/icons/document-open.svg"));
	this->type = type;

	connect(edit, SIGNAL(textChanged(QString)), this, SIGNAL(urlChanged(QString)));
	connect(button, SIGNAL(clicked()), this, SLOT(showOpenDialog()));
}

void SimonFileSystemSelector::setText(QString text)
{
	edit->setText(text);
}

void SimonFileSystemSelector::showOpenDialog()
{
	QString src;
	QString directory = this->dir;

	if (!edit->text().isEmpty())
	{
		QFileInfo fInfo(edit->text());
		directory = fInfo.path();
	}
	
	QFileDialog *dlg = new QFileDialog(this, dlgCaption, directory, filter);
	dlg->setFileMode(this->type);
	dlg->selectFile(edit->text());
	

	if (dlg->exec())
	{
		QStringList srcs = dlg->selectedFiles();
		if (srcs.count() > 0) src = srcs[0];
		edit->setText(src);
		emit urlChanged(src);
	}

	dlg->deleteLater();
}


QString SimonFileSystemSelector::text()
{
	return edit->text();
}
