/*   Copyright (C) 2010 Grasch Peter <peter.grasch@bedahr.org>
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

#include "akonadiconfiguration.h"
#include "akonadicommandmanager.h"
#include <simonlogging/logger.h>

#include <QVariantList>
#include <QString>
#include <QProgressBar>
#include <simonscenarios/scenario.h>
#include <kgenericfactory.h>
#include <KAboutData>
#include <KMessageBox>
#include <simondialogengine/avatar.h>
#include <simondialogengine/confui/templateoptionsconfiguration.h>
#include <simondialogengine/confui/boundvaluesconfiguration.h>
#include <simondialogengine/confui/avatarconfiguration.h>
#include <simondialogengine/confui/outputconfiguration.h>

#include <akonadi/control.h>
#include <akonadi/collection.h>
#include <akonadi/monitor.h>
#include <akonadi/collectionfetchjob.h>
#include <akonadi/collectionfetchscope.h>
#include <kcalcore/event.h>
#include <KProgressDialog>
#include <simondialogengine/avatarmodel.h>

K_PLUGIN_FACTORY_DECLARATION(AkonadiCommandPluginFactory)

AkonadiConfiguration::AkonadiConfiguration(AkonadiCommandManager *manager, Scenario *parent, const QVariantList &args)
: CommandConfiguration(parent,  "akonadi", ki18n( "Akonadi" ),
  "0.1", ki18n("Integrate with the personal data manager akonadi"),
  "akonadi",
  AkonadiCommandPluginFactory::componentData()),
  m_manager(manager),
  fetchCollectionsDialog(0),
  boundValuesConfig(new BoundValuesConfiguration(this)),
  templateOptionsConfig(new TemplateOptionsConfiguration(this)),
  avatarsConfig(new AvatarConfiguration(this)),
  outputConfiguration(new OutputConfiguration(this))
{
  Q_UNUSED(args);
  ui.setupUi(this);
  connect(ui.cbExecuteAkonadiRequests, SIGNAL(clicked(bool)), this, SLOT(uncheckAkonadiCommandRequests()));

  ui.twDialog->addTab(boundValuesConfig, i18n("Bound values"));
  ui.twDialog->addTab(templateOptionsConfig, i18n("Template options"));
  ui.twDialog->addTab(avatarsConfig, i18n("Avatars"));
  ui.twDialog->addTab(outputConfiguration, i18n("Output"));
}

void AkonadiConfiguration::selectedCollectionChanged()
{
  collectionIndexToSelect = ui.cbCollection->itemData(ui.cbCollection->currentIndex()).toInt();
}

void AkonadiConfiguration::showEvent(QShowEvent* event)
{
  QWidget::showEvent(event);
  initCollections();
}

void AkonadiConfiguration::initCollections()
{
  if (!fetchCollectionsDialog)
  {
    fetchCollectionsDialog = new KProgressDialog(this, i18n("Akonadi Plug-in"), i18n("Fetching collections from akonadi..."));
  
    fetchCollectionsDialog->progressBar()->setRange(0,0);
    fetchCollectionsDialog->progressBar()->setValue(0);
  }
  
  fetchCollectionsDialog->show();
  
  disconnect(ui.cbCollection, SIGNAL(currentIndexChanged(int)), this, SLOT(selectedCollectionChanged()));
  
  ui.cbCollection->clear();
  Akonadi::CollectionFetchJob *job = new Akonadi::CollectionFetchJob( Akonadi::Collection::root(), Akonadi::CollectionFetchJob::Recursive, this );
  connect(job, SIGNAL(finished(KJob*)), this, SLOT(collectionJobFinished(KJob*)));
  job->fetchScope().setContentMimeTypes(QStringList() << KCalCore::Event::eventMimeType());
}

void AkonadiConfiguration::collectionJobFinished(KJob* job)
{
  Akonadi::CollectionFetchJob *fetchJob = static_cast<Akonadi::CollectionFetchJob*>( job );
  if ( job->error() ) {
      Logger::log(i18nc("%1 is error string", "Job returned error: %1", job->errorString()), Logger::Error);
      kError() << job->errorString();
      return;
  }
  Akonadi::Collection::List collections = fetchJob->collections();
  foreach (const Akonadi::Collection& collection, collections)
  {
    kDebug() << "Collection: " << collection.name() << collection.id();
    ui.cbCollection->addItem(collection.name(), collection.id());
  }
  
  int collectionIndex = ui.cbCollection->findData(collectionIndexToSelect);
  ui.cbCollection->setCurrentIndex(collectionIndex);
  connect(ui.cbCollection, SIGNAL(currentIndexChanged(int)), this, SLOT(selectedCollectionChanged()));
  
  fetchCollectionsDialog->hide();
}

QDomElement AkonadiConfiguration::serialize(QDomDocument* doc)
{
  QDomElement configElem = doc->createElement("config");
  
  QDomElement collectionElem = doc->createElement("collection");
  collectionElem.appendChild(doc->createTextNode(
    QString::number(collectionIndexToSelect)));
  configElem.appendChild(collectionElem);
  
  QDomElement alarmsElem = doc->createElement("displayAlarms");
  alarmsElem.setAttribute("enabled", ui.cbDisplayAlarms->isChecked() ? "1" : "0");
  
  QDomElement alarmTextElem = doc->createElement("text");
  alarmTextElem.appendChild(doc->createTextNode(ui.teText->toPlainText()));
  alarmsElem.appendChild(alarmTextElem);
  
  QDomElement alarmAvatarElem = doc->createElement("displayAvatar");
  alarmAvatarElem.setAttribute("enabled", ui.cbDisplayAvatar->isChecked());
  
  int avatarId = getSelectedAvatar();
  alarmAvatarElem.appendChild(doc->createTextNode(QString::number(avatarId)));
  alarmsElem.appendChild(alarmAvatarElem);
  
  QDomElement alarmOptionsElem = doc->createElement("options");
  
  QDomElement alarmDismissElem = doc->createElement("dismiss");
  alarmDismissElem.setAttribute("enabled", ui.cbDismiss->isChecked() ? "1" : "0");
  alarmDismissElem.appendChild(doc->createTextNode(ui.leDismiss->text()));
  alarmOptionsElem.appendChild(alarmDismissElem);
  QDomElement alarmShowLaterElem = doc->createElement("showLater");
  alarmShowLaterElem.setAttribute("enabled", ui.cbShowLater->isChecked() ? "1" : "0");
  alarmShowLaterElem.setAttribute("delay", QString::number(ui.wgRestartTime->getTime()));
  alarmShowLaterElem.appendChild(doc->createTextNode(ui.leShowLater->text()));
  alarmOptionsElem.appendChild(alarmShowLaterElem);
  
  alarmsElem.appendChild(alarmOptionsElem);
  
  configElem.appendChild(alarmsElem);
  
  QDomElement executeAkonadiCommandsElem = doc->createElement("executeAkonadiCommands");
  executeAkonadiCommandsElem.setAttribute("enabled", ui.cbExecuteAkonadiRequests->isChecked() ? "1" : "0");
  executeAkonadiCommandsElem.setAttribute("trigger", ui.leAkonadiPrefix->text());
  configElem.appendChild(executeAkonadiCommandsElem);
  
  
  configElem.appendChild(templateOptionsConfig->serialize(doc));

  configElem.appendChild(boundValuesConfig->serialize(doc));

  QDomElement avatarsElem = avatarsConfig->serialize(doc);
  configElem.appendChild(avatarsElem);

  configElem.appendChild(outputConfiguration->serialize(doc));
  

  m_manager->parseConfiguration();
  return configElem;
}

int AkonadiConfiguration::getSelectedAvatar() const
{
  QModelIndex currentSelectedAvatar = avatarsConfig->getModel()->index(ui.cbAvatar->currentIndex(), 0);
  if (currentSelectedAvatar.isValid()) {
    Avatar *a = static_cast<Avatar*>(currentSelectedAvatar.internalPointer());
    return a->id();
  }
  return -1;
}

void AkonadiConfiguration::uncheckAkonadiCommandRequests()
{
  if (ui.cbExecuteAkonadiRequests->isChecked())
    return;
  
  KMessageBox::information(this, i18n("Without this option, you cannot use commands to schedule events.\n\n"
                    "You can still configure them through simon but triggering them will not have any effect."),
                    QString(), i18n("Do not show this info again"));
}

bool AkonadiConfiguration::deSerialize(const QDomElement& elem)
{
  QDomElement collectionElem = elem.firstChildElement("collection");
  
  bool ok = true;
  int storedCollection = collectionElem.text().toInt(&ok);
  if (collectionElem.isNull() || !ok)
  {
    defaults();
    return true;
  }
  collectionIndexToSelect = storedCollection;
  
  QDomElement alarmsElem = elem.firstChildElement("displayAlarms");
  ui.cbDisplayAlarms->setChecked(alarmsElem.attribute("enabled") == "1");
  
  ui.teText->setText(alarmsElem.firstChildElement("text").text());
  
  QDomElement alarmOptionsElem = alarmsElem.firstChildElement("options");
  QDomElement dismissElement = alarmOptionsElem.firstChildElement("dismiss");
  
  ui.cbDismiss->setChecked(dismissElement.attribute("enabled") == "1");
  ui.leDismiss->setText(dismissElement.text());
  
  QDomElement showLaterElem = alarmOptionsElem.firstChildElement("showLater");
  ui.cbShowLater->setChecked(showLaterElem.attribute("enabled") == "1");
  ui.leShowLater->setText(showLaterElem.text());
  
  ui.wgRestartTime->setTime(showLaterElem.attribute("delay").toInt());
  
  QDomElement executeAkonadiCommandsElem = elem.firstChildElement("executeAkonadiCommands");
  ui.cbExecuteAkonadiRequests->setChecked(executeAkonadiCommandsElem.attribute("enabled") == "1");
  ui.leAkonadiPrefix->setText(executeAkonadiCommandsElem.attribute("trigger"));
  
  if (!outputConfiguration->deSerialize(elem))
    return false;
  
  if (!templateOptionsConfig->deSerialize(elem))
    return false;

  if (!boundValuesConfig->deSerialize(elem))
    return false;

  if (!avatarsConfig->deSerialize(elem))
    return false;
  
  ui.cbAvatar->setModel(avatarsConfig->getModel());
  
  QDomElement selectedAvatar = alarmsElem.firstChildElement("displayAvatar");
  ui.cbDisplayAvatar->setChecked(selectedAvatar.attribute("enabled") == "1");
  int row = ui.cbAvatar->findData(selectedAvatar.text().toInt(), Qt::UserRole);
  ui.cbAvatar->setCurrentIndex(row);
  
  return true;
}


void AkonadiConfiguration::defaults()
{
  ui.cbDisplayAlarms->setChecked(false);
  ui.cbExecuteAkonadiRequests->setChecked(true);
  ui.leAkonadiPrefix->setText("[simon-command]");
  avatarsConfig->defaults();
  templateOptionsConfig->defaults();
  outputConfiguration->defaults();
  boundValuesConfig->defaults();
  
  ui.cbDismiss->setChecked(true);
  ui.cbShowLater->setChecked(false);
  ui.leDismiss->setText(i18n("OK"));
  ui.leShowLater->setText(i18n("Snooze"));
  ui.wgRestartTime->setTime(AkonadiCommand::Minutes, 5);
  ui.teText->setText(i18nc("%1 is summary, %2 is date, %3 is time and %4 is location", 
                            "Event: %1 (%2 %3)\nLocation: %4")); // it's correct that there are no arguments here! They will be filled in later on
}

QString AkonadiConfiguration::akonadiRequestPrefix()
{
  return ui.leAkonadiPrefix->text();
}

bool AkonadiConfiguration::displayAlarms()
{
  return ui.cbDisplayAlarms->isChecked();
}

bool AkonadiConfiguration::executeAkonadiRequests()
{
  return ui.cbExecuteAkonadiRequests->isChecked();
}

Akonadi::Entity::Id AkonadiConfiguration::getCollection()
{
  return (Akonadi::Entity::Id) collectionIndexToSelect;
}

DialogBoundValues* AkonadiConfiguration::getBoundValues()
{
  return boundValuesConfig->getDialogBoundValues();
}
DialogTemplateOptions* AkonadiConfiguration::getTemplateOptions()
{
  return templateOptionsConfig->getDialogTemplateOptions();
}

QString AkonadiConfiguration::delayText()
{
  return ui.leShowLater->text();
}
QString AkonadiConfiguration::dismissText()
{
  return ui.leDismiss->text();
}
QString AkonadiConfiguration::dialogText()
{
  return ui.teText->toPlainText();
}
bool AkonadiConfiguration::getShowDelay()
{
  return ui.cbShowLater->isChecked();
}
int AkonadiConfiguration::getRestartDelay()
{
  return ui.wgRestartTime->getTime();
}
bool AkonadiConfiguration::getShowDismiss()
{
  return ui.cbDismiss->isChecked();
}

Avatar* AkonadiConfiguration::getAvatar(int id) const
{
  return avatarsConfig->getAvatar(id);
}
bool AkonadiConfiguration::getDisplayAvatar() const
{
  return ui.cbDisplayAvatar->isChecked();
}
int AkonadiConfiguration::getAvatarSize() const
{
  return outputConfiguration->getAvatarSize();
}
bool AkonadiConfiguration::getDisplayAvatarNames() const
{
  return outputConfiguration->getDisplayAvatarNames();
}
QString AkonadiConfiguration::getOptionSeparatorText() const
{
  return outputConfiguration->getOptionSeparatorText();
}
QString AkonadiConfiguration::getRepeatAnnouncement() const
{
  return outputConfiguration->getRepeatAnnouncement();
}
bool AkonadiConfiguration::getRepeatOnInvalidInput() const
{
  return outputConfiguration->getRepeatOnInvalidInput();
}

bool AkonadiConfiguration::useGUIOutput()
{
  return outputConfiguration->useGUIOutput();
}
bool AkonadiConfiguration::useTTSOutput()
{
  return outputConfiguration->useTTSOutput();
}

QStringList AkonadiConfiguration::getRepeatTriggers() const
{
  return outputConfiguration->getRepeatTriggers();
}


AkonadiConfiguration::~AkonadiConfiguration()
{
}
