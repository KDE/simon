/*
 *   Copyright (C) 2009 Peter Grasch <peter.grasch@bedahr.org>
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

#ifndef SIMON_WORD_H_93DC69AE6BBC4BD5B1E91F34F29F4417
#define SIMON_WORD_H_93DC69AE6BBC4BD5B1E91F34F29F4417
#include <QString>
#include <QList>
#include <KI18n/klocalizedstring.h>
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

typedef QList<Word> WordList;                     //!< QList from Word

class MODELMANAGEMENT_EXPORT Word
{

  private:
    QString word;                                 //!< Saves the represented word
    QString pronunciation;                        //!< Saves all valid pronunciations of the word in Sam-Pa
    QString category;                             //!< Category of the word
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
     * @param QString category
     * Category the word belongs to
     *
     */
    Word(QString word_, QString pronunciation_, QString category_)
      : word(word_),
      pronunciation(pronunciation_),
      category(category_),
    lexiconWord(word_.toUpper()) {
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
    void setWord(QString word) {
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
    void setPronunciation(QString pronunciation) {
      this->pronunciation = pronunciation;
    }

    /**
     * @brief Getter-Method: category
     *
     * @author Peter Grasch
     * @return QString
     * Returns the category (category) of the word
     *
     */
    QString getCategory() const
    {
      return this->category;
    }

    /**
     * \brief Setter-Method: categories
     * \author Peter Grasch
     * @param category The (new) category of the word
     */
    void setCategory(QString category) {
      this->category = category;
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
          ((getPronunciation() == w2.getPronunciation()) && (getCategory() < w2.getCategory()))));
    }

    bool operator!=(const Word& w2) const
    {
      if ((getWord() != w2.getWord()) || (getCategory() != w2.getCategory()) || (getPronunciation() != w2.getPronunciation()))
        return true;

      return false;
    }

    bool operator==(const Word& w2) const
    {
      return ((getWord() == w2.getWord()) && (getCategory() == w2.getCategory()) && (getPronunciation() == w2.getPronunciation()));
    }

    /**
     * @brief Getter-Method: probability
     *
     * @author Peter Grasch
     * @return int
     * Returns the probability (how well it has been trained) of the word
     *
     */
    int getPropability() const;

    /**
     * @brief Returns true if this word has been explicitly blocked by the used acoustic model
     */
    bool getBlacklisted() const;

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
