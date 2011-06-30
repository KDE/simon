#ifndef SAMPLESHARE_H
#define SAMPLESHARE_H

#include <KDialog>
<<<<<<< HEAD
#include "sampleshareui_export.h"


/**
 \class SampleShare
 \author Alessandro Buggin
 \version 0.1a
=======

#include <ui_sampleshare.h>
/**
 \class GeneralSettings
 \author Alessandro Buggin
 \version 0.1
>>>>>>> a9304975c03500b8730c821cd44909183cfc613c
 \date 23.05.2011

 \brief Used to display a sample share kdialog saying "Hello World"

*/
<<<<<<< HEAD
 namespace Ui {
    class SampleShareDlg;
}
class SAMPLESHAREUI_EXPORT SampleShare : public KDialog{
  Q_OBJECT
  
private:
  Ui::SampleShareDlg *ui;
  
public:
  explicit SampleShare(QWidget *parent=0);
//  void SetupActions();


};
=======

class SampleShare:public KDialog
{
  Q_OBJECT 

private:
  Ui::SampleShare ui;
  void SetupActions();
  void initDisplay();

public:
  SampleShare(QWidget *parent=0);
  ~SampleShare();
  
  int exec();

};

>>>>>>> a9304975c03500b8730c821cd44909183cfc613c
#endif