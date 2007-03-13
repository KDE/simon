//
// C++ Interface: trainingtext
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef TRAININGTEXT_H
#define TRAININGTEXT_H

#include <QList>
/**
 *	@class TrainingText
 *	@brief Convenient class to be used as a container to hold all theinformation for one text
 *
 *	@version 0.1
 *	@date 23.01.2006
 *	@author Peter Grasch
 *	@todo Implementing
 */
class TrainingText;
typedef  QList<TrainingText> TrainingList;

class TrainingText{
public:
    TrainingText();

    ~TrainingText();

};

#endif
