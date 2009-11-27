#ifndef SSCD_BASE_DIRECTORY_H
#define SSCD_BASE_DIRECTORY_H
#include <QCoreApplication>

#ifdef Q_OS_WIN32
#define SSCD_BASE_DIRECTORY (QCoreApplication::instance()->applicationDirPath())
#else
#define SSCD_BASE_DIRECTORY QString("/usr/share/sscd")
#endif

#endif

