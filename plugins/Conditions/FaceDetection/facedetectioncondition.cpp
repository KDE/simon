#include "facedetectioncondition.h"
#include <KDebug>
#include "createfacedetectionconditionwidget.h"
#include <QWidget>
#include "simonvision/faceanalyzer.h"

K_PLUGIN_FACTORY( FaceDetectionPluginFactory,
                  registerPlugin< FaceDetectionCondition >();
                )

K_EXPORT_PLUGIN( FaceDetectionPluginFactory("simonfacedetectioncondition") )

FaceDetectionCondition::FaceDetectionCondition(QObject *parent, const QVariantList &args) :
        Condition(parent, args)
{
    m_pluginName = "simonfacedetectionconditionplugin.desktop";
}

CreateConditionWidget* FaceDetectionCondition::getCreateConditionWidget(CompoundCondition *compoundCondition, QWidget* parent)
{
    return new CreateFaceDetectionConditionWidget(compoundCondition, parent);
}

QDomElement FaceDetectionCondition::privateSerialize(QDomDocument *doc, QDomElement elem)
{

    return elem;
}

QString FaceDetectionCondition::name()
{
    if (!isInverted())
        return i18nc("Detecting the presense of user from the webcam", "Presense of user");
    else
        return i18nc("Detecting the presense of user from the webcam", "Absense of user");
}

bool FaceDetectionCondition::privateDeSerialize(QDomElement elem)
{
    analyzer = new FaceAnalyzer();
    connect(analyzer,SIGNAL(facePresenceChanged(bool)),this,SLOT(manageConditionState(bool)));
    return true;
}
void FaceDetectionCondition::manageConditionState(bool hasFace)
{
    if (hasFace)
    {
        m_satisfied = true;
        kDebug() << name() + " is true!";
        emit conditionChanged();
    }
    else
    {
        m_satisfied = false;
        kDebug() << name() + " is false!";
        emit conditionChanged();
    }

}
FaceDetectionCondition::~FaceDetectionCondition()
{

}


