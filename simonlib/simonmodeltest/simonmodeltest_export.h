#ifndef SIMON_MODELTEST_EXPORT_H_4E3834CE9F11405A8CDD7F9BF76585E7
#define SIMON_MODELTEST_EXPORT_H_4E3834CE9F11405A8CDD7F9BF76585E7
 
// needed for KDE_EXPORT and KDE_IMPORT macros
#include <kdemacros.h>
 
#ifndef MODELTEST_EXPORT
# if defined(MAKE_MODELTEST_LIB)
   // We are building this library
#  define MODELTEST_EXPORT KDE_EXPORT
# else
   // We are using this library
	#ifdef Q_OS_WIN
	#  define MODELTEST_EXPORT
	#else
	#  define MODELTEST_EXPORT KDE_IMPORT
	#endif
# endif
#endif
 
# ifndef MODELTEST_EXPORT_DEPRECATED
#  define MODELTEST_EXPORT_DEPRECATED KDE_DEPRECATED MODELTEST_EXPORT
# endif
 
#endif
