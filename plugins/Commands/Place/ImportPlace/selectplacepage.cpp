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


#include "selectplacepage.h"

#include <KUrl>
#include <QCoreApplication>

SelectPlacePage::SelectPlacePage(QWidget *parent) : QWizardPage(parent)
{
	ui.setupUi(this);

	setTitle(i18n("Select Place"));
	
	ui.urLocalUrl->setMode(KFile::Directory | KFile::File | KFile::ExistingOnly);
	
	connect(ui.cbProtocol->lineEdit(), SIGNAL(textEdited(QString)), this, SLOT(buildRemoteUrl()));
	connect(ui.leUser, SIGNAL(textEdited(QString)), this, SLOT(buildRemoteUrl()));
	connect(ui.lePass, SIGNAL(textEdited(QString)), this, SLOT(buildRemoteUrl()));
	connect(ui.leHost, SIGNAL(textEdited(QString)), this, SLOT(buildRemoteUrl()));
	connect(ui.lePath, SIGNAL(textEdited(QString)), this, SLOT(buildRemoteUrl()));
	connect(ui.cbAuthentification, SIGNAL(toggled(bool)), this, SLOT(buildRemoteUrl()));
	connect(ui.leRemoteUrl, SIGNAL(textEdited(QString)), this, SLOT(parseRemoteUrl()));
	
	connect(ui.rbLocalPlace, SIGNAL(toggled(bool)), this, SIGNAL(completeChanged()));
	connect(ui.leRemoteUrl, SIGNAL(textChanged(QString)), this, SIGNAL(completeChanged()));
	connect(ui.urLocalUrl, SIGNAL(textChanged(QString)), this, SIGNAL(completeChanged()));
	
}

void SelectPlacePage::initializePage()
{
	ui.gbRemoteHelp->setChecked(false);
	
	ui.urLocalUrl->hide();
	ui.leRemoteUrl->hide();
	ui.lbRemoteUrl->hide();
	ui.gbRemoteHelp->hide();
	ui.lbProtocol->hide();
	ui.cbProtocol->hide();
	ui.lbHost->hide();
	ui.leHost->hide();
	ui.lbPath->hide();
	ui.lePath->hide();
	ui.cbAuthentification->hide();
	ui.leUser->hide();
	ui.lbUser->hide();
	ui.lbPassword->hide();
	ui.lePass->hide();
	ui.rbLocalPlace->setChecked(false);
	ui.rbRemotePlace->setChecked(false);
}

bool SelectPlacePage::isComplete() const
{
	bool complete=false;
	if (ui.rbLocalPlace->isChecked())
	{ //local place
		complete = ! (ui.urLocalUrl->url().isEmpty());
	} else
	{ //remote place
		complete = ! (ui.leRemoteUrl->text().isEmpty());
	}
	return (ui.rbLocalPlace->isChecked() || ui.rbRemotePlace->isChecked()) && complete;
}

void SelectPlacePage::buildRemoteUrl()
{
	KUrl url;
	url.setScheme(ui.cbProtocol->currentText());
	url.setHost(ui.leHost->text());
	url.setPath(ui.lePath->text());
	url.setUserName(ui.leUser->text());
	url.setPassword(ui.lePass->text());
	QString urlStr = url.prettyUrl(); //toString((!ui.cbAuthentification->isChecked()) ? KUrl::RemoveUserInfo : KUrl::None);
	ui.leRemoteUrl->setText(urlStr);
}

QString SelectPlacePage::getName() const
{
	if (ui.rbLocalPlace->isChecked())
	{ //local place
		return QDir(ui.urLocalUrl->url().path()).dirName();
	} else
	{ //remote place
		return KUrl(ui.leRemoteUrl->text()).host();
	}
}

void SelectPlacePage::parseRemoteUrl()
{
	KUrl url(ui.leRemoteUrl->text());
	ui.cbProtocol->setEditText(url.scheme());
	ui.leHost->setText(url.host());
	ui.lePath->setText(url.path());
	
	if (!url.userInfo().isEmpty())
	{
		ui.cbAuthentification->setChecked(true);
		ui.leUser->setText(url.userName());
		ui.lePass->setText(url.password());
	} else ui.cbAuthentification->setChecked(false);
}

KUrl SelectPlacePage::getUrl() const
{
	if (ui.rbLocalPlace->isChecked())
	{ //local place
		return ui.urLocalUrl->url();
	} else
	{ //remote place
		return KUrl(ui.leRemoteUrl->text());
	}
}
