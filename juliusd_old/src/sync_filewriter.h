#include "sync_writer.h"
#include "sync_fileconfiguration.h"

class FileWriter : public Writer
{
public:
	FileWriter(Configuration *conf)
	:Writer(conf)
	{

	}
	
	void writeMergedData(MergeContainer *container);
};
