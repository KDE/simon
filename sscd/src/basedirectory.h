#ifndef SSCD_BASE_DIRECTORY_H
#define SSCD_BASE_DIRECTORY_H

#ifdef Q_OS_WIN32
#define SSCD_BASE_DIRECTORY (qApp()->applicationDirPath())
#else
#define SSCD_BASE_DIRECTORY QString("/usr/share/sscd")
#endif

#endif

