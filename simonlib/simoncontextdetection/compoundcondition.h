#ifndef COMPOUNDCONDITION_H
#define COMPOUNDCONDITION_H

#include <QObject>
#include "condition.h"

class CompoundCondition : public QObject
{
    Q_OBJECT
public:
    bool isSatisfied() {return m_satisfied;}
    QDomElement serialize(QDomDocument *doc);
    static CompoundCondition* createInstance(const QDomElement &elem);

private:
    explicit CompoundCondition(QObject *parent = 0);
    bool deSerialize(const QDomElement &elem);

    bool m_satisfied;
    QList<Condition*> m_conditions;

signals:
    void conditionChanged(bool);

public slots:
    void evaluateConditions();
};

#endif // COMPOUNDCONDITION_H
