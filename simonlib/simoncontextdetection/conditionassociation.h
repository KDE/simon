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
    bool privateDeSerialize(QDomElement elem);
    QDomElement privateSerialize(QDomDocument *doc, QDomElement elem);
    QString m_pluginName;

signals:

public slots:
    virtual void evaluateConditions()=0;
};

#endif // CONDITIONASSOCIATION_H
