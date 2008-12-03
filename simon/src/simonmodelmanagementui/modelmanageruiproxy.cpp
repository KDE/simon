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

#include "modelmanageruiproxy.h"
#include <KMessageBox>
#include <KDebug>
#include <KLocalizedString>
#include <speechmodelmanagement/trainingmanager.h>


ModelManagerUiProxy::ModelManagerUiProxy(QObject *parent) : ModelManager(parent)
{
	connect (this, SIGNAL(modelChanged()), this, SLOT(slotModelChanged()));

	if (ModelManager::instance)
		ModelManager::instance->deleteLater();
	
	ModelManager::instance = this;
}


void ModelManagerUiProxy::slotModelChanged()
{
	emit recompileModel();
}

//TODO: is this deprecated?
void ModelManagerUiProxy::displayCompilationProtocol(const QString& protocol)
{
	KMessageBox::detailedSorry(0, i18n("Protocol:"), protocol);
}

void ModelManagerUiProxy::sampleNotAvailable(const QString& sample)
{
	if (KMessageBox::questionYesNo(0, i18n("The sample \"%1\" could not be found neither on the local computer nor the "
"server.\n\nDo you want to remove it from the Trainings-Database?", sample)) == KMessageBox::Yes)
	{
		//kick some poor samples ass
		startGroup();
		QString sampleBaseName = sample.left(sample.length()-4);
		kWarning() << sampleBaseName;
		bool succ = TrainingManager::getInstance()->removeSample(sampleBaseName);
		if (succ)
			TrainingManager::getInstance()->savePrompts();

		commitGroup(true /*silent*/);

		if (!succ)
			KMessageBox::error(0, i18n("Couldn't remove Sample from the Trainingscorpus"));
		else
			// and _do_ _it_ _again_
			emit recompileModel();
	}
}




void ModelManagerUiProxy::wordUndefined(const QString& word)
{
	if (KMessageBox::questionYesNoCancel(0, i18n("The word \"%1\" is used in your trainings samples but is not phonetically "
"defined. Do you want to add the word now?", word)) != KMessageBox::Yes)
		return;
}

void ModelManagerUiProxy::classUndefined(const QString& undefClass)
{
	KMessageBox::sorry(0, i18n("Your wordlist uses the undefined class %1.\n\nPlease use it in at least one grammar structure.", undefClass));
}

void ModelManagerUiProxy::phonemeUndefined(const QString& phoneme)
{
	KMessageBox::sorry(0, i18n("The Phoneme %1 is unedfined. Please train at least one word that uses it.", phoneme));
}


ModelManagerUiProxy::~ModelManagerUiProxy()
{

}

