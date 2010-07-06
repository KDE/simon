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

	ui.pbDeleteSamples->setIcon(KIcon("edit-delete"));
	ui.pbUploadSamples->setIcon(KIcon("folder-sync"));
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
			ui.lwSamples->addItem(item);
		}
	}
}


