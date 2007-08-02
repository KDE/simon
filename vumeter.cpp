//
// C++ Implementation: vumeter
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "vumeter.h"


/**
 * @brief Destructor
 * 
 * Initializes the mic (SoundControl*) (member)
 *
 *	@author Peter Grasch
 */
VuMeter::VuMeter() : QThread ()
{
	//mic = new SoundControl();
}

/**
 * @brief Prepares the mic
 * 
 *	@author Peter Grasch
 * @return bool
 * success
 */
bool VuMeter::prepare()
{
	return true;//mic->initializeMic( (short) 2, 44100 );
}

/**
 * @brief Runs the thread
 * 
 * Calls the exec functions and waits to clean up before stopping
 *
 *	@author Peter Grasch
 */
void VuMeter::run()
{
	exec();
// 	wait(5000);
}

/**
 * @brief Main execution loop
 *
 * While the thread is running it stays in the main event loop which is:
 * 	capture 3 frames
 * 	calculate the average and publish it as the current "loudness" by emitting the level(...) signal
 * 	sleep for 80 ms
 *
 *	@author Peter Grasch
 */
void VuMeter::exec()
{
	/*if (!mic) return;
	
	while (true)
	{
		unsigned long int length;
		short *buffer = (short*) mic->capture(3,length);
		if (length > 2)
			if (buffer)
				emit level((buffer[0]+buffer[1]+buffer[2]) / 3);
		
		msleep( 80 );
	}*/
	
}


/**
 * @brief Constructor
 * 
 * Tells the SoundControl to close the handle
 *
 *	@author Peter Grasch
 */
VuMeter::~VuMeter()
{
	//mic->closeMic();
}


