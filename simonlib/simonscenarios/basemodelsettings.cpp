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


#include "basemodelsettings.h"
#include <QDomDocument>
#include <QDomElement>
#include <QFile>
#include <KTar>
#include <KStandardDirs>
#include <KMessageBox>
#include <KLocalizedString>

BaseModelSettings::BaseModelSettings ( QWidget* parent, Qt::WFlags flags ) : KDialog ( parent, flags)
{
  QWidget *w = new QWidget(this);
  ui.setupUi(w);
  setMainWidget(w);
  slotCompleteChanged();
  connect(ui.leName, SIGNAL(textChanged(const QString&)), this, SLOT(slotCompleteChanged()));
  connect(ui.urHMM, SIGNAL(textChanged(const QString&)), this, SLOT(slotCompleteChanged()));
  connect(ui.urTiedlist, SIGNAL(textChanged(const QString&)), this, SLOT(slotCompleteChanged()));
  connect(ui.urMacros, SIGNAL(textChanged(const QString&)), this, SLOT(slotCompleteChanged()));
  connect(ui.urStats, SIGNAL(textChanged(const QString&)), this, SLOT(slotCompleteChanged()));
}

void BaseModelSettings::slotCompleteChanged()
{
  button(Ok)->setEnabled(!ui.leName->text().isEmpty() &&
                         QFile::exists(ui.urHMM->url().toLocalFile()) && 
                         QFile::exists(ui.urTiedlist->url().toLocalFile()) && 
                         QFile::exists(ui.urMacros->url().toLocalFile()) && 
                         QFile::exists(ui.urStats->url().toLocalFile()));
}

QString BaseModelSettings::buildModel()
{
  if (!exec()) return QString();
  
  QDomDocument doc;
  QDomElement rootElem = doc.createElement("baseModel");
  
  QDomElement nameElem = doc.createElement("name");
  nameElem.appendChild(doc.createTextNode(ui.leName->text()));
  
  QDomElement creationDateElem = doc.createElement("creationDate");
  creationDateElem.appendChild(doc.createTextNode(ui.dcbCreationDate->dateTime().toString(Qt::ISODate)));
  
  QDomElement typeElem = doc.createElement("type");
  typeElem.appendChild(doc.createTextNode("HTK")); //htk specific... duh
  
  rootElem.appendChild(nameElem);
  rootElem.appendChild(creationDateElem);
  rootElem.appendChild(typeElem);
  doc.appendChild(rootElem);
  
  QString dest = KStandardDirs::locateLocal("tmp", "basemodel.sbm");
  KTar archive(dest, "application/x-gzip");
  if (!archive.open(QIODevice::WriteOnly)) {
    KMessageBox::sorry(this, i18nc("%1 is path", "Failed to create temporary archive at %1", dest));
    return QString();
  }
  
  QByteArray metadata = doc.toByteArray();
  archive.writeFile("metadata.xml", "nobody", "nobody", metadata.constData(), metadata.length());
  
  //htk specific
  archive.addLocalFile(ui.urHMM->url().toLocalFile(), "hmmdefs");
  archive.addLocalFile(ui.urTiedlist->url().toLocalFile(), "tiedlist");
  archive.addLocalFile(ui.urMacros->url().toLocalFile(), "macros");
  archive.addLocalFile(ui.urStats->url().toLocalFile(), "stats");
  //end htk specific
  
  if (!archive.close()) {
    KMessageBox::sorry(this, i18nc("%1 is path", "Failed to store temporary base model archive at %1", dest));
    return QString();
  }
  return dest;
}
