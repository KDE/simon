#ifndef NETWORKCONFIGURATION_H_
#define NETWORKCONFIGURATION_H_

#include "sync_configuration.h"

class NetworkConfiguration : public Configuration
{

public:
	NetworkConfiguration(int type, QSslSocket *socket, QString filename);
	QSslSocket* getSocket() {return this->_socket;}
	QString getFilename() {return this->_filename;}
	 ~NetworkConfiguration() {}


private:
	QSslSocket *_socket;
	QString _filename;
};

#endif //NETWORKCONFIGURATION_H_
