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

#include "importdictbompdownloadpage.h"
#include "speechmodelmanagementuiconfiguration.h"
#include <QTextCodec>
#include <QScrollBar>
#include <KFilterDev>
#include <KMimeType>
#include <QFile>
#include <KDebug>
#include <KStandardDirs>
#include <KMessageBox>
#include <kio/job.h>
#include <kio/jobuidelegate.h>

/**
 * \brief Constructor - inits the GUI
 * \author Peter Grasch
 * @param parent Parent of the page
 */
ImportDictBOMPDownloadPage::ImportDictBOMPDownloadPage(QWidget* parent): QWizardPage(parent)
{
  ui.setupUi(this);

  connect(ui.leName, SIGNAL(textChanged(const QString&)), this, SIGNAL(completeChanged()));
  connect(ui.leMail, SIGNAL(textChanged(const QString&)), this, SIGNAL(completeChanged()));
  connect(ui.leMail, SIGNAL(textChanged(const QString&)), this, SIGNAL(completeChanged()));
  connect(ui.cbUnderstoodLicence, SIGNAL(toggled(bool)), this, SIGNAL(completeChanged()));
  registerField("bompUserName*", ui.leName, "text", SIGNAL(textChanged (const QString &)));
  registerField("bompEMail*", ui.leMail, "text", SIGNAL(textChanged(const QString &)));
  registerField("bompLicenceAccepted*", ui.cbUnderstoodLicence);

  setTitle(i18n("Download HADIFIX BOMP"));
}


void ImportDictBOMPDownloadPage::initializePage()
{
  QString bompBaseUrl = SpeechModelManagementUiConfiguration::bompDownloadBase()+'/';

  QString localLicencePath = KStandardDirs::locateLocal("tmp", "bomp_licence");
  KIO::FileCopyJob *job = KIO::file_copy(KUrl(bompBaseUrl+"bomp_licence.php"),
    localLicencePath, -1, KIO::Overwrite);
  job->ui()->setWindow(this);

  if (!job->exec()) {
    job->ui()->showErrorMessage();
    return;
  }

  //read licence and set fields
  QFile f(localLicencePath);
  if (!f.open(QIODevice::ReadOnly)) {
    KMessageBox::sorry(this, i18n("Could not retrieve BOMP licence terms."));
    return;
  }

  bompUrl = bompBaseUrl+QString::fromUtf8(f.readLine()).trimmed();
  acceptTag = QString::fromUtf8(f.readLine()).trimmed();
  QString licence = QString::fromUtf8(f.readAll());

  ui.teLicence->setText(licence);
  QFile::remove(localLicencePath);
}


bool ImportDictBOMPDownloadPage::validatePage()
{
  if (bompUrl.isNull()) return false;

  //download bomp lexicon
  QString tempBompPath = KStandardDirs::locateLocal("tmp", "bomp");
  KUrl fullUrl(bompUrl);
  fullUrl.addQueryItem("iAcceptTheLicence", acceptTag);
  fullUrl.addQueryItem("name", ui.leName->text());
  fullUrl.addQueryItem("mail", ui.leMail->text());
  KIO::FileCopyJob *job = KIO::file_copy(fullUrl,
    tempBompPath, -1, KIO::Overwrite);
  job->ui()->setWindow(this);

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
