/*
 *   Copyright (C) 2012 Peter Grasch <grasch@simon-listens.org>
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


#ifndef SIMON_BASEMODELSETTINGS_H
#define SIMON_BASEMODELSETTINGS_H

#include "simonmodelmanagement_export.h"
#include <KDialog>

namespace Ui
{
  class ModelDlg;
}

class MODELMANAGEMENT_EXPORT BaseModelSettings : public QWidget
{
Q_OBJECT
public:
  explicit BaseModelSettings ( QWidget* parent = 0, Qt::WFlags flags = 0 );
  void setMinimal(bool minimal);
  
signals:
  void changed(bool);
  
public slots:
  void load();
  void save();
  void defaults();
    
private slots:
  void slotChanged();
  void createBaseModel();
  void baseModelSelectionChanged();
  void openBaseModel();
  void getNewBaseModels();
  void exportBaseModel();
  void removeBaseModel();
  
private:
  Ui::ModelDlg *ui;

  int m_storedModelType;
  
  void importBaseModel(const QString& path);
  QString baseModelDescription(const QString& path);
  QString baseModelDescription(const QString& name, const QDateTime& dateTime);

  void setupBaseModelSelection();
  void addBaseModelToSelection(const QString& path);
};

#endif // BASEMODELSETTINGS_H
