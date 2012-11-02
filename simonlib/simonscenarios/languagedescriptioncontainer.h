/*
 *   Copyright (C) 2008 Peter Grasch <peter.grasch@bedahr.org>
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

#ifndef SIMON_LANGUAGEDESCRIPTIONCONTAINER_H_E52B05FBE8084EF58429BDC3E05DAD63
#define SIMON_LANGUAGEDESCRIPTIONCONTAINER_H_E52B05FBE8084EF58429BDC3E05DAD63

#include "simonmodelmanagement_export.h"
#include <QByteArray>

class MODELMANAGEMENT_EXPORT LanguageDescriptionContainer
{
  private:
    QByteArray m_shadowVocab;
    QByteArray m_treeHed;
    QByteArray m_languageProfile;

  public:
    LanguageDescriptionContainer(const QByteArray& shadowVocab, const QByteArray& languageProfile=QByteArray());

    ~LanguageDescriptionContainer();

    QByteArray shadowVocab() { return m_shadowVocab; }
    QByteArray languageProfile() { return m_languageProfile; }

};
#endif
