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

#include "templateoptionsconfiguration.h"
#include "createtemplateoptiondialog.h"
#include <simondialogengine/dialogtemplateoptions.h>
#include <QPointer>
#include <KDebug>
#include <KMessageBox>
#include "ui_templateoptionsconfiguration.h"

TemplateOptionsConfiguration::TemplateOptionsConfiguration(QWidget *parent) : QWidget(parent),
  ui(new Ui::TemplateOptionsDlg()),
  templateOptions(0)

{
  ui->setupUi(this);
  ui->pbAddTemplateOption->setIcon(KIcon("list-add"));
  ui->pbRemoveTemplateOption->setIcon(KIcon("list-remove"));
  ui->pbEditTemplateOption->setIcon(KIcon("document-edit"));

  connect(ui->pbAddTemplateOption, SIGNAL(clicked()), this, SLOT(addTemplateOption()));
  connect(ui->pbEditTemplateOption, SIGNAL(clicked()), this, SLOT(editTemplateOption()));
  connect(ui->pbRemoveTemplateOption, SIGNAL(clicked()), this, SLOT(removeTemplateOption()));
}

QDomElement TemplateOptionsConfiguration::serialize(QDomDocument* doc)
{
  return templateOptions->serialize(doc);
}

bool TemplateOptionsConfiguration::deSerialize(const QDomElement& elem)
{
  QDomElement options = elem.firstChildElement("options");
  if (!templateOptions)
  {
    templateOptions = DialogTemplateOptions::createInstance(options);
    if (!templateOptions)
      return false;
    
    ui->tvTemplateOptions->setModel(templateOptions);
  } else {
    templateOptions->deSerialize(options);
  }

  return true;
}

void TemplateOptionsConfiguration::defaults()
{
  delete templateOptions;
  templateOptions = new DialogTemplateOptions();
  ui->tvTemplateOptions->setModel(templateOptions);
}


QString TemplateOptionsConfiguration::getCurrentTemplateIndex()
{
  QModelIndex index = ui->tvTemplateOptions->currentIndex();
  if (!index.isValid())
    return QString();
  QString id = index.data(Qt::UserRole).toString();
  return id;
}

QString TemplateOptionsConfiguration::getCurrentTemplateIndexGraphical()
{
  QString id = getCurrentTemplateIndex();
  if (id.isNull())
    KMessageBox::sorry(this, i18n("Please select a template option."));
  return id;
}

void TemplateOptionsConfiguration::addTemplateOption()
{
  QPointer<CreateTemplateOptionDialog> dialog = new CreateTemplateOptionDialog(this);
  if (dialog->exec())
    templateOptions->addOption(dialog->getName(), dialog->getEnabled());
  if (dialog) dialog->deleteLater();
}

void TemplateOptionsConfiguration::editTemplateOption()
{
  QString id = getCurrentTemplateIndexGraphical();
  if (id.isNull()) return;

  bool currentData = templateOptions->isEnabled(id);

  QPointer<CreateTemplateOptionDialog> dialog = new CreateTemplateOptionDialog(this);

  dialog->setName(id);
  dialog->setNameReadOnly(true);
  dialog->setEnabled(currentData);

  if (dialog->exec())
    //will automatically replace the old value because they share the same id
    templateOptions->addOption(dialog->getName(), dialog->getEnabled());

  if (dialog) dialog->deleteLater();
}

void TemplateOptionsConfiguration::removeTemplateOption()
{
  QString id = getCurrentTemplateIndexGraphical();

  if (id.isNull() || KMessageBox::questionYesNoCancel(this, 
        i18n("Do you really want to remove the selected template option?"))
      != KMessageBox::Yes)
    return;

  templateOptions->removeOption(id);
}
