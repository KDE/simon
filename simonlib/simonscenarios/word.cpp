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

#include "word.h"
#include "trainingmanager.h"
#include "modelmanager.h"

bool isWordLessThan(Word *w1, Word *w2)
{
  if (w1->getLexiconWord() < w2->getLexiconWord())
    return true;
  else return ((w1->getLexiconWord() == w2->getLexiconWord()) && ((w1->getPronunciation() < w2->getPronunciation()) ||
      ((w1->getPronunciation() == w2->getPronunciation()) && (w1->getTerminal() < w2->getTerminal()))));
}


int Word::getPropability() const
{
  return TrainingManager::getInstance()->getProbability (getWord());
}

bool Word::getBlacklisted() const
{
  return ModelManager::getInstance()->isTranscriptionBlackListed(getPronunciation());
}
