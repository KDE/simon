#ifndef SIMONSAMPLESHARE_H
#define SIMONSAMPLESHARE_H

#include <KDialog>
#include "simonsampleshareui_export.h"
#include <ui_sampleshare.h>
#include <KDebug>
/**
 \class SampleShare
 \author Alessandro Buggin
 \version 0.2
 \date 04.07.2011

 \brief Used to display a kdialog about sending user training files to Voxforge

*/

namespace Ui {
  class SampleShareDlg;
}

class SIMONSAMPLESHAREUI_EXPORT SampleShare : public KDialog{
  Q_OBJECT
  
private:
  Ui::SampleShareDlg *ui;
  
public:
  SampleShare( QWidget *parent = 0 );
  ~SampleShare();

private slots:
    void enableButtonUser1(bool down);
};
#endif