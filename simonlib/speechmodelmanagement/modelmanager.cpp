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

#include "modelmanager.h"
#include "grammarmanager.h"
#include "wordlistmanager.h"
#include "trainingmanager.h"
#include "speechmodelmanagementconfiguration.h"
#include <KStandardDirs>
#include <QFile>
#include <speechmodelbase/modelcontainer.h>

ModelContainer* ModelManager::createContainer()
{
	int modelSampleRate=SpeechModelManagementConfiguration::modelSampleRate();
	int modelChannels=SpeechModelManagementConfiguration::modelChannels();
	
	QStringList grammarStructures = GrammarManager::getInstance()->getStructures();
	WordList *simpleVocab = WordListManager::getInstance()->getSimpleVocab();

	QHash<QString,QString> trainingsMap = TrainingManager::getInstance()->getTransferTrainingMap();

	QFile wavConfig(KStandardDirs::locate("appdata", "model/wav_config"));
	QFile treeHed(KStandardDirs::locate("appdata", "model/tree1.hed"));

	if ((!wavConfig.open(QIODevice::ReadOnly)) || (!treeHed.open(QIODevice::ReadOnly)))
		return 0;

	QString modelWavConfig = QString::fromUtf8(wavConfig.readAll());
	QString modelTreeHed = QString::fromUtf8(treeHed.readAll());

	wavConfig.close();
	treeHed.close();

	return new ModelContainer(modelSampleRate, modelChannels, modelWavConfig,
			       grammarStructures, simpleVocab, modelTreeHed, trainingsMap);
}
