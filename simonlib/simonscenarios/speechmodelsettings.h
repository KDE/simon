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

#ifndef SIMON_SPEECHMODELSETTINGS_H_AA56F9F6811E4A39949D79E491A2C84C
#define SIMON_SPEECHMODELSETTINGS_H_AA56F9F6811E4A39949D79E491A2C84C

#include "ui_trainingsettings.h"
#include "ui_languageprofilesettings.h"
#include "ui_modelsettingsdlg.h"

#include <KCModule>
#include <QVariantList>

/**
  @author Peter Grasch <bedahr@gmx.net>
*/

class SpeechModelSettings : public KCModule
{
  Q_OBJECT
  private:
    Ui::TrainingSettingsWidget uiTrainingsData;
    Ui::LanguageProfileSettingsWidget uiLanguageProfile;

    Ui::ModelDlg ui;
    QString lastDirectory;
    QString m_hmmDefsToImport;
    QString m_tiedlistToImport;
    QString m_macrosToImport;
    QString m_statsToImport;
    QString m_languageProfileToImport;

    int m_storedModelType;
    void touchLanguageProfileDate();
    
    QString translateDefault(const QString& in);
    
  signals:
    void changed(bool);
    
  private slots:
    void slotChanged();
    void loadLanguageProfile();
    void loadBaseHMM();
    void loadBaseTiedlist();
    void loadBaseMacros();
    void loadBaseStats();
    void displayHelp();
    
  public slots:
    void load();
    void save();
    void defaults();

  public:
    explicit SpeechModelSettings(QWidget* parent, const QVariantList& args=QVariantList());

    ~SpeechModelSettings();

};
#endif
