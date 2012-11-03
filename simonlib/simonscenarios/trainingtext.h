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

#ifndef SIMON_TRAININGTEXT_H_781AFF7F16174193A6AC665BA5840256
#define SIMON_TRAININGTEXT_H_781AFF7F16174193A6AC665BA5840256

#include <QStringList>
#include <simonscenariobase/scenarioobject.h>
#include "simonmodelmanagement_export.h"
#include <KDebug>

/**
 *	@class TrainingText
 *	@brief Convenient class to be used as a container to hold all theinformation for one text
 *
 *	@version 0.1
 *	@date 23.01.2006
 *	@author Peter Grasch
 */
class Scenario;

class MODELMANAGEMENT_EXPORT TrainingText : public ScenarioObject
{
  protected:
    QString name;
    QStringList pages;
    TrainingText( Scenario *parent );
  public:
    TrainingText(const QString& name, const QStringList& pages, Scenario* parentScenario);

    static TrainingText* createTrainingText(Scenario *parent, const QDomElement& elem);

    bool deSerialize(const QDomElement&);
    QDomElement serialize(QDomDocument *doc);

    /**
     * \brief Returns the name of the text
     * \author Peter Grasch
     * \return
     * Name
     */
    QString getName() const { return this->name; }

    /**
     * \brief Returns the page count of the text
     * \author Peter Grasch
     * \return
     * count of pages
     */
    int getPageCount() const { return pages.count(); }

    /**
     * \brief Returns the pages of the text
     * \author Peter Grasch
     * \return
     * pages
     */
    QStringList getPages() const { return pages; }

    /**
     * \brief Returns the page <page> of the text
     * \author Peter Grasch
     * \return
     * page
     */
    QString getPage( int page ) const
    {
      return pages.at(page);
    }

    /**
     * \brief Returns the relevance of the text
     *
     * The higher the value, the more the text would improve the recognition rate
     *
     * \author Peter Grasch
     * \return
     * relevance
     */
    float getRelevance();

    ~TrainingText();

};
#endif
