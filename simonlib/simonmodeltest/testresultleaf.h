/*
 *   Copyright (C) 2010 Peter Grasch <grasch@simon-listens.org>
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

#ifndef SIMON_TESTRESULTLEAF_H_8163A007180343A3A027C402EDCE6C93
#define SIMON_TESTRESULTLEAF_H_8163A007180343A3A027C402EDCE6C93

#include "simonmodeltest_export.h"
#include <QString>

class MODELTEST_EXPORT TestResultLeaf
{
  private:
    QString m_label;
    QString m_pronunciation;
    float m_accuracy;
    bool m_insertionError, m_substitutionError, m_deletionError;

  public:
    TestResultLeaf(const QString& label, const QString& pronunciation, float accuracy);
    TestResultLeaf();

    void setInsertionError(bool f) { m_insertionError = f; }
    void setSubstitutionError(bool f) { m_substitutionError = f; }
    void setDeletionError(bool f) { m_deletionError = f; }

    bool insertionError() { return m_insertionError; }
    bool substitutionError() { return m_substitutionError; }
    bool deletionError() { return m_deletionError; }

    float accuracy() { return m_accuracy; }
    QString label() { return m_label; }
    QString prompt() { return m_pronunciation; }

};
#endif

