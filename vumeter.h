//
// C++ Interface: vumeter
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef VUMETER_H
#define VUMETER_H

#include <QThread>
#include "miccontrol.h"

/**
 *	@class VuMeter
 *	@brief Measures the current "loudness" of the mic-in
 *
 *	@version 0.1
 *	@date 5.02.2007
 *	@author Peter Grasch
 */
class VuMeter : public QThread
{
	Q_OBJECT
signals:
	void level(int level); //!< publishs the current "loudness"
private:
	MicControl *mic;
public:
	VuMeter();
	void run();
	bool prepare();
	void exec();
    ~VuMeter();

};

#endif
