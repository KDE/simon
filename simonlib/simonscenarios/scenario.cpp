/*
 *   Copyright (C) 2009 Peter Grasch <grasch@simon-listens.org>
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
#include <simonscenariobase/versionnumber.h>
#include "activevocabulary.h"
#include "grammar.h"
#include "trainingtextcollection.h"

#include "actioncollection.h"

#include <simonscenariobase/scenarioobject.h>

#include "version.h"


#include <KStandardDirs>
#include <QDomDocument>
#include <QDateTime>
#include <QDomElement>
#include <QFile>
#include <KDebug>


Scenario::Scenario(const QString& scenarioId, const QString& prefix) : 
	m_prefix(prefix),
	m_inGroup(0),
	m_dirty(true),
	m_scenarioId(scenarioId),
	m_simonMinVersion(NULL),
	m_simonMaxVersion(NULL),
	m_vocabulary(NULL),
	m_texts(NULL),
	m_grammar(NULL),
	m_actionCollection(NULL)
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
 * \author Peter Grasch <grasch@simon-listens.org>
 */
bool Scenario::init(QString path)
{
	QDomDocument *doc=NULL;
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

	delete doc;
	return true;
}

bool Scenario::create(const QString& name, const QString& iconSrc, int version, VersionNumber* simonMinVersion, 
		VersionNumber* simonMaxVersion, const QString& licence, QList<Author*> authors)
{
	m_name = name;
	m_version = version;
	m_iconSrc = iconSrc;
	if (m_simonMinVersion) delete m_simonMinVersion;
	m_simonMinVersion = simonMinVersion;
	if (m_simonMaxVersion) delete m_simonMaxVersion;
	m_simonMaxVersion = simonMaxVersion;
	m_licence = licence;

	foreach (Author *a, m_authors)
		if (!authors.contains(a))
			delete a;
	m_authors.clear();
	foreach (Author *a, authors)
		m_authors << a;

	return true;
}

bool Scenario::update(const QString& name, const QString& iconSrc, int version, VersionNumber* simonMinVersion, 
		VersionNumber* simonMaxVersion, const QString& licence, QList<Author*> authors)
{
	return create(name, iconSrc, version, simonMinVersion, simonMaxVersion, licence, authors);
}

bool Scenario::setupToParse(QString& path, QDomDocument*& doc, bool& deleteDoc)
{
	if (path.isNull()) {
		if (m_prefix.isNull())
			path = KStandardDirs::locate("appdata", "scenarios/"+m_scenarioId);
		else
			path = KStandardDirs::locate("data", m_prefix+"scenarios/"+m_scenarioId);
	}

	if (!doc) {
		doc = new QDomDocument("scenario");
		QFile file(path);
		if (!file.open(QIODevice::ReadOnly)) {
			delete doc;
			return false;
		}
		if (!doc->setContent(&file)) {
			file.close();
			delete doc;
			return false;
		}
		file.close();
		deleteDoc = true;
	}
	return true;
}

bool Scenario::skim(QString path, QDomDocument* doc, bool deleteDoc)
{
	if (!setupToParse(path, doc, deleteDoc)) return false;

	QDomElement docElem = doc->documentElement();

	bool ok=true;

	//  Scenario Infos 
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

	kDebug() << "Loading scenario " << m_name << " version " << m_version;

	//  simon compatibility 
	//************************************************/
	
	QDomElement compatibilityElem = docElem.firstChildElement("simonCompatibility");
	QDomElement simonMinVersionElem = compatibilityElem.firstChildElement();

	if (m_simonMinVersion) delete m_simonMinVersion;
	if (m_simonMaxVersion) delete m_simonMaxVersion;

	m_simonMinVersion = VersionNumber::createVersionNumber(this, simonMinVersionElem);
	m_simonMaxVersion = VersionNumber::createVersionNumber(this, simonMinVersionElem.nextSiblingElement());

	if (!m_simonMinVersion) {
		kDebug() << "Couldn't parse simon requirements!";
		if (deleteDoc) delete doc;
		return false;
	}

	VersionNumber simonCurVersion(this, simon_version);
	if ((!m_simonMinVersion->isValid()) || (simonCurVersion < *m_simonMinVersion) || 
		(m_simonMaxVersion && m_simonMaxVersion->isValid() && (!(simonCurVersion <= *m_simonMaxVersion)))) {
		kDebug() << "Scenario not compatible with this simon version";
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
			kDebug() << "Author information could not be parsed!";
			continue;
		}

		kDebug() << "Scenario written by " << a->name() << a->contact();
		m_authors << a;
		authorElem = authorElem.nextSiblingElement();
	}
	

	//  Licence
	//************************************************/
	m_licence = docElem.firstChildElement("licence").text();
	kDebug() << "Licence: " << m_licence;
		
	if (deleteDoc) delete doc;
	return true;
}


bool Scenario::readLanguageModel(QString path, QDomDocument* doc, bool deleteDoc)
{
	if (!setupToParse(path, doc, deleteDoc)) return false;

	QDomElement docElem = doc->documentElement();

	//  Vocab
	//************************************************/
	kDebug() << "About to create the vocabulary...";
	QDomElement vocabElem = docElem.firstChildElement("vocabulary");
	m_vocabulary = ActiveVocabulary::createVocabulary(this, vocabElem);
	if (!m_vocabulary) {
		kDebug() << "Vocabulary could not be loaded!";
		if (deleteDoc) delete doc;
		return false;
	}
	kDebug() << m_vocabulary->wordCount() << " words loaded";


	//  Grammar
	//************************************************/
	QDomElement grammarElem = docElem.firstChildElement("grammar");
	m_grammar = Grammar::createGrammar(this, grammarElem);
	if (!m_grammar) {
		kDebug() << "Grammar could not be loaded!";
		if (deleteDoc) delete doc;
		return false;
	}
	kDebug() << m_grammar->structureCount() << " structurs loaded";

	if (deleteDoc) delete doc;
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
		kDebug() << "ActionCollection could not be loaded!";
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
		kDebug() << "Trainingtextcollection could not be loaded!";
		return false;
	}
	return true;
}

/**
 * Stores the scenario; The storage location will be determined automatically
 * through the scenarioId of the scenario.
 *
 * Only use the parameter to EXPORT the scenario
 * \author Peter Grasch <grasch@simon-listens.org>
 */
bool Scenario::save(QString path)
{
	if (m_inGroup > 0) {
		m_dirty = true;
		return true;
	}

	m_lastModifiedDate = QDateTime::currentDateTime();
	QString serialized = serialize();

	if (serialized.isNull()) {
		kDebug() << "serialized is null";
		return false;
	}
	
	if (path.isNull()) {
		if (m_prefix.isNull())
			path = KStandardDirs::locate("appdata", "scenarios/"+m_scenarioId);
		else
			path = KStandardDirs::locate("data", m_prefix+"scenarios/"+m_scenarioId);
	}

	QFile file(path);
	if (!file.open(QIODevice::WriteOnly)) {
		kDebug() << "couldn't open file at " << path;
		return false;
	}

	file.write(serialized.toUtf8());

	m_dirty = false;
	emit changed(this);

	return true;
}


QString Scenario::serialize()
{
	QDomDocument doc("scenario");
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


	//  Licence
	//************************************************/
	QDomElement licenceElem = doc.createElement("licence");
	QDomText licenceElemText = doc.createTextNode(m_licence);
	licenceElem.appendChild(licenceElemText);
	rootElem.appendChild(licenceElem);
		

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

	doc.appendChild(rootElem);
	return doc.toString();
}

bool Scenario::addWords(QList<Word*>* w)
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
	

QStringList Scenario::getTerminals(SpeechModel::ModelElements elements)
{
	QStringList terminals;

	if (elements & SpeechModel::ScenarioVocabulary)
		terminals = m_vocabulary->getTerminals();

	if (elements & SpeechModel::ScenarioGrammar) {
		QStringList grammarTerminals = m_grammar->getTerminals();
		foreach (const QString& terminal, grammarTerminals)
			if (!terminals.contains(terminal))
				terminals << terminal;
	}

	return terminals;
}

bool Scenario::renameTerminal(const QString& terminal, const QString& newName, SpeechModel::ModelElements affect)
{
	bool success = true;
	bool scenarioChanged = false;

	if (affect & SpeechModel::ScenarioVocabulary) {
		if (m_vocabulary->renameTerminal(terminal, newName))
			scenarioChanged=true;
		else
			success=false;
	}

	if (affect & SpeechModel::ScenarioGrammar) {
		if (m_grammar->renameTerminal(terminal, newName))
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

QList<Word*> Scenario::findWordsByTerminal(const QString& name)
{
	return m_vocabulary->findWordsByTerminal(name);
}

QString Scenario::fillGrammarSentenceWithExamples(const QString& terminalSentence, bool &ok, const QString& toDemonstrate, 
			const QString& toDemonstrateTerminal)
{

	int terminalOccuranceCount = terminalSentence.count(toDemonstrateTerminal);
	//this occurance of the terminal in the sentence is going to be replaced
	//by the word we should put in a random sentence
	int selectedOccurance = qrand() % terminalOccuranceCount;

	QStringList segmentedTerminals = terminalSentence.split(" ");

	QStringList actualSentence;

	ok = true;

	int currentOccuranceCounter = 0;
	foreach (const QString& terminalNow, segmentedTerminals) {
		if (!toDemonstrate.isNull() && (terminalNow == toDemonstrateTerminal)) {
			if (currentOccuranceCounter == selectedOccurance) {
				actualSentence.append(toDemonstrate);
				continue;
			}
		}

		QString randomWord = m_vocabulary->getRandomWord(terminalNow);
		if (randomWord.isNull()) {
			if (!toDemonstrate.isNull() && (terminalNow == toDemonstrateTerminal)) {
				actualSentence.append(toDemonstrate);
			} else ok = false;
		}  else {
			actualSentence.append(randomWord);
		}
	}

	return actualSentence.join(" ");
}

QStringList Scenario::getExampleSentences(const QString& name, const QString& terminal, int count)
{
	QStringList out;

	int failedCounter = 0;

	for (int i=0; i < count; i++) {
		QString terminalSentence = m_grammar->getExampleSentence(terminal);
		if (terminalSentence.isNull()) {
			//no sentence found
			return out;
		}

		bool ok = true;
		QString resolvedSentence = fillGrammarSentenceWithExamples(terminalSentence, ok, name, terminal);

		if (ok) {
			out << resolvedSentence;
		} else {
			i--;
			if (failedCounter++ == 10)
				break;
		}
	}

	return out;
}

QStringList Scenario::getAllPossibleSentences()
{
	QStringList terminalSentences = m_grammar->getStructures();

	QStringList allSentences;

	foreach (const QString& structure, terminalSentences) {
		allSentences.append(getAllPossibleSentencesOfStructure(structure));
	}
	return allSentences;
}

QStringList Scenario::getAllPossibleSentencesOfStructure(const QString& structure)
{
	//Object, Command
	QStringList structureElements = structure.split(" ");
	QList< QList<Word*> > sentenceMatrix;

	foreach (const QString& element, structureElements)
		sentenceMatrix.append(m_vocabulary->findWordsByTerminal(element));

	//sentences: ( (Window, Test), (Next, Previous) )
	
	return getValidSentences(sentenceMatrix);
}

QStringList Scenario::getValidSentences(QList< QList<Word*> > sentenceMatrix)
{
	QStringList out;
	QList<Word*> poss = sentenceMatrix.takeAt(0);
	foreach (Word *w, poss) {
		if (sentenceMatrix.count() == 0) {
			out.append(w->getWord());
		} else {
			QStringList returned = getValidSentences(sentenceMatrix);
			foreach (const QString& ret, returned)
				out.append(w->getWord()+" "+ret);
		}
	}
	return out;
}


QString Scenario::getRandomWord(const QString& terminal)
{
	return m_vocabulary->getRandomWord(terminal);
}

bool Scenario::containsWord(const QString& word)
{
	return (m_vocabulary->containsWord(word));
}

bool Scenario::containsWord(const QString& word, const QString& terminal, const QString& pronunciation)
{
	return (m_vocabulary->containsWord(word, terminal, pronunciation));
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

bool Scenario::processResult(RecognitionResult recognitionResult)
{
	return m_actionCollection->processResult(recognitionResult);
}

bool Scenario::triggerCommand(const QString& type, const QString& trigger)
{
	return m_actionCollection->triggerCommand(type, trigger);
}

CommandList* Scenario::getCommandList()
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
	id.replace(" ", "_").replace("/","_").remove("?").replace("\\", "_").remove("<").remove(">").remove("|").remove("\"");
	return id+"-"+QDateTime::currentDateTime().toString("dd.MM.dd.yyyy-hh:mm:ss:zzz");
}

void Scenario::setPluginFont(const QFont& font)
{
	kDebug() << "Setting plugin font...";
	m_actionCollection->setPluginFont(font);
}

QList<CommandLauncher*> Scenario::getLauncherList()
{
	return m_actionCollection->getLauncherList();
}

Scenario::~Scenario()
{
	qDeleteAll(m_authors);
	delete m_actionCollection;
	delete m_grammar;
	delete m_texts;
	delete m_vocabulary;
	delete m_simonMinVersion;
	delete m_simonMaxVersion;
}

