#ifndef CONTEXTVIEW_H
#define CONTEXTVIEW_H

#include <simonuicomponents/inlinewidget.h>
#include "simoncontextui_export.h"
#include <simonscenarios/scenariodisplay.h>

class ContextViewPrivate;
class Scenario;

class SIMONCONTEXTUI_EXPORT ContextView : public InlineWidget, public ScenarioDisplay
{
  Q_OBJECT

    signals:
  void actionsChanged();

  private:
    ContextViewPrivate *d;

  protected:
    void displayScenarioPrivate(Scenario *scenario);

  public:
    ContextView(QWidget *parent);

    ~ContextView();

};

#endif // CONTEXTVIEW_H
