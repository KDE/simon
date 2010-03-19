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


#ifndef SIMON_MODELCOMPILATIONADAPTERHTK_H_676A176EAB4B40B39F6F8E0F65064C75
#define SIMON_MODELCOMPILATIONADAPTERHTK_H_676A176EAB4B40B39F6F8E0F65064C75

#include <QThread>
#include <QProcess>
#include <QByteArray>
#include "modelcompilationadapter.h"
#include "simonmodelcompilationadapter_export.h"

class Vocabulary;
class Grammar;
/**
 *	@class ModelAdapterHTK
 *	@brief Adapts the generic simon model to a more usable format for the htk model compiler
 *
 *	@version 0.1
 *	@date 20.12.2009
 *	@author Peter Grasch
 */
class MODELCOMPILATIONADAPTER_EXPORT ModelCompilationAdapterHTK : public ModelCompilationAdapter{
Q_OBJECT

private:
	inline QByteArray htkify(const QByteArray& in);
	inline QString htkify(const QString& in);

public:
	ModelCompilationAdapterHTK(const QString& userName, QObject *parent=0);

	bool adaptModel(ModelCompilationAdapter::AdaptionType adaptionType,
			const QStringList& scenarioPaths, const QString& promptsPathIn, 
			const QString& lexiconPathOut, const QString& grammarPathOut, 
			const QString& simpleVocabPathOut, const QString& promptsPathOut);

	bool storeModel(ModelCompilationAdapter::AdaptionType adaptionType,
			const QString& lexiconPathOut, const QString& simpleVocabPathOut, const QString& grammarPathOut, 
			const QString& promptsPathOut, Vocabulary* vocab, Grammar *grammar, const QString& promptsPathIn);

	~ModelCompilationAdapterHTK();

};

#endif
