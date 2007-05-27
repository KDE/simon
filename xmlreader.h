#ifndef XMLREADER_H
#define XMLREADER_H
#include <QObject>
/**
 *  @class XMLReader
 *  @brief To handle with xml-files
 *
 *  @version 0.1
 *  @date 17.03.2007
 *  @author Martin Gigerl, Peter Grasch
 */
 

 
class XMLReader : public QObject {
 	Q_OBJECT
signals:
	void loaded();
	void closed();
	void written();

protected:
 	QString path;
 
public:
	XMLReader(QString path, QObject *parent=0);
	
	~XMLReader();
};

#endif
