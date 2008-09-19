#ifndef MERGER_H_
#define MERGER_H_


#include "sync_mergecontainer.h"

class Merger 
{

	public:
		Merger();
		MergeContainer* mergeProcess(MergeContainer *contone, MergeContainer *conttwo);
};

#endif //MERGER_H_
