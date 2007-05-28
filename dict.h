//
// C++ Interface: dict
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef DICT_H
#define DICT_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QHash>

/**
	\class Dict
	
	\brief Provides some basic functions and members that make up a Dictionary
	
	Provides functions to convert between IPA and XSP and holds some members like
	the words, pronunciations and terminals.
	This are simple QStringLists because this is still _very_ low-level
	
	The matrix from ipa to x-sampa was built with the matrix from theiling for reference.
	Thanks!
	
	\author Peter Grasch
	\date 6.5.2007
*/
class Dict {
protected:
	QHash<int, QString> phonemes, modifiers;
	QStringList words;
	QStringList pronunciations;
	QStringList terminals;
public:
    Dict();
	QString ipaToXSampa(QString ipa);
    ~Dict();

};

#endif
