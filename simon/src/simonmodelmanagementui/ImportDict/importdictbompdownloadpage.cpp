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

#include "importdictbompdownloadpage.h"
#include "speechmodelmanagementuiconfiguration.h"
#include <KArchive/KFilterDev>
#include <QFile>
#include <QDebug>

#include <kio/job.h>
#include <kio/jobuidelegate.h>
#include <KWidgetsAddons/KMessageBox>
#include <KJobWidgets>
#include <QDir>


/**
 * \brief Constructor - inits the GUI
 * \author Peter Grasch
 * @param parent Parent of the page
 */
ImportDictBOMPDownloadPage::ImportDictBOMPDownloadPage(QWidget* parent): QWizardPage(parent)
{
  ui.setupUi(this);

  connect(ui.leName, SIGNAL(textChanged(QString)), this, SIGNAL(completeChanged()));
  connect(ui.leMail, SIGNAL(textChanged(QString)), this, SIGNAL(completeChanged()));
  connect(ui.leMail, SIGNAL(textChanged(QString)), this, SIGNAL(completeChanged()));
  connect(ui.cbUnderstoodLicence, SIGNAL(toggled(bool)), this, SIGNAL(completeChanged()));
  registerField("bompUserName*", ui.leName, "text", SIGNAL(textChanged(QString)));
  registerField("bompEMail*", ui.leMail, "text", SIGNAL(textChanged(QString)));
  registerField("bompLicenceAccepted*", ui.cbUnderstoodLicence);

  setTitle(i18n("Download HADIFIX BOMP"));
}


void ImportDictBOMPDownloadPage::initializePage()
{
  QString bompBaseUrl = SpeechModelManagementUiConfiguration::bompDownloadBase()+'/';

  QString localLicencePath = QDir::tempPath() + QLatin1Char('/') +  "bomp_license";
  KIO::FileCopyJob *job = KIO::file_copy(QUrl(bompBaseUrl+"bomp_license.php"),
    QUrl::fromLocalFile(localLicencePath), -1, KIO::Overwrite);
  KJobWidgets::setWindow(job, this);

  if (!job->exec()) {
    job->ui()->showErrorMessage();
    return;
  }

  //read license and set fields
  QFile f(localLicencePath);
  if (!f.open(QIODevice::ReadOnly)) {
    KMessageBox::sorry(this, i18n("Could not retrieve BOMP license terms."));
    return;
  }

  bompUrl = bompBaseUrl+QString::fromUtf8(f.readLine()).trimmed();
  acceptTag = QString::fromUtf8(f.readLine()).trimmed();
  QString license = QString::fromUtf8(f.readAll());

  ui.teLicence->setText(license);
  QFile::remove(localLicencePath);
}


bool ImportDictBOMPDownloadPage::validatePage()
{
  if (bompUrl.isNull()) return false;

  //download bomp lexicon
  QString tempBompPath = QDir::tempPath() + QLatin1Char('/') +  "bomp";
  QUrl fullUrl(bompUrl);
  fullUrl.addQueryItem("iAcceptTheLicence", acceptTag);
  fullUrl.addQueryItem("name", ui.leName->text());
  fullUrl.addQueryItem("mail", ui.leMail->text());
  KIO::FileCopyJob *job = KIO::file_copy(fullUrl,
    QUrl::fromLocalFile(tempBompPath), -1, KIO::Overwrite);
  KJobWidgets::setWindow(job, this);

  if (!job->exec()) {
    job->ui()->showErrorMessage();
    return false;
  }

  return true;
}


/**
 * \brief Destructor
 * \author Peter Grasch
 */
ImportDictBOMPDownloadPage::~ImportDictBOMPDownloadPage()
{
}
