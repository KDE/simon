#ifndef SAMPLESHARE_H
#define SAMPLESHARE_H

#include <KDialog>
#include "sampleshareui_export.h"

/**
 \class SampleShare
 \author Alessandro Buggin
 \version 0.2
 \date 04.07.2011

 \brief Used to display a sample share kdialog saying "Hello World"

*/

namespace Ui {
  class SampleShareDlg;
}

class SAMPLESHAREUI_EXPORT SampleShare : public KDialog{
  Q_OBJECT
  
private:
  Ui::SampleShareDlg *ui;
  
public:
  explicit SampleShare(QDialog *parent=0);
//  void SetupActions();


};
#endif