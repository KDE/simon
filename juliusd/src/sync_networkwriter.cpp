#include "sync_networkwriter.h"
#include <QMessageBox>

void NetworkWriter::writeMergedData(MergeContainer *container)
{
	QString data;
	QString stamp;

	QByteArray block;
	QDataStream out(&block, QIODevice::WriteOnly);
	out.setVersion(QDataStream::Qt_4_1);

	QString filename = dynamic_cast<NetworkConfiguration*>(this->_config)->getFilename();

	
	out << qint32(99990);
	
	out << filename;

	int count = container->vsp.count();
	out << count;

	for(int i = 0; i < container->vsp.count(); ++i)
	{
		QDateTime dt = container->vsp.at(i)->second;
		QString stamp = dt.toString("yyyy.MM.dd-hh:mm:ss:zzz");
		out << container->vsp.at(i)->first << stamp;
	}
	
	dynamic_cast<NetworkConfiguration*>(this->_config)->getSocket()->write(block);
}
