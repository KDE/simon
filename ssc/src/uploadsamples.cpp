/*
 *   Copyright (C) 2010 Peter Grasch <grasch@simon-listens.org>
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

#include "uploadsamples.h"
#include "trainingswizard.h"

#include <QDir>
#include <QFile>
#include <QStringList>
#include <QSettings>
#include <QInputDialog>

#include <KDebug>
#include <KMessageBox>
#include <KStandardDirs>
#include <KDialogButtonBox>
#include <KIcon>

UploadSamples::UploadSamples(QWidget* parent) : 
	KDialog(parent)
{
	QWidget *widget = new QWidget( this );
	ui.setupUi(widget);

	setMainWidget( widget );
	
	setCaption( i18n("Local Samples") );

	connect(ui.pbDeleteSamples, SIGNAL(clicked()), this, SLOT(remove()));
	connect(ui.pbUploadSamples, SIGNAL(clicked()), this, SLOT(upload()));
	connect(ui.pbEdit, SIGNAL(clicked()), this, SLOT(edit()));

	ui.pbDeleteSamples->setIcon(KIcon("edit-delete"));
	ui.pbUploadSamples->setIcon(KIcon("folder-sync"));
	ui.pbEdit->setIcon(KIcon("document-edit"));
}

UploadSamples::~UploadSamples()
{
}

void UploadSamples::upload()
{
	QListWidgetItem *item = ui.lwSamples->currentItem();
	if (!item) return;

	QString path = item->data(Qt::UserRole).toString();

	kDebug() << "Restoring data from: " << path;
	TrainingsWizard *wizard = new TrainingsWizard(this);

	wizard->init(item->data(Qt::UserRole+1).toInt() /*user id*/, path); 
	wizard->exec();
	wizard->deleteLater();
	initDisplay();
}

void UploadSamples::remove()
{
	QListWidgetItem *item = ui.lwSamples->currentItem();
	if (!item) return;

	QString path = item->data(Qt::UserRole).toString();

	kDebug() << "Deleting data from: " << path;
	if (KMessageBox::questionYesNoCancel(this, i18n("Do you really want to delete the stored samples at \"%1\"?", path)) != KMessageBox::Yes)
		return;

	QDir d(path);
	QStringList files = d.entryList(QDir::Files|QDir::NoDotAndDotDot);
	bool succ = true;
	kDebug() << "Entries in " << path << files;
	foreach (const QString& file, files)
	{
		kDebug() << "Removing: " << path+"/"+file;
		succ = QFile::remove(path+"/"+file) && succ;
	}

	succ = d.rmdir(path) && succ;

	if (!succ)
		KMessageBox::sorry(this, i18n("The directory could not be removed:\n%1", path));

	initDisplay();
}

void UploadSamples::edit()
{
	QListWidgetItem *item = ui.lwSamples->currentItem();
	if (!item) return;
	
	QString path = item->data(Qt::UserRole).toString();
	qint32 userId = item->data(Qt::UserRole+1).toInt();
	QString model = item->data(Qt::UserRole+2).toString();
	
	//int QInputDialog::getInt ( QWidget * parent, const QString & title, const QString & label, int value = 0, int min = -2147483647, int max = 2147483647, int step = 1, bool * ok = 0, Qt::WindowFlags flags = 0 )   [static]
	bool ok = true;
	qint32 newId = QInputDialog::getInt(this, i18n("Edit samples pack"), i18n("If you want to change the user id of this samples pack, please provide the new id below."), userId, 0, 2147483647, 1, &ok);
	if (!ok) return;

	kDebug() << "Editing: " << path << " to new id: " << newId;
	
	QString rootDirectory = KStandardDirs::locateLocal("appdata", QString("stored/"));
	QString newDirectory = rootDirectory+QString::number(newId)+"/";
	
	QDir storedDirectory(rootDirectory);
	if (!storedDirectory.cd(newDirectory))
	{
		if (!storedDirectory.mkdir(newDirectory))
		{
			KMessageBox::sorry(this, i18n("Failed to create target directory at \"%1\"",
							    newDirectory));
			return;
		}
	}
	
	if (!storedDirectory.rename(path, newDirectory+model))
		KMessageBox::sorry(this, i18n("Could not move \"%1\" to \"%2\".", 
					      path, newDirectory));
	
	/*
	 * 	QString directory = KStandardDirs::locateLocal("appdata", QString("stored/"));
	QDir d(directory);
	QStringList storedUsers = d.entryList(QDir::Dirs|QDir::NoDotAndDotDot);

	foreach (const QString& user, storedUsers)
	{
		bool ok = true;
		qint32 userId = user.toInt(&ok);
		if (!ok) continue;

		kDebug() << "Found stored user: " << user;

		d.cd(directory+"/"+user);
		QStringList models = d.entryList(QDir::Dirs|QDir::NoDotAndDotDot);
		foreach (const QString& model, models)
		{
			if (!QFile::exists(directory+"/"+user+"/"+model+"/profile.ini")) continue;
			kDebug() << "   Found stored samples: " << model;
			QListWidgetItem* item = new QListWidgetItem(QString("%1: %2").arg(user).arg(model), ui.lwSamples);
			item->setData(Qt::UserRole, directory+"/"+user+"/"+model+"/");
			item->setData(Qt::UserRole+1, userId);
			ui.lwSamples->addItem(item);
		}
	}*/
	
	initDisplay();
}


int UploadSamples::exec()
{
	initDisplay();
	return (KDialog::exec());
}
	
void UploadSamples::initDisplay()
{
	ui.lwSamples->clear();

	QString directory = KStandardDirs::locateLocal("appdata", QString("stored/"));
	QDir d(directory);
	QStringList storedUsers = d.entryList(QDir::Dirs|QDir::NoDotAndDotDot);

	foreach (const QString& user, storedUsers)
	{
		bool ok = true;
		qint32 userId = user.toInt(&ok);
		if (!ok) continue;

		kDebug() << "Found stored user: " << user;

		d.cd(directory+"/"+user);
		QStringList models = d.entryList(QDir::Dirs|QDir::NoDotAndDotDot);
		foreach (const QString& model, models)
		{
			if (!QFile::exists(directory+"/"+user+"/"+model+"/profile.ini")) continue;
			kDebug() << "   Found stored samples: " << model;
			QListWidgetItem* item = new QListWidgetItem(QString("%1: %2").arg(user).arg(model), ui.lwSamples);
			item->setData(Qt::UserRole, directory+"/"+user+"/"+model+"/");
			item->setData(Qt::UserRole+1, userId);
			item->setData(Qt::UserRole+2, model);
			ui.lwSamples->addItem(item);
		}
	}
}


