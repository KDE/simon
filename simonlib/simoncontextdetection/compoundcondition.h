#ifndef COMPOUNDCONDITION_H
#define COMPOUNDCONDITION_H

#include <QObject>
#include "condition.h"

class CompoundCondition : public QObject
{
    Q_OBJECT
public:
    explicit CompoundCondition(QList<Condition*> conditions, QObject *parent = 0);

    bool isSatisfied() {return m_satisfied;}

private:
    bool m_satisfied;
    QList<Condition*> m_conditions;

signals:
    void conditionChanged(bool);

public slots:
    void evaluateConditions();
};

#endif // COMPOUNDCONDITION_H
