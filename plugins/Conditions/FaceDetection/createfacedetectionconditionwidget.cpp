#include "createfacedetectionconditionwidget.h"
#include "facedetectioncondition.h"
#include "simoncontextdetection/contextmanager.h"



CreateFaceDetectionConditionWidget::CreateFaceDetectionConditionWidget(CompoundCondition *compoundCondition,
QWidget *parent) : CreateConditionWidget(compoundCondition, parent)
{
  ui.setupUi(this);

  setWindowTitle(i18n("Face Detection"));
  setWindowIcon(KIcon(""));
  
  connect(ui.cbInverted, SIGNAL(stateChanged(int)), this, SIGNAL(completeChanged()));
}


bool CreateFaceDetectionConditionWidget::isComplete()
{
  return true;
}

bool CreateFaceDetectionConditionWidget::init(Condition *condition)
{
  Q_ASSERT(condition);

  FaceDetectionCondition *faceDetectionCondition = dynamic_cast<FaceDetectionCondition*>(condition);
  if (!faceDetectionCondition) return false;

  ui.cbInverted->setChecked(faceDetectionCondition->isInverted());
  return true;
}


Condition* CreateFaceDetectionConditionWidget::createCondition()
{
    kDebug() << "Creating Face Detection Condition";
    QDomDocument doc;
    QDomElement conditionElem = doc.createElement("condition");
    conditionElem.setAttribute("name", "simonfacedetectionconditionplugin.desktop");

    QDomElement invertElem = doc.createElement("inverted");
    invertElem.appendChild(doc.createTextNode(ui.cbInverted->isChecked() ? "1" : "0"));
    conditionElem.appendChild(invertElem);


    ContextManager* manager = ContextManager::instance();

    return manager->getCondition(conditionElem);
}


CreateFaceDetectionConditionWidget::~CreateFaceDetectionConditionWidget()
{
}
