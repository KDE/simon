#ifndef FILECONFIGURATION_H_
#define FILECONFIGURATION_H_

#include "sync_configuration.h"

class FileConfiguration : public Configuration
{

public:
	FileConfiguration(int type, QString path)
	:Configuration(type)
	,_path(path)
	{
		
	}
	~FileConfiguration() {}

	QString getPath(){return this->_path;}

private:
	QString _path;
};

#endif //FILECONFIGURATION_H_
