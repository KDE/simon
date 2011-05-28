#ifndef CONDITIONASSOCIATION_H
#define CONDITIONASSOCIATION_H

#include "condition.h"
#include "simoncontextdetection_export.h"

class SIMONCONTEXTDETECTION_EXPORT ConditionAssociation : public Condition
{
    Q_OBJECT
public:
    explicit ConditionAssociation(QObject *parent, const QVariantList &args);

protected:
    QList<Condition*> m_conditions;
    void privateDeSerialize(QDomElement elem);

signals:

public slots:
    virtual void evaluateConditions()=0;
};

#endif // CONDITIONASSOCIATION_H
