#include "sync_syncmanager.h"
#include <QMessageBox>
#include "sync_factory.h"

SyncManager::SyncManager()
{
	FirstIsSetFlag = false;
}

void SyncManager::setMergeContainer(MergeContainer *container, Configuration *config)
{
	if(!FirstIsSetFlag)
	{
		this->confone = config;
		this->ContainerOne = container;
		this->FirstIsSetFlag = true;
	}
	else
	{
		this->conftwo = config;
		this->ContainerTwo = container;
		this->ContainerFinal = this->merger->mergeProcess(ContainerOne, ContainerTwo);
		this->sendFinalContainer();
	}
}

void SyncManager::sendFinalContainer()
{
	Factory factory;
	Writer* writerOne = factory.getWriter(this->confone);
	Writer* writerTwo = factory.getWriter(this->conftwo);
	
	writerOne->writeMergedData(this->ContainerFinal);
	writerTwo->writeMergedData(this->ContainerFinal);
}
