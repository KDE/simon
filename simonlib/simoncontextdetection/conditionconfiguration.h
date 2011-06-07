#ifndef CONDITIONCONFIGURATION_H
#define CONDITIONCONFIGURATION_H

#include <QWidget>
#include "simoncontextdetection_export.h"

class KComponentData;
class KAboutData;

class SIMONCONTEXTDETECTION_EXPORT ConditionConfiguration : public QWidget
{
    Q_OBJECT

protected:
    KAboutData *m_about;

signals:
    void changed(bool);

private slots:
    void slotChanged();

public:
    ConditionConfiguration(QWidget *parent, const QByteArray& internalName, const KLocalizedString& name,
                         const QByteArray& version, const KLocalizedString& desc,
                         const QString& iconName, const KComponentData& componentData,
                         const QVariantList &args = QVariantList());

    ~ConditionConfiguration();

    KAboutData* aboutData() { return m_about; }

public slots:
    virtual bool deSerialize(const QDomElement& element)=0;
    virtual QDomElement serialize(QDomDocument *doc)=0;
    virtual void defaults()=0;
};

#endif // CONDITIONCONFIGURATION_H
