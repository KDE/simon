#include "sync_filewriter.h"
#include <QStringList>
#include <QFile>
#include <QByteArray>
#include <QMessageBox>


//TODO Bugfix DateTimeFormat
void FileWriter::writeMergedData(MergeContainer *container)
{	
	QString path = dynamic_cast<FileConfiguration*>(this->_config)->getPath();
	QStringList templist = path.split("/");
	QString tempelem = templist.takeLast();
	tempelem = "shadow" + tempelem;
	templist.append(tempelem);
	QString shadowpath = "";
	shadowpath = shadowpath + templist.at(0);
	for (int i = 1; i < templist.count(); ++i)
	{
		shadowpath = shadowpath +"/"+templist.at(i);
	}
	QFile *datafile = new QFile(path);
	QFile *stampfile = new QFile(shadowpath);
	
	if(!datafile->open(QIODevice::WriteOnly))
	{
		QMessageBox::critical(NULL,"Fehler","Fehler beim lesen der Daten-Datei");
	}
	if(!stampfile->open(QIODevice::WriteOnly))
	{
		QMessageBox::critical(NULL,"Fehler","Fehler beim lesen der Shadow-Datei");
	}


	QByteArray databyte;
	QByteArray stampbyte;
	
	QString data;
	QString stamp;

	for(int i = 0; i < container->vsp.count(); ++i)
	{
		data = container->vsp.at(i)->first;
		stamp = container->vsp.at(i)->second.toString("yyyy.MM.dd-hh:mm:ss:zzz");
		stamp = stamp+"\n";
		databyte.append(data.toUtf8());
		stampbyte.append(stamp.toUtf8());
	}
	datafile->flush();
	datafile->write(databyte);
	datafile->close();
	stampfile->flush();
	stampfile->write(stampbyte);
	stampfile->close();

}
