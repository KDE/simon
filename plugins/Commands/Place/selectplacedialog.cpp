/*
 *   Copyright (C) 2008 Peter Grasch <peter.grasch@bedahr.org>
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

#include "selectplacedialog.h"
#include "placecommand.h"

#include <QUrl>
#include <QDir>

SelectPlaceDialog::SelectPlaceDialog(QWidget *parent) : KDialog(parent)
{
  QWidget *widget = new QWidget( this );
  ui.setupUi(widget);
//PORTING: Verify that widget was added to mainLayout: //PORTING: Verify that widget was added to mainLayout: //PORTING: Verify that widget was added to mainLayout:   setMainWidget( widget );
// Add mainLayout->addWidget(widget); if necessary
// Add mainLayout->addWidget(widget); if necessary
// Add mainLayout->addWidget(widget); if necessary
  setWindowTitle( i18n("Select Place") );

  ui.urLocalPlaceUrl->setMode(KFile::Directory | KFile::ExistingOnly | KFile::LocalOnly);
  ui.urLocalFileUrl->setMode(/*KFile::Directory |*/ KFile::File | KFile::ExistingOnly);

  connect(ui.cbProtocol->lineEdit(), SIGNAL(textEdited(QString)), this, SLOT(buildRemoteUrl()));
  connect(ui.leUser, SIGNAL(textEdited(QString)), this, SLOT(buildRemoteUrl()));
  connect(ui.lePass, SIGNAL(textEdited(QString)), this, SLOT(buildRemoteUrl()));
  connect(ui.leHost, SIGNAL(textEdited(QString)), this, SLOT(buildRemoteUrl()));
  connect(ui.lePath, SIGNAL(textEdited(QString)), this, SLOT(buildRemoteUrl()));
  connect(ui.cbAuthentification, SIGNAL(toggled(bool)), this, SLOT(buildRemoteUrl()));
  connect(ui.leRemoteUrl, SIGNAL(textEdited(QString)), this, SLOT(parseRemoteUrl()));

  connect(ui.rbLocalPlace, SIGNAL(toggled(bool)), this, SLOT(checkComplete()));
  connect(ui.rbLocalFile, SIGNAL(toggled(bool)), this, SLOT(checkComplete()));
  connect(ui.leRemoteUrl, SIGNAL(textChanged(QString)), this, SLOT(checkComplete()));
  connect(ui.urLocalPlaceUrl, SIGNAL(textChanged(QString)), this, SLOT(checkComplete()));
  connect(ui.urLocalFileUrl, SIGNAL(textChanged(QString)), this, SLOT(checkComplete()));
}


void SelectPlaceDialog::checkComplete()
{
  enableButton(KDialog::Ok,isComplete());
}


PlaceCommand* SelectPlaceDialog::selectPlace()
{
  initializeDialog();
  checkComplete();
  if ((!exec()) || (!isComplete()))
    return 0;

  QString iconSrc;
  QString description;
  if (ui.rbLocalPlace->isChecked()) {
    iconSrc = "folder";
    description = i18nc("%1 is a path", "Open local path: %1", ui.urLocalPlaceUrl->url().path());
  }
  else if (ui.rbLocalFile->isChecked()) {
    iconSrc = "folder";
    description = i18nc("%1 is a path", "Open local file: %1", ui.urLocalFileUrl->url().path());
  }
  else {
    iconSrc = "internet-web-browser";
    description = i18nc("%1 is a URL", "Open remote URL: %1", ui.leRemoteUrl->text());
  }

  return new PlaceCommand(getName(), iconSrc, description, getUrl());
}


void SelectPlaceDialog::initializeDialog()
{
  ui.gbRemoteHelp->setChecked(false);

  ui.urLocalPlaceUrl->hide();
  ui.urLocalFileUrl->hide();
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
  ui.rbLocalFile->setChecked(false);
  ui.rbRemotePlace->setChecked(false);
}


bool SelectPlaceDialog::isComplete() const
{
  bool complete=false;
  if (ui.rbLocalPlace->isChecked()) {
                                                  //local place
    complete = ! (ui.urLocalPlaceUrl->url().isEmpty());
  }
  else if (ui.rbLocalFile->isChecked()) {
                                                  //local file
    complete = ! (ui.urLocalFileUrl->url().isEmpty());
  }
  else {
                                                  //remote place
    complete = ! (ui.leRemoteUrl->text().isEmpty());
  }
  return ( ui.rbLocalPlace->isChecked() || ui.rbLocalFile->isChecked()
    || ui.rbRemotePlace->isChecked() ) && complete;
}


void SelectPlaceDialog::buildRemoteUrl()
{
  QUrl url;
  url.setScheme(ui.cbProtocol->currentText());
  url.setHost(ui.leHost->text());
  url.setPath(ui.lePath->text());
  url.setUserName(ui.leUser->text());
  url.setPassword(ui.lePass->text());
  QString urlStr = url.toDisplayString();               //toString((!ui.cbAuthentification->isChecked()) ? QUrl::RemoveUserInfo : QUrl::None);
  ui.leRemoteUrl->setText(urlStr);
}


QString SelectPlaceDialog::getName() const
{
  if (ui.rbLocalPlace->isChecked()) {
                                                  //local place
    return QDir(ui.urLocalPlaceUrl->url().path()).dirName();
  }
  else if (ui.rbLocalFile->isChecked()) {
                                                  //local file
    return QDir(ui.urLocalFileUrl->url().path()).dirName();
  }
  else {
                                                  //remote place
    QString name = QUrl(ui.leRemoteUrl->text()).host();

    if (name.isEmpty())
      name = ui.leRemoteUrl->text();

    return name;
  }
}


void SelectPlaceDialog::parseRemoteUrl()
{
  QUrl url(ui.leRemoteUrl->text());
  ui.cbProtocol->setEditText(url.scheme());
  ui.leHost->setText(url.host());
  ui.lePath->setText(url.path());

  if (!url.userInfo().isEmpty()) {
    ui.cbAuthentification->setChecked(true);
    ui.leUser->setText(url.userName());
    ui.lePass->setText(url.password());
  } else ui.cbAuthentification->setChecked(false);
}


QUrl SelectPlaceDialog::getUrl() const
{
  if (ui.rbLocalPlace->isChecked()) {
                                                  //local place
    return ui.urLocalPlaceUrl->url();
  }
  else if (ui.rbLocalFile->isChecked()) {
                                                  //local file
    return ui.urLocalFileUrl->url();
  }
  else {
                                                  //remote place

    QUrl url(ui.leRemoteUrl->text());
    if (url.scheme().isEmpty()) { //QT5TODO: was url.protocol()
      url = "http://"+ui.leRemoteUrl->text();     //default to http
    }

    return url;
  }
}
