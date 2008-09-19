#include "sync_writer.h"
#include "sync_networkconfiguration.h"

class NetworkWriter : public Writer
{
public:
	NetworkWriter(Configuration *conf)
	:Writer(conf)
	{

	}
	
	void writeMergedData(MergeContainer *container);
};
