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

#ifndef SIMON_MODELMANAGERUIPROXY_H_E976001244F74B98889BDF22C72B7258
#define SIMON_MODELMANAGERUIPROXY_H_E976001244F74B98889BDF22C72B7258

#include <speechmodelmanagement/modelmanager.h>

#include "simonmodelmanagementui_export.h"

/**
 *	@class ModelManagementUiProxy
 *	@brief Provides a "proxy" for the underlying modelmanager-concept-class which adds ui elements
 *
 *	@version 0.1
 *	@date 18.11.2008
 *	@author Peter Grasch
*/

class SIMONMODELMANAGEMENTUI_EXPORT ModelManagerUiProxy : public ModelManager
{
	Q_OBJECT

signals:
	void recompileModel();

private slots:
	void slotModelChanged();

private:
	static ModelManagerUiProxy *instance;

public:
	ModelManagerUiProxy(QObject *parent=0);
	~ModelManagerUiProxy();

	void sampleNotAvailable(const QString&);
	void wordUndefined(const QString&);
	void classUndefined(const QString&);
 	void phonemeUndefined(const QString&);
	void displayCompilationProtocol(const QString& protocol);

	bool storeWordList(const QDateTime& changedTime, const QByteArray& simpleVocab,
				const QByteArray& activeVocab, const QByteArray& activeLexicon);
	bool storeGrammar(const QDateTime& changedTime, const QByteArray& grammarStructures);
	bool storeLanguageDescription(const QDateTime& changedTime, const QByteArray& shadowVocab, 
				const QByteArray& treeHed);
	bool storeTraining(const QDateTime& changedTime, qint32 sampleRate, const QByteArray& wavConfig,
				const QByteArray& prompts);
	bool storeActiveModel(const QDateTime& changedTime, qint32 sampleRate, const QByteArray& hmmDefs,
			const QByteArray& tiedList, const QByteArray& dict, const QByteArray& dfa);
	bool storeSample(const QByteArray& sample);
	
};

#endif
