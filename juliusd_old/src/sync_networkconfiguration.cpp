#include "sync_networkconfiguration.h"

NetworkConfiguration::NetworkConfiguration(int type, QSslSocket *socket, QString filename)
:Configuration(type)
{
	this->_socket = socket;
	this->_filename = filename;
}

