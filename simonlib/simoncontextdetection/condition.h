#ifndef CONDITION_H
#define CONDITION_H

#include <QObject>
#include <QtXml/QDomElement>
#include "simoncontextdetection_export.h"

class SIMONCONTEXTDETECTION_EXPORT Condition : public QObject
{
    Q_OBJECT
public:
    explicit Condition(QObject *parent, const QVariantList &args);

    bool isSatisfied();

    void serialize(QDomDocument *doc);
    void deSerialize(QDomElement elem);

protected:
    virtual void privateDeSerialize(QDomElement elem)=0;

    bool m_satisfied;

private:
    bool m_inverted;

signals:
    void conditionChanged();

public slots:

};

#endif // CONDITION_H
