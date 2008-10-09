#ifndef READER_H_
#define READER_H_


#include "sync_syncmanager.h"



class Reader : public QObject
{
	Q_OBJECT
	
	public:
		Reader(SyncManager *man);
		virtual void createMergeContainer();
	
	protected:
		SyncManager *manager;
		MergeContainer *container;


	signals:
		void readyContainer(MergeContainer* container, Configuration *config);
	
};

#endif //READER_H_
