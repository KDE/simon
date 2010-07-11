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

#ifndef SIMON_TESTRESULT_H_8163A007180343A3A027C402EDCE6C93
#define SIMON_TESTRESULT_H_8163A007180343A3A027C402EDCE6C93

#include <simonrecognitionresult/recognitionresult.h>
#include "simonmodeltest_export.h"

class MODELTEST_EXPORT TestResult
{
  private:
    QString prompt;
    RecognitionResultList results;
  public:
    TestResult(const QString& prompt, const RecognitionResultList& results);
    QString getPrompt() const { return prompt; }
    RecognitionResultList getResults() const { return results; }

};
#endif
