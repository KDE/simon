#ifndef BUNZIP_H
#define BUNZIP_H

 #include "compression.h"

class QString;
class QProcess;

/**
 * \class Bunzip
 * \brief Extracts *.bz2 / *.bzip2 files
 * \author Peter Grasch
 * \note This uses the programm bzip2 which might not be installed
 * \version 0.1
 * \date 10.08.2007
 */
class Bunzip : public Compression {
Q_OBJECT
private:
	QProcess *proc;
	QString filename;

private slots:
	void readError();
	void extractingFinishing(int exitCode);

public slots:
	void cancel();
	void extract(QString filename);
public:
    Bunzip(QObject *parent=0);
    ~Bunzip();


};

#endif
