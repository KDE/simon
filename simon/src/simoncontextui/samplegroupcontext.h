#ifndef SAMPLEGROUPCONTEXT_H
#define SAMPLEGROUPCONTEXT_H

#include <QWidget>
#include "simoncontextui_export.h"
#include "simoncontextdetection/contextmanager.h"
#include "simoncontextdetection/samplegroupconditionmodel.h"

namespace Ui {
    class SampleGroupContext;
}

class SIMONCONTEXTUI_EXPORT SampleGroupContext : public QWidget
{
    Q_OBJECT

public:
    explicit SampleGroupContext(QWidget *parent = 0);
    ~SampleGroupContext();

    Condition* getCurrentCondition();

private:
    Ui::SampleGroupContext *ui;

    SampleGroupConditionModel* m_model;

public slots:
    void addCondition();
    void editCondition();
    void removeCondition();
    void promoteCondition();
    void demoteCondition();
};

#endif // SAMPLEGROUPCONTEXT_H
