/*
 * create a new library next to simonappcore, simonactionsui, etc. called "simonsampleshare";
 * start by creating a ui (KDialog), that simply says "Hello World";
 * integrate this library and dialog into simon by adding a menu option to "Actions" called "Publish samples" displaying the
 * dialog from the new library
 * 
 */
//
#include "sampleShare.h"
sampleShare::sampleShare(QWidget* parent) : KXmlGuiWindow(parent){
  
  ui.setupUi(this);
  ui.label->setText("Hello World!");

}