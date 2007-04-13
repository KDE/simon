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
public:
	Setting(QString name, QString value, QString hardware)
	{
		this->name=name;
		this->value=value;
	}
	
	QString getName()
	{
		return this->name;
	}
	
	QString getValue()
	{
		return this->value;
	}
	
	~Setting()
	{
	}
	
};

#endif
