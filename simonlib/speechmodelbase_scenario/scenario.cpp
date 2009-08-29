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
#include <simonscenariobase/scenarioobject.h>
#include <simonscenariobase/author.h>
#include <simonscenariobase/versionnumber.h>
#include "vocabulary.h"
#include "grammar.h"
#include "trainingtext.h"

#include <simonactions/action.h>

//Just for testing of the actions
#include <commandpluginbase/commandmanager.h>

#include "version.h"


#include <KStandardDirs>
#include <QDomDocument>
#include <QDomElement>
#include <QFile>
#include <KDebug>


Scenario::Scenario(const QString& scenarioId)
{
	this->scenarioId = scenarioId;

	grammar = NULL;
	vocabulary = NULL;
	simonMinVersion = NULL;
	simonMaxVersion = NULL;
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
	if (path.isNull())
		path = KStandardDirs::locate("appdata", "scenarios/"+scenarioId);

	QDomDocument doc("scenario");
	QFile file(path);
	if (!file.open(QIODevice::ReadOnly))
		return false;
	if (!doc.setContent(&file)) {
		file.close();
		return false;
	}
	file.close();

	bool ok=true;

	QDomElement docElem = doc.documentElement();

	//  Scenario Infos 
	//************************************************/
	
	//name of the scenario
	name = docElem.attribute("name");
	//version of the scenario
	version = docElem.attribute("version").toInt(&ok);
	if (!ok) return false;

	kDebug() << "Loading scenario " << name << " version " << version;

	//  simon compatibility 
	//************************************************/
	
	QDomElement compatibilityElem = docElem.firstChildElement("simonCompatibility");
	QDomElement simonMinVersionElem = compatibilityElem.firstChildElement();

	simonMinVersion = VersionNumber::createVersionNumber(scenarioId, simonMinVersionElem);
	simonMaxVersion = VersionNumber::createVersionNumber(scenarioId, simonMinVersionElem.nextSiblingElement());

	if (!simonMinVersion) {
		kDebug() << "Couldn't parse simon requirements!";
		return false;
	}

	VersionNumber simonCurVersion(scenarioId, simon_version);
	if ((!simonMinVersion->isValid()) || (simonCurVersion < *simonMinVersion) || 
		(simonMaxVersion && simonMaxVersion->isValid() && (!(simonCurVersion <= *simonMaxVersion)))) {
		kDebug() << "Scenario not compatible with this simon version";
		return false;
	}

	//  Authors
	//************************************************/
	
	//clear authors
	qDeleteAll(authors);

	QDomElement authorsElem = docElem.firstChildElement("authors");
	QDomElement authorElem = authorsElem.firstChildElement();
	while (!authorElem.isNull()) {
		Author *a = Author::createAuthor(scenarioId, authorElem);
		if (!a) {
			kDebug() << "Author information could not be parsed!";
			continue;
		}

		kDebug() << "Scenario written by " << a->name() << a->contact();
		authors << a;
		authorElem = authorElem.nextSiblingElement();
	}
	

	//  Licence
	//************************************************/
	licence = docElem.firstChildElement("licence").text();
	kDebug() << "Licence: " << licence;
		

	//  Vocab
	//************************************************/
	kDebug() << "About to create the vocabulay...";
	QDomElement vocabElem = docElem.firstChildElement("vocabulary");
	vocabulary = Vocabulary::createVocabulary(scenarioId, vocabElem);
	if (!vocabulary) {
		kDebug() << "Vocabulary could not be loaded!";
		return false;
	}
	kDebug() << vocabulary->wordCount() << " words loaded";


	//  Grammar
	//************************************************/
	QDomElement grammarElem = docElem.firstChildElement("grammar");
	grammar = Grammar::createGrammar(scenarioId, grammarElem);
	if (!grammar) {
		kDebug() << "Grammar could not be loaded!";
		return false;
	}
	kDebug() << grammar->structureCount() << " structurs loaded";


	//  Actions
	//************************************************/
	QDomElement actionsElem = docElem.firstChildElement("actions");
	QDomElement pluginElem = actionsElem.firstChildElement();
	while (!pluginElem.isNull()) {
		Action *a = Action::createAction(scenarioId, pluginElem);
		if (!a) {
			kDebug() << "Couldn't load action";
		} else {
			actions << a;

			//begin test
			RecognitionResult r("Nummer eingeben", "sdofij", "sdofij", QList<float>() << 1.0f);
			a->manager()->processResult(r);
			//end test
		}

		pluginElem = pluginElem.nextSiblingElement();
	}



	//  Trainingstexts
	//************************************************/
	QDomElement textsElem = docElem.firstChildElement("trainingstexts");
	QDomElement textElem = textsElem.firstChildElement();
	while (!textElem.isNull()) {
		TrainingText *t = TrainingText::createTrainingText(scenarioId, textElem);
		if (!t) {
			kDebug() << "Couldn't load trainingtext";
		} else {
			texts << t;
			kDebug() << "Trainingtext loaded: " << t->getName();
		}
		textElem = textElem.nextSiblingElement();
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
	if (path.isNull())
		path = KStandardDirs::locateLocal("appdata", "scenarios/"+scenarioId);
	
	QDomDocument doc("scenario");
	QFile file(path);
	if (!file.open(QIODevice::WriteOnly))
		return false;

	QDomElement rootElem = doc.createElement("scenario");
	rootElem.setAttribute("name", name);
	rootElem.setAttribute("version", version);

	//  simon compatibility 
	//************************************************/

	QDomElement compatibilityElem = doc.createElement("simonCompatibility");
	QDomElement simonMinVersionElem = doc.createElement("minimumVersion");
	//QDomText simonMinVersionElemText = doc.createTextNode(simonMinVersion->toString());
	simonMinVersionElem.appendChild(simonMinVersion->serialize(&doc));
	compatibilityElem.appendChild(simonMinVersionElem);
	if (simonMaxVersion->isValid()) {
		QDomElement simonMaxVersionElem = doc.createElement("maximumVersion");
		//QDomText simonMaxVersionElemText = doc.createTextNode(simonMaxVersion->toString());
		simonMaxVersionElem.appendChild(simonMaxVersion->serialize(&doc));
		compatibilityElem.appendChild(simonMaxVersionElem);
	}

	rootElem.appendChild(compatibilityElem);

	//  Authors
	//************************************************/
	QDomElement authorsElem = doc.createElement("authors");
	foreach (Author *a, authors) {
		authorsElem.appendChild(a->serialize(&doc));
	}
	rootElem.appendChild(authorsElem);


	//  Licence
	//************************************************/
	QDomElement licenceElem = doc.createElement("licence");
	QDomText licenceElemText = doc.createTextNode(licence);
	licenceElem.appendChild(licenceElemText);
	rootElem.appendChild(licenceElem);
		

	//  Vocab
	//************************************************/
	rootElem.appendChild(vocabulary->serialize(&doc));


	//  Grammar
	//************************************************/
	rootElem.appendChild(grammar->serialize(&doc));


	//  Actions
	//************************************************/
	QDomElement actionsElem = doc.createElement("actions");
	foreach (Action *a, actions) {
		actionsElem.appendChild(a->serialize(&doc));
	}
	rootElem.appendChild(actionsElem);


	//  Trainingstexts
	//************************************************/
	QDomElement textsElem = doc.createElement("texts");
	foreach (TrainingText *t, texts) {
		textsElem.appendChild(t->serialize(&doc));
	}
	rootElem.appendChild(textsElem);

	doc.appendChild(rootElem);
	file.write(doc.toString().toUtf8());

	return true;
}

Scenario::~Scenario()
{
	qDeleteAll(authors);
	qDeleteAll(actions);
	qDeleteAll(texts);
	delete grammar;
	delete vocabulary;
	delete simonMinVersion;
	delete simonMaxVersion;
}

