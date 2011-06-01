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

    QDomElement serialize(QDomDocument *doc);
    bool deSerialize(QDomElement elem);

protected:
    virtual bool privateDeSerialize(QDomElement elem)=0;
    virtual QDomElement privateSerialize(QDomDocument *doc, QDomElement elem)=0;

    bool m_satisfied;
    QString m_pluginName;

private:
    bool m_inverted;

signals:
    void conditionChanged();

public slots:

};

#endif // CONDITION_H
