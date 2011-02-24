/*
 *   Copyright (C) 2010 Peter Grasch <grasch@simon-listens.org>
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


#include "createboundvaluedialog.h"
#include "ui_createboundvalue.h"
#include <simondialogengine/staticboundvalue.h>
#include <simondialogengine/scriptboundvalue.h>
#include <simondialogengine/argumentboundvalue.h>

#ifdef USE_PLASMA
#include <simondialogengine/plasmaboundvalue.h>
#endif

#include <QWidget>

#include <KMessageBox>
#include <KLocalizedString>

#ifdef USE_PLASMA
#include <KIcon>
#include <KService>
#include <KServiceTypeTrader>
#include <Plasma/DataEngine>
#include <Plasma/DataEngineManager>
#endif

CreateBoundValueDialog::CreateBoundValueDialog(QWidget *parent) : KDialog(parent),
  ui(new Ui::CreateBoundValueDialog())
#ifdef USE_PLASMA
  , m_requestingSource(false)
  , m_currentEngine(0)
  , m_engineManager(0)
#endif
{
  QWidget *main = new QWidget(this);
  ui->setupUi(main);
  setMainWidget(main);
  setCaption(i18n("Bound value"));

#ifdef USE_PLASMA
  m_engineManager = Plasma::DataEngineManager::self();
  ui->cbType->addItem(i18n("Plasma data engine"));
  connect(ui->cbDataEngine, SIGNAL(activated(QString)), this, SLOT(initDataEngine()));
  connect(ui->pbRequest, SIGNAL(clicked()), this, SLOT(requestSource()));
#endif
  connect(ui->pbTest, SIGNAL(clicked()), this, SLOT(test()));
}

#ifdef USE_PLASMA
void CreateBoundValueDialog::initPlasma()
{
  ui->cbDataEngine->clear();

  KService::List services;
  KServiceTypeTrader* trader = KServiceTypeTrader::self();

  services = trader->query("Plasma/DataEngine");

  foreach (KService::Ptr service, services) {
    QVariant internalName = service->property("X-KDE-PluginInfo-Name", QVariant::String);
    if (!internalName.isValid()) continue;
    QString parentApp = service->property("X-KDE-ParentApp", QVariant::String).toString();
    if (!parentApp.isEmpty())
    {
      kDebug() << "Skipping application specific data engine: " << internalName;
      continue;
    }
    ui->cbDataEngine->addItem(KIcon(service->icon()), service->name(), internalName.toString());
  }
  ui->cbDataEngine->setCurrentIndex(-1);
  ui->cbDataSource->clear();
}

void CreateBoundValueDialog::initDataEngine()
{
  kDebug() << "Initing data engine...";
  int currentIndex = ui->cbDataEngine->currentIndex();
  if (currentIndex == -1)
  {
    ui->cbDataSource->clear();
    ui->cbKey->clear();
    return;
  }

  if (!m_currentEngineName.isEmpty())
    m_engineManager->unloadEngine(m_currentEngineName);

  ui->cbKey->clear();

  QString internalName = ui->cbDataEngine->itemData(currentIndex).toString();
  kDebug() << "Loading data engine: " << internalName;
  Plasma::DataEngine *engine = m_engineManager->loadEngine(internalName);
  m_currentEngineName = internalName;

  kDebug() << "Engine: " << engine;
  if (!engine) return;

  m_currentEngine = engine;

  //kDebug() << "Source dict: " << sourceDict.count();
  kDebug() << "Available sources: " << engine->sources();
  kDebug() << "Data source valid: " << engine->isValid();
  ui->cbDataSource->clear();

  foreach (const QString& source, engine->sources())
    addPlasmaSource(source);

  connect(engine, SIGNAL(sourceAdded(QString)), this, SLOT(sourceAdded(QString)));
  connect(engine, SIGNAL(sourceRemoved(QString)), this, SLOT(sourceRemoved(QString)));

}

void CreateBoundValueDialog::addPlasmaSource(const QString source)
{
  ui->cbDataSource->addItem(source);
  if (!m_requestingSource || (ui->cbDataSource->currentText() != source))
    m_currentEngine->connectSource(source, this, 0);
}

void CreateBoundValueDialog::sourceAdded(const QString& source)
{
  ui->cbDataSource->addItem(source);
}

void CreateBoundValueDialog::sourceRemoved(const QString& source)
{
  int index = -1;
  while ((index = ui->cbDataSource->findText(source)) != -1)
    ui->cbDataSource->removeItem(index);
  m_currentEngine->disconnectSource(source, this);
}

void CreateBoundValueDialog::requestSource()
{
  QString source = ui->cbDataSource->currentText();

  if (!m_currentEngine || source.isEmpty()) return;

  m_requestingSource = true;
  m_currentEngine->connectSource(source, this, 0 /* no polling */);
  m_requestingSource = false;
}

void CreateBoundValueDialog::dataUpdated(const QString& source, const Plasma::DataEngine::Data& data)
{
  kDebug() << "Source sent new data: " << source;
  Plasma::DataEngine::DataIterator it(data);

  QString currentText = ui->cbKey->currentText();
  while (it.hasNext()) {
    it.next();

    //avoid duplicates
    QString key = it.key();
    for (int i=0; i < ui->cbKey->count(); i++)
    {
      if (ui->cbKey->itemText(i) == key)
      {
        ui->cbKey->removeItem(i);
        i--;
      }
    }

    kDebug() << key;
    ui->cbKey->addItem(key);
    /*
    parent->setChild(rowCount, 1, new QStandardItem(it.key()));
    if (it.value().canConvert(QVariant::List)) {
      foreach(const QVariant &var, it.value().toList()) {
        QStandardItem *item = new QStandardItem(convertToString(var));
        item->setToolTip(item->text());
        parent->setChild(rowCount, 2, item);
        parent->setChild(rowCount, 3, new QStandardItem(var.typeName()));
        ++rowCount;
      }
    } else {
      QStandardItem *item;
      if (it.value().canConvert<QIcon>()) {
        item = new QStandardItem(it.value().value<QIcon>(), "");
      } else {
        item = new QStandardItem(convertToString(it.value()));
      }

      item->setToolTip(item->text());
      parent->setChild(rowCount, 2, item);
      parent->setChild(rowCount, 3, new QStandardItem(it.value().typeName()));
      ++rowCount;
    }
    */
  }
  if (ui->cbKey->currentText() != currentText)
  {
    int changedIndex = ui->cbKey->findText(currentText);
    if (changedIndex != -1)
      ui->cbKey->setCurrentIndex(changedIndex);
  }
}

#endif

void CreateBoundValueDialog::initToBoundValue(BoundValue *init)
{
    //init
    ui->leName->setText(init->getName());

    //static
    StaticBoundValue *staticBoundValue = dynamic_cast<StaticBoundValue*>(init);
    if (staticBoundValue)
    {
      ui->cbType->setCurrentIndex(0);
      ui->leStaticValue->setText(staticBoundValue->getValue().toString());
    }

    //script
    ScriptBoundValue *scriptBoundValue = dynamic_cast<ScriptBoundValue*>(init);
    if (scriptBoundValue)
    {
      ui->cbType->setCurrentIndex(1);
      ui->teScript->setText(scriptBoundValue->getScript());
    }

    //argument
    ArgumentBoundValue *argumentBoundValue = dynamic_cast<ArgumentBoundValue*>(init);
    if (argumentBoundValue)
    {
      ui->cbType->setCurrentIndex(2);
      ui->sbArgument->setValue(argumentBoundValue->getArgumentNumber());
    }

#ifdef USE_PLASMA
    //plasma
    PlasmaBoundValue *plasmaBoundValue = dynamic_cast<PlasmaBoundValue*>(init);
    if (plasmaBoundValue)
    {
      ui->cbType->setCurrentIndex(3);
      ui->cbDataEngine->setCurrentIndex(
          ui->cbDataEngine->findData(plasmaBoundValue->getDataEngine()));
      initDataEngine();
      ui->cbDataSource->setEditText(plasmaBoundValue->getDataSource());
      ui->cbKey->setEditText(plasmaBoundValue->getKey());
    }
#endif
}

BoundValue* CreateBoundValueDialog::createBoundValue(BoundValue *init)
{
#ifdef USE_PLASMA
  initPlasma();
#endif

  if (init)
    initToBoundValue(init);

  QString name;
  do
  {
    if (!exec()) return 0;
    name = ui->leName->text();
    if (name.isEmpty())
      KMessageBox::information(this, i18n("Each bound value has to have a name."));
  } 
  while (name.isEmpty());

  BoundValue *value = createBoundValueInstance();

  return value;
}


BoundValue* CreateBoundValueDialog::createBoundValueInstance()
{
  BoundValue *value = 0;
  QString name = ui->leName->text();
  switch (ui->cbType->currentIndex())
  {
    case 0:
      //static
      value = new StaticBoundValue(name, ui->leStaticValue->text());
      break;
    case 1:
      //script
      value = new ScriptBoundValue(name, ui->teScript->toPlainText());
      break;
    case 2:
      //script
      value = new ArgumentBoundValue(name, ui->sbArgument->value());
      break;
#ifdef USE_PLASMA
    case 3:
      //plasma
      value = new PlasmaBoundValue(name, ui->cbDataEngine->itemData(ui->cbDataEngine->currentIndex()).toString(),
          ui->cbDataEngine->currentText(), ui->cbDataSource->currentText(), ui->cbKey->currentText());
      break;
#endif
  }
  return value;
}


void CreateBoundValueDialog::test()
{
  BoundValue *value = createBoundValueInstance();
  KMessageBox::information(this, i18n("Expression $%1$ evaluates to:\n%2", ui->leName->text(),
        value->getValue().toString()));
  delete value;
}

CreateBoundValueDialog::~CreateBoundValueDialog()
{
#ifdef USE_PLASMA
  if (!m_currentEngineName.isEmpty())
    m_engineManager->unloadEngine(m_currentEngineName);
#endif
}

