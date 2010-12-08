/*   Copyright (C) 2010 Grasch Peter <grasch@simon-listens.org>
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
#include "avatar.h"
#include "avatarmodel.h"
#include <QFile>
#include <KStandardDirs>
#include <KMessageBox>

CreateAvatarDialog::CreateAvatarDialog(QWidget* parent, Qt::WFlags flags): KDialog(parent, flags), ui(new Ui::CreateAvatarDlg)
{
  QWidget *w = new QWidget(this);
  ui->setupUi(this);
  setMainWidget(w);
}

void CreateAvatarDialog::addAvatar(AvatarModel* model)
{
  if (!KDialog::exec())
    return;
  
  QString name = ui->leName->text();
  QImage image(ui->urImage->url().toLocalFile());
  model->addAvatar(new Avatar(model->getNextId(name), name, image));
}

void CreateAvatarDialog::editAvatar(Avatar* avatar)
{
  QString fileName = KStandardDirs::locateLocal("tmp", "simon_avatar_tmp.png");
  QFile f(fileName);
  
  if (!f.open(QIODevice::WriteOnly))
  {
    KMessageBox::sorry(this, i18n("Couldn't open temporary file at \"%1\".", fileName));
    return;
  }
  
  avatar->image().save(fileName);
  ui->urImage->setUrl(KUrl(fileName));
  
  if (!KDialog::exec())
    return;
  
  avatar->setName(ui->leName->text());
  avatar->setImage(QImage(ui->urImage->url().toLocalFile()));
  
  QFile::remove(fileName);
}

CreateAvatarDialog::~CreateAvatarDialog()
{
  delete ui;
}
