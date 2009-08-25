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
#include "scenarioobject.h"
#include "author.h"
#include "versionnumber.h"
#include <KStandardDirs>
#include <QDomDocument>
#include <QDomElement>
#include <QFile>
#include <KDebug>
#include "version.h"


Scenario::Scenario(const QString& scenarioId)
{
	this->scenarioId = scenarioId;
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

	if (!simonMinVersion || !simonMaxVersion) {
		kWarning() << "Couldn't parse simon requirements!";
		return false;
	}

	VersionNumber simonCurVersion(scenarioId, simon_version);
	if ((!simonMinVersion->isValid()) || (simonCurVersion < *simonMinVersion) || 
		(simonMaxVersion->isValid() && (!(simonCurVersion <= *simonMaxVersion)))) {
		kWarning() << "Scenario not compatible with this simon version";
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
			kWarning() << "Author information could not be parsed!";
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
	QDomElement vocabElem = docElem.firstChildElement("vocabulary");


	//  Grammar
	//************************************************/
	QDomElement gramamrElem = docElem.firstChildElement("grammar");


	//  Actions
	//************************************************/
	QDomElement actionsElem = docElem.firstChildElement("actions");


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
	
	return false;
}

Scenario::~Scenario()
{
}

