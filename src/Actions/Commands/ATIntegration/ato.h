#ifndef ATO_H
#define ATO_H
#include <QMetaType>

 struct ATOLocation
 {
     qint32 x;
     qint32 y;
     qint32 width;
     qint32 height;
 };
Q_DECLARE_METATYPE(ATOLocation)
 
 struct ATOPosition
 {
     qint32 x;
     qint32 y;
 };
Q_DECLARE_METATYPE(ATOPosition)

enum ATOMenuType {
	None=0,
	MenuBar=1,
	ToolBar=2,
 	Entry=3
};
struct ATOMenu {
	QString title;
	ATOMenuType type;
	ATOLocation geometry;
	QList<ATOMenu*> actions;
};

#endif

