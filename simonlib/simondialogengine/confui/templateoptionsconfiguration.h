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

#ifndef SIMON_TEMPLATEOPTIONSCONFIGURATION_H 
#define SIMON_TEMPLATEOPTIONSCONFIGURATION_H  

#include "simondialogengineconfui_export.h"
#include <QWidget>
#include <QDomElement>

class TemplateOptionsModel;
class TemplateOptions;
class QDomDocument;
class QModelIndex;
class DialogTemplateOptions;
namespace Ui {
  class TemplateOptionsDlg;
}

class SIMONDIALOGENGINECONFUI_EXPORT TemplateOptionsConfiguration : public QWidget
{
  Q_OBJECT
  private:
    Ui::TemplateOptionsDlg *ui;

    DialogTemplateOptions *templateOptions;

    QString getCurrentTemplateIndex();
    QString getCurrentTemplateIndexGraphical();

  private slots:
    void addTemplateOption();
    void editTemplateOption();
    void removeTemplateOption();
    
    
  public:
    TemplateOptionsConfiguration(QWidget *parent);
    DialogTemplateOptions* getDialogTemplateOptions() { return templateOptions; }

  public slots:
    virtual bool deSerialize(const QDomElement&);
    virtual QDomElement serialize(QDomDocument *doc);
    virtual void defaults();
};

#endif


