#include <QtTest>
#include <QtCore>
#include "../staticboundvalue.h"
 
class testStaticBoundValue: public QObject
{
  Q_OBJECT
  private slots:
    virtual void testSetup();
};

void testStaticBoundValue::testSetup()
{
  StaticBoundValue *value = new StaticBoundValue("test", "value");
  QCOMPARE(value->getValue(), QVariant("value"));
}
  
QTEST_MAIN(testStaticBoundValue)

#include "staticboundvaluetest.moc"
