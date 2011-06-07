#include "conditionconfiguration.h"

ConditionConfiguration::ConditionConfiguration(QWidget *parent,
const QByteArray &internalName, const KLocalizedString &name, const QByteArray &version,
const KLocalizedString &desc, const QString &iconName, const KComponentData &componentData,
const QVariantList &args) : QWidget(parent)
{
    Q_UNUSED(args);
    Q_UNUSED(componentData);

    m_about = new KAboutData(internalName, "", name,
                           version, desc, KAboutData::License_GPL);
    m_about->setProgramIconName(iconName);
}

ConditionConfiguration::~ConditionConfiguration()
{
  delete m_about;
}

void ConditionConfiguration::slotChanged()
{
  emit changed(true);
}
