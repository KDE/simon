//
// C++ Interface: postprocessing
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef POSTPROCESSING_H
#define POSTPROCESSING_H

#include <QString>
/**
 \class PostProcessing
 \author Peter Grasch
 \version 0.1
 \date 19.2.2008
 \brief Applies the specified postprocessing stack to the given filenames
*/
class PostProcessing{
public:
    PostProcessing();

	bool process(QString in, QString out, bool deleteIn=false);

    ~PostProcessing();

};

#endif
