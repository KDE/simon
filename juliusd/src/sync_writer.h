#ifndef _WRITER_H_
#define _WRITER_H_

#include "sync_mergecontainer.h"
#include "sync_configuration.h"

class Writer
{
	public:
		Writer(Configuration *config);
		
		virtual ~Writer() {}
		virtual void writeMergedData(MergeContainer *container);

protected:
	Configuration *_config;
};

#endif //_WRITER_H_
