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

#include "avatarconfiguration.h"
#include "createavatardialog.h"
#include "ui_avatarconfiguration.h"
#include <simondialogengine/avatar.h>
#include <simondialogengine/avatarmodel.h>
#include <KDebug>
#include <KMessageBox>

AvatarConfiguration::AvatarConfiguration(QWidget *parent) : QWidget(parent),
  ui(new Ui::AvatarConfigurationDlg()),
  avatarModel(0)
{
  ui->setupUi(this);
  ui->pbAddAvatar->setIcon(KIcon("list-add"));
  ui->pbEditAvatar->setIcon(KIcon("document-edit"));
  ui->pbRemoveAvatar->setIcon(KIcon("list-remove"));
  
  connect(ui->pbAddAvatar, SIGNAL(clicked()), this, SLOT(addAvatar()));
  connect(ui->pbEditAvatar, SIGNAL(clicked()), this, SLOT(editAvatar()));
  connect(ui->pbRemoveAvatar, SIGNAL(clicked()), this, SLOT(removeAvatar()));
}

QDomElement AvatarConfiguration::serialize(QDomDocument* doc)
{
  return avatarModel->serialize(doc);
}

bool AvatarConfiguration::deSerialize(const QDomElement& elem)
{
  QDomElement avatarsElement = elem.firstChildElement("avatars");
  if (!avatarModel)
  {
    avatarModel = AvatarModel::createInstance(avatarsElement);
    if (!avatarModel)
    {
      kDebug() << "Error loading avatars";
      return false;
    }
    ui->lvAvatars->setModel(avatarModel);
  } else
    avatarModel->deSerialize(avatarsElement);
  return true;
}

void AvatarConfiguration::defaults()
{
  delete avatarModel;
  avatarModel = new AvatarModel();
}
  
Avatar* AvatarConfiguration::getCurrentAvatar()
{
  QModelIndex currentIndex = ui->lvAvatars->currentIndex();
  if (!currentIndex.isValid()) return 0;
  
  return static_cast<Avatar*>(currentIndex.internalPointer());
}

Avatar* AvatarConfiguration::getCurrentAvatarGraphical()
{
  Avatar *a = getCurrentAvatar();
  if (!a)
    KMessageBox::information(this, i18n("Please select an existing avatar from the list or add a new one as appropriate."));
  return a;
}

void AvatarConfiguration::editAvatar()
{
  Avatar *a = getCurrentAvatarGraphical();
  CreateAvatarDialog *dlg = new CreateAvatarDialog(this);
  dlg->editAvatar(a);
  delete dlg;
}
void AvatarConfiguration::removeAvatar()
{
  Avatar *a = getCurrentAvatarGraphical();
  if (KMessageBox::questionYesNoCancel(this, i18n("Do you really want to remove the avatar \"%1\" from your dialog?", a->name())) == KMessageBox::Yes)
  {
    if (!avatarModel->removeAvatar(a))
      KMessageBox::sorry(this, i18n("Failed to remove avatar."));
    else delete a;
  }
}

void AvatarConfiguration::addAvatar()
{
  CreateAvatarDialog *dlg = new CreateAvatarDialog(this);
  dlg->addAvatar(avatarModel);
  delete dlg;
}

QModelIndex AvatarConfiguration::getAvatarIndex ( int id )
{
  return avatarModel->getAvatarIndex(id);
}

Avatar* AvatarConfiguration::getAvatar ( int id ) const
{
  return avatarModel->getAvatar(id);
}

