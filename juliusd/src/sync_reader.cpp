#include "sync_reader.h"


Reader::Reader(SyncManager *man)
{
	this->manager = man;
	connect(this,SIGNAL(readyContainer(MergeContainer*, Configuration*)),manager,SLOT(setMergeContainer(MergeContainer*, Configuration*)));
}

void Reader::createMergeContainer()
{
	
}

