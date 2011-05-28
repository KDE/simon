#ifndef CONTEXTMANAGER_H
#define CONTEXTMANAGER_H

#include <QObject>
#include <QtXml/QDomElement>
#include <QHash>
#include "compoundcondition.h"
#include "simoncontextdetection_export.h"

class SIMONCONTEXTDETECTION_EXPORT ContextManager : public QObject
{
    Q_OBJECT
public:
    static ContextManager* instance();
    ~ContextManager();

    Condition* getCondition(const QDomElement &elem);
    CompoundCondition* getCompoundCondition(const QDomElement &elem);
    
    void test();

private:
    explicit ContextManager(QObject *parent = 0);
    
    static ContextManager* m_instance;
  
    QHash<QString, Condition*> m_conditionLookup;
    QList<Condition*> m_conditions;
    QList<CompoundCondition*> m_compoundConditions;

signals:

public slots:
};

#endif // CONTEXTMANAGER_H
