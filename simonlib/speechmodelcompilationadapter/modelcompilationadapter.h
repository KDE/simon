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


#ifndef MODELCOMPILATIONADAPTER_H
#define MODELCOMPILATIONADAPTER_H

#include <QThread>
#include <QProcess>
#include <QString>
#include "simonmodelcompilationadapter_export.h"

/**
 *	@class ModelAdapter
 *	@brief Adapts the generic simon model to a more usable format for the underlying model compiler
 *
 *	@version 0.1
 *	@date 20.12.2009
 *	@author Peter Grasch
 */
class MODELCOMPILATIONADAPTER_EXPORT ModelCompilationAdapter : public QThread {
Q_OBJECT
signals:
	/**
	 * Max progress: 100
	 */
	void status(QString, int progressNow);
	void error(QString);

	void adaptionComplete();
	void adaptionAborted();
public:
	enum AdaptionType {
		AdaptLanguageModel=1,
		AdaptAcousticModel=2
	};

	virtual int maxProgress();
	ModelCompilationAdapter(const QString& userName, QObject *parent=0);

	virtual bool startAdaption(AdaptionType adaptionType, const QString& lexiconPathOut, 
			const QString& grammarPathOut, const QString& simpleVocabPathOut, 
			const QString& promptsPathOut, const QStringList& scenarioPathsIn, 
			const QString& promptsIn);

	virtual bool adaptModel(AdaptionType adaptionType, const QStringList& scenarioPaths, 
			const QString& promptsPathIn, const QString& lexiconPathOut, 
			const QString& grammarPathOut, const QString& simpleVocabPathOut, 
			const QString& promptsPathOut)=0;

	QString getStatus() { return currentStatus; }

	virtual ~ModelCompilationAdapter();

protected:
	bool keepGoing;
	AdaptionType m_adaptionType;
	QString currentStatus;
	QStringList m_scenarioPathsIn;
	QString m_promptsPathIn;
	QString m_userName;

	//output
	QString m_lexiconPathOut, m_grammarPathOut, m_simpleVocabPathOut, m_promptsPathOut;

	void run();
};

#endif
