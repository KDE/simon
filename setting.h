#ifndef SETTING_H
#define SETTING_H
#include <QString>
#include <QIODevice>
#include <QStringList>

class Setting
{
private:
	QString name;
	QString value;
	QString hardware;
public:
	Setting(QString name, QString value, QString hardware)
	{
		this->name=name;
		this->value=value;
		this->hardware=hardware;
	}
	
	QString getName()
	{
		return this->name;
	}
	
	QString getValue()
	{
		return this->value;
	}
	
	QString getHardware()
	{
		return this->hardware;
	}
	
	~Setting()
	{
	}
	
};

#endif