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
#include <QStringList>
#include <QList>

/**
 *	@class Word
 *	@brief Represents the Information of one word
 *
 *	This class contains all information to ressemble one word in the Dictionary
 *	It can handle multiple pronounciations of the word
 *
 *	@version 0.1
 *	@date 07.01.2006
 *	@author Peter Grasch
 *	@todo Implementing functions
 */

class Word;

typedef QList<Word*> WordList; //!< QList from Word

class Word{
	
	QString word; //!< Saves the represented word
	QStringList pronounciations; //!< Saves all valid pronounciations of the word in Sam-Pa
	QString terminal; //!< Category of the word
	int probability; //!< Probability
	
public:
	
	/**
	* @brief Constructor
	*
	* This is the constructor of the Word class.
	* It features the pronounciations of the word as a QStringList
	* which is passed on to the member
	*
	* @author Peter Grasch
	* @param QString word
	* Contains the written word
	* @param QStringList pronounciations
	* A List of pronounciations - each an individual pronounciation of the word
	* @param QString terminal
	* Terminal the word belongs to
	* @param int probability
	* Recognition probability
	* 
	*/
	Word(QString word, QStringList pronounciations, QString terminal=NULL, int probability=NULL)
	{
		this->word = word;
		this->pronounciations = pronounciations;
		this->terminal = terminal;
		this->probability = probability;
	}
	
	/**
	 * @brief Constructor
	 *
	 * This is the constructor of the Word class.
	 * It features the pronounciations of the word as a QString
	 * This, overloaded, constructor is intended to be used in such
	 * cases where there is only one pronounciation of the word or the
	 * using class does not know QStringList
	 *
	 * @author Peter Grasch
	 * @param QString word
	 * Contains the written word
	 * @param QString pronounciation
	 * Sam-Pa pronounciation
	 * @param QString terminal
	 * Terminal the word belongs to
	 * @param int probability
	 * Recognition probability
	 * 
	 */
	Word(QString word, QString pronounciation, QString terminal=NULL, int probability=NULL)
	{
		this->word = word;
		this->pronounciations = QStringList();
		this->pronounciations << pronounciation;
		this->terminal = terminal;
		this->probability = probability;
	}

	/**
	 * @brief Adding a new Pronounciation
	 *
	 * Adds a new Pronounciation to the word
	 *
	 * @author Peter Grasch
	 * @param QString pronounciation
	 * The new pronounciation
	 * @todo Maybe check if the Pronounciation is already stored?
	 * 
	 */	
	void addPronounciation (QString pronounciation)
	{
		this->pronounciations.append(pronounciation);
	}
	
	/**
	 * @brief Deletes a pronounciation
	 *
	 * Searches for the given pattern in the QStringList and passes it on to the
	 * overloaded delPronounciation (int) function which actually deletes the item
	 *
	 * @author Peter Grasch
	 * @see delPronounciation (int i)
	 * @param QString pronounciation
	 * The pronounciation to delete
	 * 
	 */
	void delPronounciation ( QString pronounciation )
	{
		//get index of the pattern
		//call the delPronounciation-function again - this time with the index
		//as paramter
		this->delPronounciation( this->pronounciations.indexOf(pronounciation) );
	}
	
	/**
	 * @brief Deletes a pronounciation
	 *
	 * Deletes the pronounciation i
	 *
	 * @author Peter Grasch
	 * @see delPronounciation (int i)
	 * @param QString pronounciation
	 * The index of the pronounciation to delete
	 * 
	 */
	void delPronounciation ( int i )
	{
		this->pronounciations.removeAt( i );
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
	QString getWord() 
	{
		return this->word;
	}
	
	
	/**
	 * @brief Getter-Method: Pronounciation
	 *
	 * Returns a pointer of the pronounciation[i] of the word
	 * If there is no pronounciation i - NULL is returned
	 *
	 * @author Peter Grasch
	 * @param int i
	 * Index of the pronounciation
	 * @return QString*
	 * Contains the pronounciation i
	 * 
	 */
	const QString* getPronounciation ( int i )
	{
		if (this->pronounciations.count() > i)
			return &(this->pronounciations.at( i ));
		else return NULL;
	}
	
	/**
	 * @brief Getter-Method: Pronounciations
	 *
	 * Returns a QStringList Object of all the pronounciations
	 *
	 * @author Peter Grasch
	 * @return QStringList
	 * Contains the pronounciations of the word
	 * 
	 */
	QStringList getPronounciations() 
	{
		return this->pronounciations;
	}
	
	/**
	 * @brief Getter-Method: terminal
	 *
	 * @author Peter Grasch
	 * @return QString
	 * Returns the terminal (category) of the word
	 * 
	 */
	QString getTerminal() 
	{
		return this->terminal;
	}

	
	/**
	 * @brief Getter-Method: probability
	 *
	 * @author Peter Grasch
	 * @return int
	 * Returns the probability (how well it has been trained) of the word
	 * 
	 */
	int getPropability() 
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
    ~Word();

};

#endif
