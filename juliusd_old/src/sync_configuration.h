#ifndef _CONFIGURATION_H_
#define _CONFIGURATION_H_

#include <QtNetwork>

class Configuration
{

public:
	Configuration(int type)
	:_type(type)
	{

	}

	Configuration(const Configuration *conf) 
	{
		_type = conf->_type;
	}
	Configuration&
	operator = (const Configuration *conf)
	{
		this->_type = conf->_type;
		return *this;
	}

	virtual int getType(){return this->_type;}

	virtual ~Configuration() {}

private:
	int _type;
};



#endif // _CONFIGURATION_H_
