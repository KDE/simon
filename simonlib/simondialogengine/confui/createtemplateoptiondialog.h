/*
 *   Copyright (C) 2010 Peter Grasch <peter.grasch@bedahr.org>
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


#ifndef SIMON_CREATETEMPLATEOPTIONDIALOG_H_4B4956DCAE204C49977297D20CB81F09
#define SIMON_CREATETEMPLATEOPTIONDIALOG_H_4B4956DCAE204C49977297D20CB81F09

#include <QDialog>

namespace Ui
{
  class CreateTemplateOptionDialog;
}

class CreateTemplateOptionDialog : public QDialog
{
  private:
    Ui::CreateTemplateOptionDialog *ui;
  public:
    CreateTemplateOptionDialog(QWidget *parent=0);
    virtual ~CreateTemplateOptionDialog();

    QString getName();
    bool getEnabled();

    void setName(const QString& id);
    void setNameReadOnly(bool readOnly);
    void setEnabled(bool enabled);
};

#endif


