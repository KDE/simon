/*
 *   Copyright (C) 2009 Peter Grasch <peter.grasch@bedahr.org>
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

#include "scenario.h"
#include "author.h"
#include <versionnumber.h>
#include "activevocabulary.h"
#include "grammar.h"
#include "trainingtextcollection.h"

#include "actioncollection.h"

#include <scenarioobject.h>

#include <version.h>


#include <QDomDocument>
#include <QDateTime>
#include <QDomElement>
#include <QFile>
#include <QFileInfo>
#include <QDebug>
#include <KDateTime>
#include <QDir>
#include <QStandardPaths>

Scenario::Scenario(const QString& scenarioId, const QString& prefix, QObject *parent)
: QObject(parent),
m_prefix(prefix),
m_inGroup(0),
m_dirty(false),
m_scenarioId(scenarioId),
m_simonMinVersion(0),
m_simonMaxVersion(0),
m_vocabulary(0),
m_texts(0),
m_grammar(0),
m_actionCollection(0),
m_compoundCondition(0),
m_parentScenario(0)
{
}


/**
 * Initializes and loads the scenario
 *
 * The scenario will look for itself based on the supplied scenarioId automatically
 *
 * Only use the parameter to IMPORT the scenario!
 *
 * \warning The scenario is NOT valid until this method executes successfully!
 * \author Peter Grasch <peter.grasch@bedahr.org>
 */
bool Scenario::init(QString path)
{
  startGroup(); //ensure we have completely initialized the scenario before saving it
  QDomDocument *doc=0;
  bool deleteDoc;
  if (!setupToParse(path, doc, deleteDoc)) return false;

  if (!skim(path, doc)) return false;

  //  Language model
  //************************************************/
  if (!readLanguageModel(path, doc)) return false;

  //  Actions
  //************************************************/
  if (!readActions(path, doc)) return false;

  //  Trainingstexts
  //************************************************/
  if (!readTrainingsTexts(path, doc)) return false;

  //  CompoundCondition
  //************************************************/
  if (!readCompoundCondition(path, doc)) return false;

  delete doc;
  commitGroup();
  return true;
}


bool Scenario::create(const QString& name, const QString& iconSrc, int version, VersionNumber* simonMinVersion,
VersionNumber* simonMaxVersion, const QString& license, QList<Author*> authors)
{
  m_name = name;
  m_version = version;
  m_iconSrc = iconSrc;
  delete m_simonMinVersion;
  m_simonMinVersion = simonMinVersion;
  delete m_simonMaxVersion;
  m_simonMaxVersion = simonMaxVersion;
  m_license = license;
  m_active = true;

  foreach (Author *a, m_authors)
    if (!authors.contains(a))
    delete a;
  m_authors.clear();
  foreach (Author *a, authors)
    m_authors << a;

  return true;
}

bool Scenario::setupChildScenarios()
{
  m_childScenarios.clear();

  foreach (const QString& id, m_childScenarioIds)
  {
    Scenario* child = ScenarioManager::getInstance()->getScenario(id);

    if (child)
    {
      m_childScenarios << child;
      child->setParentScenario(this);

      qDebug() << child->id() + " is set as child of " + this->id();
    }
    else
    {
      qDebug() << "Error: Child id '" + id + "'' is unavailable";
    }
  }

  return true;
}

QList<Scenario*> Scenario::childScenarios() const
{
  return m_childScenarios;
}

QStringList Scenario::childScenarioIds() const
{
  return m_childScenarioIds;
}

Scenario* Scenario::parentScenario()
{
  return m_parentScenario;
}

void Scenario::setParentScenario(Scenario *parent)
{
  m_parentScenario = parent;
  updateActivation();
}

void Scenario::setChildScenarioIds(QStringList ids)
{
  m_childScenarioIds = ids;
}

bool Scenario::updateChildScenarioIds(const QString& id, const QStringList& ids)
{
  QString path = Scenario::pathFromId(id, QString(), false);
  qDebug() << "Updating child ids of " << path << " to " << ids;
  QDomDocument doc("scenario");
  QFile file(path);
  if (!file.open(QIODevice::ReadOnly) || (!doc.setContent(&file))) {
    return false;
  }
  QDomElement docElem = doc.documentElement();
  docElem.setAttribute("lastModified", utcTime().toString(Qt::ISODate));
  QDomElement childrenElem = docElem.firstChildElement("childscenarioids");
  if (childrenElem.isNull()) {
    qDebug() << "No child scenario id list element!";
    childrenElem = doc.createElement("childscenarioids");
    docElem.appendChild(childrenElem);
  } else {
    QDomElement idElem;
    while (!(idElem = childrenElem.firstChildElement("scenarioid")).isNull())
      childrenElem.removeChild(idElem); // remove all children
  }

  foreach(const QString& id, ids)
  {
    QDomElement idElem = doc.createElement("scenarioid");
    idElem.appendChild(doc.createTextNode(id));
    childrenElem.appendChild(idElem);
  }

  file.close();
  file.setFileName(Scenario::pathFromId(id));
  if (!file.open(QIODevice::WriteOnly|QIODevice::Truncate))
    return false;

  QString out = Scenario::pathFromId(id, QString(), false);
  QByteArray data = doc.toString().toUtf8();
  file.write(data);
  file.close();
  return true;
}

bool Scenario::update(const QString& name, const QString& iconSrc, int version, VersionNumber* simonMinVersion,
VersionNumber* simonMaxVersion, const QString& license, QList<Author*> authors)
{
  return create(name, iconSrc, version, simonMinVersion, simonMaxVersion, license, authors);
}

bool Scenario::setupPathToParse(QString& path)
{
  if (path.isNull()) {
    if (m_prefix.isNull())
        path = QStandardPaths::writableLocation(QStandardPaths::DataLocation) + "/scenarios/";
    else
        path = QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + QLatin1Char('/') + m_prefix + "/scenarios/";
    QDir dir(path);
    path = path + m_scenarioId;
    return dir.mkpath(dir.path());
  }
  return true;
}

bool Scenario::setupToParse(QString& path, QDomDocument*& doc, bool& deleteDoc)
{
  if (!setupPathToParse(path)) {
      qWarning() << "Failed to make directory" << path;
      return false;
  }

  if (!doc) {
    doc = new QDomDocument("scenario");
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
      qWarning() << "Could not open file" << path;
      delete doc;
      return false;
    }
    if (!doc->setContent(&file)) {
      qDebug() << "Could not parse scenario document";
      file.close();
      delete doc;
      return false;
    }
    file.close();
    deleteDoc = true;
  }
  return true;
}

QDateTime Scenario::skimDate(QString path)
{
  QFile f(path);
  if (!f.open(QIODevice::ReadOnly)) return QDateTime();

  QByteArray line;
  do
  {
    line = f.readLine();
  }
  while (!f.atEnd() && (!line.contains("lastModified")));
  
  int index = line.indexOf("lastModified");
  if (index == -1) return QDateTime();

  int startIndex = line.indexOf('"', index+13);
  int endIndex = line.indexOf('"', startIndex+1);

  return QDateTime::fromString(line.mid(startIndex+1, endIndex - startIndex), Qt::ISODate);
}

bool Scenario::skim(QString path, QDomDocument* doc, bool deleteDoc)
{
  if (!setupToParse(path, doc, deleteDoc)) return false;

  QDomElement docElem = doc->documentElement();

  bool ok=true;

  //  Scenario Information
  //************************************************/

  //name of the scenario
  m_name = docElem.attribute("name");

  if (m_scenarioId.isEmpty()) m_scenarioId = createId(m_name);

  //version of the scenario
  m_version = docElem.attribute("version").toInt(&ok);
  m_iconSrc = docElem.attribute("icon");
  m_lastModifiedDate = QDateTime::fromString(docElem.attribute("lastModified"), Qt::ISODate);
  if (!ok) {
    if (deleteDoc) delete doc;
    return false;
  }

  //qDebug() << "Loading scenario " << m_name << " version " << m_version;

  //  simon compatibility
  //************************************************/

  QDomElement compatibilityElem = docElem.firstChildElement("simonCompatibility");
  QDomElement simonMinVersionElem = compatibilityElem.firstChildElement();

  delete m_simonMinVersion;
  delete m_simonMaxVersion;

  m_simonMinVersion = VersionNumber::createVersionNumber(this, simonMinVersionElem);
  m_simonMaxVersion = VersionNumber::createVersionNumber(this, simonMinVersionElem.nextSiblingElement());

  if (!m_simonMinVersion) {
    qDebug() << "Could not parse Simon requirements!";
    if (deleteDoc) delete doc;
    return false;
  }

  VersionNumber simonCurVersion(this, simon_version);
  if ((!m_simonMinVersion->isValid()) || (simonCurVersion < *m_simonMinVersion) ||
  (m_simonMaxVersion && m_simonMaxVersion->isValid() && (!(simonCurVersion <= *m_simonMaxVersion)))) {
    qDebug() << "Scenario not compatible with this Simon version";
    if (deleteDoc) delete doc;
    return false;
  }

  //  Authors
  //************************************************/

  //clear authors
  qDeleteAll(m_authors);
  m_authors.clear();

  QDomElement authorsElem = docElem.firstChildElement("authors");
  QDomElement authorElem = authorsElem.firstChildElement();
  while (!authorElem.isNull()) {
    Author *a = Author::createAuthor(this, authorElem);
    if (!a) {
      qDebug() << "Author information could not be parsed!";
      continue;
    }

    m_authors << a;
    authorElem = authorElem.nextSiblingElement();
  }

  //  License
  //************************************************/
  m_license = docElem.firstChildElement("license").text();
  if (m_license.isNull())
	  //load previous version
	 m_license = docElem.firstChildElement("licence").text(); // krazy:exclude=spelling

  //  ChildScenarios
  //************************************************/
  if (!readChildScenarioIds(path, doc)) return false;

  if (deleteDoc) delete doc;
  return true;
}


bool Scenario::readLanguageModel(QString path, QDomDocument* doc, bool deleteDoc)
{
  if (!setupToParse(path, doc, deleteDoc)) return false;

  QDomElement docElem = doc->documentElement();

  //  Vocab
  //************************************************/
  QDomElement vocabElem = docElem.firstChildElement("vocabulary");
  m_vocabulary = ActiveVocabulary::createVocabulary(this, vocabElem);
  if (!m_vocabulary) {
    if (deleteDoc) delete doc;
    return false;
  }
  //qDebug() << m_vocabulary->wordCount() << " words loaded";

  //  Grammar
  //************************************************/
  QDomElement grammarElem = docElem.firstChildElement("grammar");
  m_grammar = Grammar::createGrammar(this, grammarElem);
  if (!m_grammar) {
    qDebug() << "Grammar could not be loaded!";
    if (deleteDoc) delete doc;
    return false;
  }
  //qDebug() << m_grammar->structureCount() << " structurs loaded";

  if (deleteDoc)
    delete doc;
  
  return true;
}


bool Scenario::readActions(QString path, QDomDocument* doc, bool deleteDoc)
{
  if (!setupToParse(path, doc, deleteDoc)) return false;

  QDomElement docElem = doc->documentElement();

  QDomElement actionsElement = docElem.firstChildElement("actions");
  m_actionCollection = ActionCollection::createActionCollection(this, actionsElement);

  if (deleteDoc) delete doc;

  if (!m_actionCollection) {
    qDebug() << "ActionCollection could not be loaded!";
    return false;
  }
  return true;
}


bool Scenario::readTrainingsTexts(QString path, QDomDocument* doc, bool deleteDoc)
{
  if (!setupToParse(path, doc, deleteDoc)) return false;

  QDomElement docElem = doc->documentElement();

  QDomElement textsElem = docElem.firstChildElement("trainingtexts");
  m_texts = TrainingTextCollection::createTrainingTextCollection(this, textsElem);

  if (deleteDoc) delete doc;

  if (!m_texts) {
    qDebug() << "Trainingtextcollection could not be loaded!";
    return false;
  }
  return true;
}

bool Scenario::readCompoundCondition(QString path, QDomDocument* doc, bool deleteDoc)
{
  if (!setupToParse(path, doc, deleteDoc)) return false;

  QDomElement docElem = doc->documentElement();

  QDomElement textsElem = docElem.firstChildElement("compoundcondition");

  if (textsElem.isNull())
  {
    textsElem = CompoundCondition::createEmpty(doc);
  }

  m_active = true;

  m_compoundCondition = CompoundCondition::createInstance(textsElem);

  if (deleteDoc) delete doc;

  if (!m_compoundCondition) {
    qDebug() << "CompoundCondition could not be deSerialized!";
    return false;
  }

  m_active = m_compoundCondition->isSatisfied();

  connect(m_compoundCondition, SIGNAL(conditionChanged(bool)),
          this, SLOT(updateActivation()));
  connect(m_compoundCondition, SIGNAL(modified()),
          this, SLOT(save()));

  qDebug() << "Compound condition has been deSerialized!";

  return true;
}

bool Scenario::readChildScenarioIds(QString path, QDomDocument* doc, bool deleteDoc)
{
  if (!setupToParse(path, doc, deleteDoc)) return false;

  QDomElement docElem = doc->documentElement();

  QDomElement childrenElem = docElem.firstChildElement("childscenarioids");

  if (childrenElem.isNull())
  {
    qDebug() << "No child scenario id list element!";
    return true;
  }

  m_childScenarioIds.clear();
  QDomElement idElem = childrenElem.firstChildElement("scenarioid");
  while (!idElem.isNull())
  {
    m_childScenarioIds.push_back(idElem.text());

    qDebug() << "Child scenario id added: " + idElem.text();

    idElem = idElem.nextSiblingElement("scenarioid");
  }

  qDebug() << "Child scenario id list has been deSerialized!";

  return true;
}

QStringList Scenario::explode(const QString& inFile, bool isIntegrated)
{
  QFile f(inFile);

  if (!f.open(QIODevice::ReadOnly))
    return QStringList();

  QDomDocument d("scenario");
  d.setContent(f.readAll());
  f.close();

  QDomElement scenarioElem = d.firstChildElement("scenario");
  if (scenarioElem.isNull()) {
    qDebug() << "This is not a scenario";
    return QStringList();
  }

  QStringList allScenarios;

  QDomElement childrenElem = scenarioElem.firstChildElement("childscenarioids");
  QDomElement childElem = childrenElem.firstChildElement("scenario");
  QStringList childrenSrc;
  while (!childElem.isNull())
  {
    //expand children
    QString name = childElem.attribute("name");
    QString id = Scenario::createId(name);
    QString path = pathFromId(id);
    qDebug() << "this id: " << id << " full path: " << path;
    QString suffix;
    while (QFile::exists(path + suffix))
      suffix += "_";

    QFile f(path + suffix);
    if (!f.open(QIODevice::WriteOnly)) {
      qWarning() << "Failed to write temporary scenario";
      continue;
    }
    QDomDocument tempDoc;
    tempDoc.appendChild(tempDoc.importNode(childElem, true));
    qDebug() << "Writing: " << tempDoc.toString().toUtf8();
    f.write(tempDoc.toString().toUtf8());

    childrenSrc << id + suffix;
    childElem = childElem.nextSiblingElement("scenario");
  }
  //strip children
  while (!(childElem = childrenElem.firstChildElement("scenario")).isNull()) {
    childrenElem.removeChild(childElem);
  }
  foreach (QString child, childrenSrc) {//explode recursively
    allScenarios << Scenario::explode(pathFromId(child), true);
    QDomElement idElem = d.createElement("scenarioid");
    idElem.appendChild(d.createTextNode(child));
    childrenElem.appendChild(idElem); // add link
  }

  QString outerId;
  QString outerPath;
  if (!isIntegrated) {
    outerId = Scenario::createId(scenarioElem.attribute("name"));
    outerPath = pathFromId(outerId);
  } else {
    outerId = idFromPath(inFile);
    outerPath = inFile;
  }

  QFile::remove(inFile);
  qDebug() << "Output path: " << outerPath;
  QFile fInSrc(outerPath);
  if (!fInSrc.open(QIODevice::WriteOnly)) {
    qWarning() << "couldn't write outer scenario";
  } else {
    fInSrc.write(d.toString().toUtf8());
    fInSrc.close();
    allScenarios.insert(0, outerId);
  }
  return allScenarios;
}

//QT5TODO: 
QString Scenario::pathFromId(const QString& id, const QString& prefix, bool local)
{
  if (local) {
    if (prefix.isNull())
      return QStandardPaths::writableLocation(QStandardPaths::DataLocation) + QLatin1Char('/') + "scenarios/"+id;
    else
      return QStandardPaths::writableLocation(QStandardPaths::GenericDataLocation) + QLatin1Char('/') + prefix+"scenarios/"+id;
  } else {
    if (prefix.isNull())
      return QStandardPaths::StandardLocation(QStandardPaths::DataLocation) + "/scenarios/"+id;
    else
      return QStandardPaths::StandardLocation(QStandardPaths::GenericDataLocation) + prefix+"/scenarios/"+id;
  }
}

QString Scenario::idFromPath(const QString& path)
{
  return QFileInfo(path).fileName();
}

QString Scenario::preferredPath() const
{
  return pathFromId(m_scenarioId, m_prefix);
}

/**
 * Stores the scenario; The storage location will be determined automatically
 * through the scenarioId of the scenario.
 *
 * Only use the parameter @a path to EXPORT the scenario to the given path
 *
 * When @a full is true, all sub-secnarios will be inlined in the output XML
 *
 * When @a touchModifiedTime is true, the modification time will be set to the current
 * UTC time, otherwise it will stay unchanged.
 *
 * \author Peter Grasch <peter.grasch@bedahr.org>
 */
bool Scenario::save(QString path, bool full, bool touchModifiedTime)
{
  if (m_inGroup > 0) {
    m_dirty = true;
    return true;
  }

  if (touchModifiedTime)
    m_lastModifiedDate = utcTime();

  QString serialized = serialize(full);

  if (serialized.isNull())
    return false;

  if (path.isNull())
    path = preferredPath();

  QFile file(path);
  if (!file.open(QIODevice::WriteOnly)) {
    qDebug() << "could not open file at " << path;
    return false;
  }

  file.write(serialized.toUtf8());
  file.close();

  m_dirty = false;
  emit changed(this);

  qDebug() << "Wrote scenario file" << file.fileName();
  return true;
}

QDateTime Scenario::utcTime()
{
  return KDateTime::currentUtcDateTime().dateTime();
}

QDomElement Scenario::serialize(QDomDocument doc, bool full)
{
  QDomElement rootElem = doc.createElement("scenario");
  rootElem.setAttribute("name", m_name);
  rootElem.setAttribute("version", m_version);
  rootElem.setAttribute("icon", m_iconSrc);
  rootElem.setAttribute("lastModified", m_lastModifiedDate.toString(Qt::ISODate));

  //  simon compatibility
  //************************************************/

  QDomElement compatibilityElem = doc.createElement("simonCompatibility");
  QDomElement simonMinVersionElem = doc.createElement("minimumVersion");
  simonMinVersionElem.appendChild(m_simonMinVersion->serialize(&doc));
  compatibilityElem.appendChild(simonMinVersionElem);
  QDomElement simonMaxVersionElem = doc.createElement("maximumVersion");
  if (m_simonMaxVersion && m_simonMaxVersion->isValid()) {
    simonMaxVersionElem.appendChild(m_simonMaxVersion->serialize(&doc));
  }
  compatibilityElem.appendChild(simonMaxVersionElem);

  rootElem.appendChild(compatibilityElem);

  //  Authors
  //************************************************/
  QDomElement authorsElem = doc.createElement("authors");
  foreach (Author *a, m_authors) {
    authorsElem.appendChild(a->serialize(&doc));
  }
  rootElem.appendChild(authorsElem);

  //  License
  //************************************************/
  QDomElement licenseElem = doc.createElement("license");
  QDomText licenseElemText = doc.createTextNode(m_license);
  licenseElem.appendChild(licenseElemText);
  rootElem.appendChild(licenseElem);

  //  Vocab
  //************************************************/
  if (!m_vocabulary)
    rootElem.appendChild(Vocabulary::createEmpty(&doc));
  else
    rootElem.appendChild(m_vocabulary->serialize(&doc));

  //  Grammar
  //************************************************/
  if (!m_grammar)
    rootElem.appendChild(Grammar::createEmpty(&doc));
  else
    rootElem.appendChild(m_grammar->serialize(&doc));

  //  Actions
  //************************************************/
  if (!m_actionCollection)
    rootElem.appendChild(ActionCollection::createEmpty(&doc));
  else
    rootElem.appendChild(m_actionCollection->serialize(&doc));

  //  Trainingstexts
  //************************************************/
  if (!m_texts)
    rootElem.appendChild(TrainingTextCollection::createEmpty(&doc));
  else
    rootElem.appendChild(m_texts->serialize(&doc));

  //  CompoundCondition
  //************************************************/
  if (!m_compoundCondition)
  {
    qDebug() << "No compound condition to serialize!";
    rootElem.appendChild(CompoundCondition::createEmpty(&doc));
  }
  else
  {
    qDebug() << "Serializing compound condition!";
    rootElem.appendChild(m_compoundCondition->serialize(&doc));
  }

  //  Child Scenario Ids
  //************************************************/
  QDomElement childrenElem = doc.createElement("childscenarioids");
  QDomElement idElem;
  QList<Scenario*> scenariosToDelete;
  foreach(const QString& id, m_childScenarioIds)
  {
    if (!full) {
      idElem = doc.createElement("scenarioid");
      idElem.appendChild(doc.createTextNode(id));

      childrenElem.appendChild(idElem);
    } else {
      // serialize child scenarios and include them here
      Scenario *s = ScenarioManager::getInstance()->getScenario(id);
      if (!s) {
        //not loaded, load it temporarily
        s = new Scenario(id);
        scenariosToDelete.push_back(s);
        if (!s->init()) continue;
      }
      childrenElem.appendChild(s->serialize(doc, full));
    }
  }
  foreach (Scenario *s, scenariosToDelete)
    delete s;
  rootElem.appendChild(childrenElem);
  return rootElem;
}


QString Scenario::serialize(bool full)
{
  QDomDocument doc("scenario");
  doc.appendChild(serialize(doc, full));
  return doc.toString();
}


bool Scenario::addWords(QList<Word*> w)
{
  return (m_vocabulary->addWords(w));
}


bool Scenario::addWord(Word* w)
{
  return (m_vocabulary->addWord(w));
}


bool Scenario::removeWord(Word* w)
{
  return (m_vocabulary->removeWord(w));
}


bool Scenario::addStructures(const QStringList& newStructures)
{
  return m_grammar->addStructures(newStructures);
}


QStringList Scenario::getCategories(SpeechModel::ModelElements elements)
{
  QStringList categories;

  if (elements & SpeechModel::ScenarioVocabulary)
    categories = m_vocabulary->getCategories();

  if (elements & SpeechModel::ScenarioGrammar) {
    QStringList grammarCategories = m_grammar->getCategories();
    foreach (const QString& category, grammarCategories)
      if (!categories.contains(category))
      categories << category;
  }

  return categories;
}


bool Scenario::renameCategory(const QString& category, const QString& newName, SpeechModel::ModelElements affect)
{
  bool success = true;
  bool scenarioChanged = false;

  if (affect & SpeechModel::ScenarioVocabulary) {
    if (m_vocabulary->renameCategory(category, newName))
      scenarioChanged=true;
    else
      success=false;
  }

  if (affect & SpeechModel::ScenarioGrammar) {
    if (m_grammar->renameCategory(category, newName))
      scenarioChanged=true;
    else
      success=false;
  }

  if (scenarioChanged) emit changed(this);

  return success;
}


QList<Word*> Scenario::findWords(const QString& name, Vocabulary::MatchType type)
{
  return m_vocabulary->findWords(name, type);
}


QList<Word*> Scenario::findWordsByCategory(const QString& name)
{
  return m_vocabulary->findWordsByCategory(name);
}


QString Scenario::fillGrammarSentenceWithExamples(const QString& categorySentence, bool &ok, const QString& toDemonstrate,
const QString& toDemonstrateCategory)
{

  int categoryOccuranceCount = categorySentence.count(toDemonstrateCategory);
  //this occurrence of the category in the sentence is going to be replaced
  //by the word we should put in a random sentence
  int selectedOccurance = qrand() % categoryOccuranceCount;

  QStringList segmentedCategories = categorySentence.split(' ');

  QStringList actualSentence;

  ok = true;

  int currentOccuranceCounter = 0;
  foreach (const QString& categoryNow, segmentedCategories) {
    if (!toDemonstrate.isNull() && (categoryNow == toDemonstrateCategory)) {
      if (currentOccuranceCounter++ == selectedOccurance) {
        actualSentence.append(toDemonstrate);
        continue;
      }
    }

    QString randomWord = m_vocabulary->getRandomWord(categoryNow);
    if (randomWord.isNull()) {
      if (!toDemonstrate.isNull() && (categoryNow == toDemonstrateCategory)) {
        actualSentence.append(toDemonstrate);
      } else ok = false;
    }
    else {
      actualSentence.append(randomWord);
    }
  }

  return actualSentence.join(" ");
}


QStringList Scenario::getExampleSentences(const QString& name, const QString& category, int count)
{
  QStringList out;

  int failedCounter = 0;

  for (int i=0; i < count; i++) {
    QString categorySentence = m_grammar->getExampleSentence(category);
    if (categorySentence.isNull()) {
      //no sentence found
      return out;
    }

    bool ok = true;
    QString resolvedSentence = fillGrammarSentenceWithExamples(categorySentence, ok, name, category);

    if (ok) {
      out << resolvedSentence;
    }
    else {
      i--;
      if (failedCounter++ == 10)
        break;
    }
  }

  return out;
}


QStringList Scenario::getAllPossibleSentences()
{
  QStringList categorySentences = m_grammar->getStructures();

  QStringList allSentences;

  foreach (const QString& structure, categorySentences) {
    allSentences.append(getAllPossibleSentencesOfStructure(structure));
  }
  return allSentences;
}


QStringList Scenario::getExampleSentencesOfStructure(const QString& structure)
{
  int alreadyFoundExamples = 0;
  return getAllPossibleSentencesOfStructure(structure, &alreadyFoundExamples);
}


QStringList Scenario::getAllPossibleSentencesOfStructure(const QString& structure, int* alreadyFoundExamples)
{
  if (alreadyFoundExamples && (*alreadyFoundExamples > 35))
    return QStringList();

  //Object, Command
  QStringList structureElements = structure.split(' ');
  QList< QList<Word*> > sentenceMatrix;

  foreach (const QString& element, structureElements)
    sentenceMatrix.append(m_vocabulary->findWordsByCategory(element));

  //sentences: ( (Window, Test), (Next, Previous) )

  return getValidSentences(sentenceMatrix, alreadyFoundExamples);
}


QStringList Scenario::getValidSentences(QList< QList<Word*> > sentenceMatrix, int* alreadyFoundExamples)
{
  if (alreadyFoundExamples && (*alreadyFoundExamples > 35))
    return QStringList();

  QStringList out;
  QList<Word*> poss = sentenceMatrix.takeAt(0);
  foreach (Word *w, poss) {
    if (sentenceMatrix.isEmpty()) {
      out.append(w->getWord());
      if (alreadyFoundExamples) {
        (*alreadyFoundExamples)++;
        if (*alreadyFoundExamples > 35)
          return out;
      }
    }
    else {
      QStringList returned = getValidSentences(sentenceMatrix, alreadyFoundExamples);
      foreach (const QString& ret, returned)
        out.append(w->getWord()+' '+ret);
    }
  }
  return out;
}


QString Scenario::getRandomWord(const QString& category)
{
  return m_vocabulary->getRandomWord(category);
}


bool Scenario::containsWord(const QString& word)
{
  return (m_vocabulary->containsWord(word));
}


bool Scenario::containsWord(const QString& word, const QString& category, const QString& pronunciation)
{
  return (m_vocabulary->containsWord(word, category, pronunciation));
}


bool Scenario::removeText(TrainingText* text)
{
  return m_texts->removeText(text);
}


bool Scenario::addTrainingText(TrainingText* text)
{
  return m_texts->addTrainingText(text);
}


/*bool Scenario::addCommand(Command *command)
{
  return m_actionCollection->addCommand(command);
}*/

bool Scenario::removeCommand(Command *command)
{
  return m_actionCollection->removeCommand(command);
}

bool Scenario::setAutorun(bool active, const QString& type, const QString& trigger)
{
  return m_actionCollection->setAutorun(active, type, trigger);
}

bool Scenario::autorunActive()
{
  return m_actionCollection->autorunActive();
}

QString Scenario::autorunType()
{
  return m_actionCollection->autorunType();
}

QString Scenario::autorunTrigger()
{
  return m_actionCollection->autorunTrigger();
}

bool Scenario::processResult(RecognitionResult recognitionResult)
{
  return m_actionCollection->processResult(recognitionResult);
}


bool Scenario::triggerCommand(const QString& type, const QString& trigger, bool silent)
{
  return m_actionCollection->triggerCommand(type, trigger, silent);
}


CommandList Scenario::getCommandList()
{
  return m_actionCollection->getCommandList();
}


void Scenario::startGroup()
{
  m_inGroup++;
}


bool Scenario::commitGroup()
{
  m_inGroup--;
  bool success = true;
  if ((m_inGroup == 0) && (m_dirty))
    success = save();

  return success;
}


QString Scenario::createId(const QString& name)
{
  QString id = name;
  id.replace(QRegExp("[\\/:*?\"<>| ]"), "_");
  return id+'-'+utcTime().toString("dd.MM.dd.yyyy-hh-mm-ss-zzz");
}


void Scenario::setPluginFont(const QFont& font)
{
  m_actionCollection->setPluginFont(font);
}


QHash<CommandListElements::Element, VoiceInterfaceCommand*> Scenario::getListInterfaceCommands()
{
  return m_actionCollection->getListInterfaceCommands();
}


void Scenario::setListInterfaceCommands(QHash<CommandListElements::Element, VoiceInterfaceCommand*> commands)
{
  m_actionCollection->setListInterfaceCommands(commands);
}

void Scenario::updateActivation()
{
  if (!m_parentScenario)
  {
    if (m_compoundCondition->isSatisfied())
    {
      qDebug() << "Scenario '" + m_name + "' and its applicable children should activate due to context!";
      m_active = true;
    }
    else
    {
      qDebug() << "Scenario '" + m_name + "' and its children should deactivate due to context!";
      m_active = false;
    }
  }
  else
  {
    if (m_compoundCondition->isSatisfied() && m_parentScenario->isActive())
    {
      qDebug() << "Scenario '" + m_name + "' and its applicable children should activate due to context!";
      m_active = true;
    }
    else
    {
      qDebug() << "Scenario '" + m_name + "' and its children should deactivate due to context!";
      m_active = false;
    }
  }

  foreach(Scenario *s, m_childScenarios)
  {
    s->updateActivation();
  }

  emit activationChanged();
}


Scenario::~Scenario()
{
  //plugins might do something with the scenario when deleted so kill them first
  delete m_actionCollection;
  qDeleteAll(m_authors);
  if (m_grammar)
    m_grammar->deleteLater();
  if (m_texts)
    m_texts->deleteLater();
  if (m_vocabulary)
    m_vocabulary->deleteLater();
  if (m_compoundCondition)
    m_compoundCondition->deleteLater();
  delete m_simonMinVersion;
  delete m_simonMaxVersion;
}
