/*   Copyright (C) 2010 Grasch Peter <peter.grasch@bedahr.org>
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

#include "createavatardialog.h"
#include "ui_createavatardlg.h"
#include <simondialogengine/avatar.h>
#include <simondialogengine/avatarmodel.h>
#include <QFile>
#include <KMessageBox>
#include <QDir>

CreateAvatarDialog::CreateAvatarDialog(QWidget* parent, Qt::WFlags flags): QDialog(parent, flags), ui(new Ui::CreateAvatarDlg)
{
  QWidget *w = new QWidget(this);
  ui->setupUi(this);
  QVBoxLayout *mainLayout = new QVBoxLayout;
  setLayout(mainLayout);
  mainLayout->addWidget(w);
  setWindowTitle(i18n("Avatar"));
  connect(ui->urImage, SIGNAL(returnPressed()), this, SLOT(updateImagePreview()));
}

void CreateAvatarDialog::updateImagePreview()
{
  ui->ifImage->showPreview(ui->urImage->url());
}

bool CreateAvatarDialog::getData(QString& name, QImage& image)
{
  bool dataOk = false;
  do
  {
    if (!QDialog::exec())
      return false;
    name = ui->leName->text();
    image.load(ui->urImage->url().toLocalFile());
    if (name.isEmpty() || image.isNull())
      KMessageBox::information(this, i18n("Please fill in all fields or select cancel."));
    else
      dataOk = true;
  } while (!dataOk);
  return dataOk;
}

void CreateAvatarDialog::addAvatar(AvatarModel* model)
{
  QString name;
  QImage image;
  if (getData(name, image))
    model->addAvatar(new Avatar(model->getNextId(name), name, image));
}

void CreateAvatarDialog::editAvatar(Avatar* avatar)
{
  ui->leName->setText(avatar->name());
  QString fileName = QDir::tempPath() + QLatin1Char('/') +  "simon_avatar_tmp.png";
  QFile f(fileName);

  if (!f.open(QIODevice::WriteOnly))
  {
    KMessageBox::sorry(this, i18n("Could not open temporary file at \"%1\".", fileName));
    return;
  }
  
  avatar->image().save(fileName);
  ui->urImage->setUrl(QUrl(fileName));
  
  updateImagePreview();
  
  QString name;
  QImage image;
  
  if (getData(name, image))
  {
    avatar->setName(name);
    avatar->setImage(image);
  }
  
  QFile::remove(fileName);
}

CreateAvatarDialog::~CreateAvatarDialog()
{
  delete ui;
}
