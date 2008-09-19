#include "sync_configuration.h"

// forward
class Writer;

class Factory
{
public:
	Writer* getWriter(Configuration *config);

};
