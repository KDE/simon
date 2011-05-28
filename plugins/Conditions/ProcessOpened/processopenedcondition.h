#ifndef PROCESSOPENEDCONDITION_H
#define PROCESSOPENEDCONDITION_H

#include "simoncontextdetection/condition.h"
#include "simoncontextdetection/processinfo.h"
#include "simoncontextdetection/simoncontextdetection_export.h"

class SIMONCONTEXTDETECTION_EXPORT ProcessOpenedCondition : public Condition
{
    Q_OBJECT
public:
    explicit ProcessOpenedCondition(QObject *parent, const QVariantList& args);

    QString getName() {return m_processName;}

private:
    void privateDeSerialize(QDomElement elem);

    QString m_processName;

signals:

public slots:
    void checkRemovedProcess(QString name);
    void checkAddedProcess(QString name);
};

#endif // PROCESSOPENEDCONDITION_H
