#ifndef FILEREADER_H_
#define FILEREADER_H_

#include <QFile>

#include "sync_reader.h"
#include "sync_fileconfiguration.h"

class FileReader : public Reader
{
	private:
		QString datapath, stamppath;

	public:
		FileReader(SyncManager *man);
		virtual void createMergeContainer();

	public slots:
		void setFiles(QString datapath, QString stamppath);
	
};

#endif //FILEREADER_H_
