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


#ifndef WORD_H
#define WORD_H
#include <QString>
#include <QList>
#include <KLocalizedString>
#include "simonmodelmanagement_export.h"

/**
 *	@class Word
 *	@brief Represents the Information of one word
 *
 *	This class contains all information to ressemble one word in the Dictionary
 *	It can handle multiple pronunciations of the word
 *
 *	@version 0.1
 *	@date 07.01.2006
 *	@author Peter Grasch
 */

class Word;

typedef QList<Word> WordList; //!< QList from Word

class MODELMANAGEMENT_EXPORT Word {

private:
	QString word; //!< Saves the represented word
	QString pronunciation; //!< Saves all valid pronunciations of the word in Sam-Pa
	QString terminal; //!< Category of the word
	QString lexiconWord;
	
public:
	
	/**
	* @brief Constructor
	*
	* This is the constructor of the Word class.
	*
	* @author Peter Grasch
	* @param QString word
	* Contains the written word
	* @param QString pronunciation
	* The sampa notation of the pronunciation
	* @param QString terminal
	* Terminal the word belongs to
	* 
	*/
	Word(QString word_, QString pronunciation_, QString terminal_)
		: word(word_),
		pronunciation(pronunciation_),
		terminal(terminal_),
		lexiconWord(word_.toUpper())
	{
	}

	/**
	 * @brief Getter-Method: Word
	 *
	 * Returns the Word itself
	 *
	 * @author Peter Grasch
	 * @return QString Word
	 * Returns the word
	 * 
	 */
	QString getWord() const
	{
		return this->word;
	}

	/**
	 * \brief Setter-Method: word
	 * \author Peter Grasch
	 * @param word The (new) word of the word
	 */
	void setWord(QString word)
	{
		this->word = word;
		this->lexiconWord = word.toUpper();
	}


	
	/**
	 * @brief Getter-Method: Lexiconword
	 *
	 * Returns the Word used for the lexicon itself
	 *
	 * @author Peter Grasch
	 * @return QString Word
	 * Returns the word
	 * 
	 */
	QString getLexiconWord() const
	{
		return this->lexiconWord;
	}

	/**
	 * @brief Getter-Method: Pronunciation
	 *
	 * @author Peter Grasch
	 * @return QString
	 * Contains the pronunciations of the word
	 * 
	 */
	QString getPronunciation() const
	{
		return this->pronunciation;
	}

	/**
	 * \brief Setter-Method: pronunciation
	 * \author Peter Grasch
	 * @param pronunciation The (new) pronunciation of the word
	 */
	void setPronunciation(QString pronunciation)
	{
		this->pronunciation = pronunciation;
	}

	/**
	 * @brief Getter-Method: terminal
	 *
	 * @author Peter Grasch
	 * @return QString
	 * Returns the terminal (category) of the word
	 * 
	 */
	QString getTerminal() const
	{
		return this->terminal;
	}


	/**
	 * \brief Setter-Method: terminals
	 * \author Peter Grasch
	 * @param terminal The (new) terminal of the word
	 */
	void setTerminal(QString terminal)
	{
		this->terminal = terminal;
	}
	
	/**
	 * \brief Determines if a word is "smaller" than the other
	 * used for sorting
	 * \author Peter Grasch
	 * \param const Word& w2
	 * The second word
	 * \return bool
	 * smaller?
	 */
	bool operator<(const Word& w2) const
	{
		if (getLexiconWord() < w2.getLexiconWord())
			return true;
		else return ((getLexiconWord() == w2.getLexiconWord()) && ((getPronunciation() < w2.getPronunciation()) || 
							((getPronunciation() == w2.getPronunciation()) && (getTerminal() < w2.getTerminal()))));
	}
	
	
	bool operator!=(const Word& w2) const
	{
		if ((getWord() != w2.getWord()) || (getTerminal() != w2.getTerminal()) || (getPronunciation() != w2.getPronunciation()))
			return true;
		
		return false;
	}
	
	
	bool operator==(const Word& w2) const
	{
		return ((getWord() == w2.getWord()) && (getTerminal() == w2.getTerminal()) && (getPronunciation() == w2.getPronunciation()));
	}

	
	/**
	 * @brief Getter-Method: probability
	 *
	 * @author Peter Grasch
	 * @return int
	 * Returns the probability (how well it has been trained) of the word
	 * 
	 */
	int getPropability();
	

	/**
	* @brief Destructor
	*
	* Default - empty - destructor
	*
	* @author Peter Grasch
	* 
	*/
    ~Word() {}

};

bool isWordLessThan(Word *w1, Word *w2);

#endif
