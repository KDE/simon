#ifndef SAMPLESHARE_H
#define SAMPLESHARE_H
#include <KXmlGuiWindow>

#include "ui_sampleShare.h"
/**
 \class GeneralSettings
 \author Alessandro Buggin
 \version 0.1
 \date 23.05.2011

 \brief Used to display a sample share kdialog saying "Hello World"

*/

class sampleShare:public KXmlGuiWindow
{
  Q_OBJECT 
  
public:
  sampleShare(QWidget *parent=0);
  
private:
  void setupActions();
  Ui_sampleShare ui;
};

#endif