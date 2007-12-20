#ifndef BUNZIP_H
#define BUNZIP_H

 #include <QObject>

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
class Bunzip : public QObject {
Q_OBJECT

signals:
	void extractionFinished(QString filename);
	void errorOccured(QString);
	void extracting(QString filename);
	void progress(int);
	void canceled();

private:
	QProcess *proc;
	QString filename;

private slots:
	void readError();

public slots:
	void cancel();
	void extract(QString filename);
	void extractingFinishing(int exitCode);
public:
    Bunzip(QObject *parent=0);


};

#endif
