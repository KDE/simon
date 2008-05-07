//
// C++ Interface: word
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef WORD_H
#define WORD_H
#include <QString>
#include <QList>

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

class Word{

private:
	QString word; //!< Saves the represented word
	QString pronunciation; //!< Saves all valid pronunciations of the word in Sam-Pa
	QString terminal; //!< Category of the word
	int probability; //!< Probability
	
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
	* @param int probability
	* Recognition probability
	* 
	*/
	Word(QString word, QString pronunciation, QString terminal=NULL, int probability=NULL)
	{
		this->word = word;
		this->pronunciation = pronunciation;
		this->terminal = terminal;
		this->probability = probability;
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
	 * \brief Sets the probability to the given probability
	 * \author Peter Grasch
	 * @param newProbability 
	 */
	void setProbability(int newProbability) { this->probability=newProbability; }
	
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
	 * This is a simple comparison of the two "word" values (retrieved by getWord())
	 * used for sorting
	 * \author Peter Grasch
	 * \param const Word& w2
	 * The second word
	 * \return bool
	 * smaller?
	 */
	const bool operator<(const Word w2) const
	{
		return getWord().toUpper() < w2.getWord().toUpper();
	}
	
	
	const bool operator!=(const Word w2) const
	{
		if ((getWord() != w2.getWord()) || (getTerminal() != w2.getTerminal()) || (getPronunciation() != w2.getPronunciation()))
			return true;
		
		return false;
	}

	
	/**
	 * @brief Getter-Method: probability
	 *
	 * @author Peter Grasch
	 * @return int
	 * Returns the probability (how well it has been trained) of the word
	 * 
	 */
	int getPropability() const
	{
		return this->probability;
	}

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

#endif
