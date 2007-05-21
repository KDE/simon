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
#include <QMessageBox>
#include <QList>
#include <QDebug>

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

typedef QList<Word*> WordList; //!< QList from Word

class Word{
	
	QString word; //!< Saves the represented word
	QStringList pronunciations; //!< Saves all valid pronunciations of the word in Sam-Pa
	QString terminal; //!< Category of the word
	int probability; //!< Probability
	
public:
	
	/**
	* @brief Constructor
	*
	* This is the constructor of the Word class.
	* It features the pronunciations of the word as a QStringList
	* which is passed on to the member
	*
	* @author Peter Grasch
	* @param QString word
	* Contains the written word
	* @param QStringList pronunciations
	* A List of pronunciations - each an individual pronounciation of the word
	* @param QString terminal
	* Terminal the word belongs to
	* @param int probability
	* Recognition probability
	* 
	*/
	Word(QString word, QStringList pronunciations, QString terminal=NULL, int probability=NULL)
	{
		this->word = word;
		this->pronunciations = pronunciations;
		this->terminal = terminal;
		this->probability = probability;
	}
	
	/**
	 * @brief Constructor
	 *
	 * This is the constructor of the Word class.
	 * It features the pronunciations of the word as a QString
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
		this->pronunciations = QStringList();
		this->pronunciations << pronounciation;
		this->terminal = terminal;
		this->probability = probability;
	}

	/**
	 * @brief Adding a new Pronunciations
	 *
	 * Adds a new Pronunciations to the word
	 *
	 * @author Peter Grasch
	 * @param QString pronounciation
	 * The new pronounciation
	 * @todo Maybe check if the Pronunciations is already stored?
	 * 
	 */	
	void addPronunciations (QString pronounciation)
	{
		this->pronunciations.append(pronounciation);
	}
	
	/**
	 * @brief Deletes a pronounciation
	 *
	 * Searches for the given pattern in the QStringList and passes it on to the
	 * overloaded delPronunciations (int) function which actually deletes the item
	 *
	 * @author Peter Grasch
	 * @see delPronunciations (int i)
	 * @param QString pronounciation
	 * The pronounciation to delete
	 * 
	 */
	void delPronunciations ( QString pronounciation )
	{
		//get index of the pattern
		//call the delPronunciations-function again - this time with the index
		//as paramter
		this->delPronunciations( this->pronunciations.indexOf(pronounciation) );
	}
	
	/**
	 * @brief Deletes a pronounciation
	 *
	 * Deletes the pronounciation i
	 *
	 * @author Peter Grasch
	 * @see delPronunciations (int i)
	 * @param QString pronounciation
	 * The index of the pronounciation to delete
	 * 
	 */
	void delPronunciations ( int i )
	{
		this->pronunciations.removeAt( i );
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
	 * @brief Getter-Method: Pronunciations
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
	const QString* getPronunciation ( int i ) const
	{
		if (this->pronunciations.count() > i)
			return &(this->pronunciations.at( i ));
		else return NULL;
	}
	
	/**
	 * @brief Getter-Method: Pronunciations
	 *
	 * Returns a QStringList Object of all the pronunciations
	 *
	 * @author Peter Grasch
	 * @return QStringList
	 * Contains the pronunciations of the word
	 * 
	 */
	QStringList getPronunciations() const
	{
		return this->pronunciations;
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
		
// 		return (getWord().trimmed().compare(w2.getWord().trimmed()));
		
		if ((getWord().trimmed().toUpper() == "BUTAN") || (w2.getWord().trimmed().toUpper() == "BUTAN"))
			QMessageBox::information(0, "Hallo", "Vergleiche: "+getWord().trimmed().toUpper()+" und "+w2.getWord().trimmed().toUpper()+" und das is: "+((getWord().trimmed().toUpper() < w2.getWord().trimmed().toUpper()) ? "Kleiner" : "Nicht kleiner"));
		
		return getWord().trimmed().toUpper() < w2.getWord().trimmed().toUpper();
		
// 		QString word1=getWord().toUpper();
// 		QString word2=w2.getWord().toUpper();
// 		
// 		if (word1.startsWith(word2))
// 		{
// 			if (word1.size() < word2.size())
// 				return true;
// 			else return false;
// 		} else if (word2.startsWith(word1))
// 			{
// 				if (word1.size() < word2.size())
// 					return true;
// 				else return false;
// 			}
// 		
// 		return (word1.toUpper() < word2.toUpper());
		
		
// 		QStringList words;
// 		words << getWord().trimmed();
// 		words << w2.getWord().trimmed();<
// 		words.sort();
// 		if ((words.at(0) == "AACHEN") || (words.at(1) == "AACHEN"))
// 			QMessageBox::information(0, "sdifj", words.at(0)+" ist kleiner als "
// 					+words.at(1));
		
// 		return (words.at(0) == getWord());
		
// 		QString s1;
// 		QString s2;
// 		QString wo1 = getWord();
// 		QString wo2 = w2.getWord();
// 		
// 		if (wo1.size() < wo2.size()){ s1 = wo1; s2 = wo2.left(wo1.size()); }
// 		else { s1 = wo1.left(wo2.size()); s2 = wo2; }
// 		
// 		if (s1 == s2)
// 			return (wo1.size() < wo2.size());
// 		else return (s1<s2);
		

		
// 		
// 		QString s1 = getWord().toUpper();
// 		QString s2 = w2.getWord().toUpper();
// 		QString t;
// 		if (s2.size() < s1.size())
// 		{
// 			t = s1;
// 			s1 = s2;
// 			s2 = t;
// 		}
// 		
// 		//s1.size() < s2.size
// 		
// 		for (int i=0; i<s1.size(); i++)
// 		{
// 			if (s1.at(i) < s2.at(i)) 
// 			{ 
// 				/*QMessageBox::information(0, "sidjf", s1+" < "+s2);*/
// 				if (((getWord().toUpper() == "ABER") && 
// 					((w2.getWord().toUpper() == "AB"))) ||
// 						((w2.getWord().toUpper() == "ABER") && 
// 						((getWord().toUpper() == "AB"))))
// 						QMessageBox::information(0, "sidjf", s1+" < "+s2);
// 				return true; 
// 			}
// 			else if (s1.at(i) > s2.at(i))
// 			{ 
// 				if (((getWord().toUpper() == "ABER") && 
// 						((w2.getWord().toUpper() == "AB"))) ||
// 						((w2.getWord().toUpper() == "ABER") && 
// 						((getWord().toUpper() == "AB"))))
// 					QMessageBox::information(0, "sidjf", s1+" < "+s2);
// 				/*QMessageBox::information(0, "sidjf", s1+" > "+s2);*/ 
// 				return false; 
// 			}
// 		}
// 		return (s1.size() < s2.size());
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
    ~Word() {}

};

#endif
