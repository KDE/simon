#ifndef SIMON_SSCD_BASE_DIRECTORY_H_C43F1E0DA7CC438A8A7E90E43CFDCFEA
#define SIMON_SSCD_BASE_DIRECTORY_H_C43F1E0DA7CC438A8A7E90E43CFDCFEA
#include <QCoreApplication>

#ifdef Q_OS_WIN32
#define SSCD_BASE_DIRECTORY (QCoreApplication::instance()->applicationDirPath())
#else
#define SSCD_BASE_DIRECTORY QString("/usr/share/sscd")
#endif
#endif
