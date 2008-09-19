#ifndef SYNCMANAGER_H_
#define SYNCMANAGER_H_

#include "sync_merger.h"
#include "sync_writer.h"
#include "sync_configuration.h"

class SyncManager : public QObject
{
	Q_OBJECT

	public:
		SyncManager();
		//abfrage mittels der ersten beiden Container, wohin der Final gespeichert werden soll
		void sendFinalContainer();
	
	private:
		Configuration *confone;
		Configuration *conftwo;
		Merger *merger;
		MergeContainer *ContainerOne;
		MergeContainer *ContainerTwo;
		MergeContainer *ContainerFinal;
		bool FirstIsSetFlag;
		
	public slots:
		 void setMergeContainer(MergeContainer *container, Configuration *config);
	
};


#endif //SYNCMANAGER_H_
