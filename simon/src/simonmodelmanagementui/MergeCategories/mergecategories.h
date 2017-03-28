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

#ifndef SIMON_MERGECATEGORIES_H_8AFCF149C4C744B1A75467E07DD4E402
#define SIMON_MERGECATEGORIES_H_8AFCF149C4C744B1A75467E07DD4E402

#include <QThread>

/**
  @author Peter Grasch <bedahr@gmx.net>
*/

class MergeCategories : public QThread
{
  Q_OBJECT
    signals:
  void status ( QString );
  void progress ( int cur, int max );
  void done();
  private:
    QString newName, categoryA, categoryB;
    bool includeShadow, includeGrammar;
  public:
    void run();

    void setNewName ( QString newName ) { this->newName = newName.trimmed(); }
    void setCategoryA ( QString categoryA ) { this->categoryA = categoryA.trimmed(); }
    void setCategoryB ( QString categoryB ) { this->categoryB = categoryB.trimmed(); }
    void setIncludeShadow ( bool includeShadow ) { this->includeShadow= includeShadow; }
    void setIncludeGrammar ( bool includeGrammar) { this->includeGrammar= includeGrammar; }

    MergeCategories ( QObject* parent );

    ~MergeCategories();

};
#endif
