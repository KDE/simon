#include "sync_networkreader.h"
#include <QMessageBox>

	//DateTimeFormat
	//"yyyy.MM.dd-hh:mm:ss:zzz"

NetworkReader::NetworkReader(QSslSocket *socket, QByteArray out)
:Reader(new SyncManager())
{
	this->client = socket;
	this->container = new MergeContainer();
	//definition of the container type
	this->container->type = 3;
	this->out = out;
}

void NetworkReader::createMergeContainer()
{
	QDataStream stream(&this->out, QIODevice::ReadOnly);
	
	qint32 type;

	stream >> type;
	
	QDateTime dt;
	QString timestamp;
	QString text;
	QString filename;

	stream >> filename;
	FileReader *fr = new FileReader(this->manager);
	fr->setFiles(filename, "shadow"+filename);

	while((text != "[end_file]") && (timestamp != "0000.00.00-00:00:00:000"))
	{
		stream >> text >> timestamp;	
		dt = dt.fromString(timestamp, "yyyy.MM.dd-hh:mm:ss:zzz");
		StampPair *stampp = new StampPair();
		stampp->first = text;
		stampp->second = dt;
		if((text != "[end_file]") && (timestamp != "0000.00.00-00:00:00:000"))
			this->container->vsp.append(stampp);
	}

	NetworkConfiguration *netconfig = new NetworkConfiguration(3,client, filename);
	emit Reader::readyContainer(this->container,netconfig);
}

