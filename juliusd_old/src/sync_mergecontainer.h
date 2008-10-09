#ifndef _MERGE_CONTAINER_H_
#define _MERGE_CONTAINER_H_

#include <QVector>
#include <QDateTime>
#include <QPair>

typedef QPair<QString, QDateTime> StampPair;
typedef QVector<StampPair*> VectorStampPair;


class MergeContainer
{
	public:
		VectorStampPair vsp;
		int type; //1 = FileReader; 2 = DBReader; 3 = StreamReader; usw. usw.
	
};

#endif //_MERGE_CONTAINER_H_
