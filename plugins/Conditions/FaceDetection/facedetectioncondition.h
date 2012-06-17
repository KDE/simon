/*
*   Copyright (C) 2012 Yash Shah <blazonware@gmail.com>
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

#ifndef FACEDETECTIONCONDITION_H
#define FACEDETECTIONCONDITION_H

class FaceAnalyzer;
#include "simoncontextdetection/condition.h"
#include "simoncontextdetection/simoncontextdetection_export.h"
#include "simoncontextdetection/compoundcondition.h"


class FaceDetectionCondition : public Condition
{
    Q_OBJECT
public:
    explicit FaceDetectionCondition(QObject *parent, const QVariantList& args);
    ~FaceDetectionCondition();
    virtual QString name();

    virtual CreateConditionWidget* getCreateConditionWidget(CompoundCondition *compoundCondition, QWidget *parent);

private:
    bool privateDeSerialize(QDomElement elem);
    QDomElement privateSerialize(QDomDocument *doc, QDomElement elem);
    FaceAnalyzer* analyzer;

public slots:
    // Slot is connect to the signal in the Analyzer which will be emmitted when there will be any change the presense of user (hasFace)
    void manageConditionState(bool hasFace);

};

#endif // FACEDETECTIONCONDITION_H
