#ifndef NETWORKREADER_H_
#define NETWORKREADER_H_


#include <QtNetwork>
#include "sync_filereader.h"
#include "sync_networkconfiguration.h"

class NetworkReader : public Reader
{

	//Q_OBJECT

	private:
		QSslSocket *client;

		qint16 size;
		QString _Filename;
		QByteArray out;

	public:
		
		NetworkReader(QSslSocket *socket, QByteArray out);
		virtual void createMergeContainer();

};

#endif //NETWORKREADER_H_
