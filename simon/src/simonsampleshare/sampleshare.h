#ifndef SAMPLESHARE_H
#define SAMPLESHARE_H

#include <KDialog>

#include <ui_sampleshare.h>
/**
 \class GeneralSettings
 \author Alessandro Buggin
 \version 0.1
 \date 23.05.2011

 \brief Used to display a sample share kdialog saying "Hello World"

*/

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

#endif