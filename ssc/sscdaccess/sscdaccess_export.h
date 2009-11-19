#ifndef SSCDACESS_EXPORT_H
#define SSCDACESS_EXPORT_H
 
// needed for KDE_EXPORT and KDE_IMPORT macros
#include <kdemacros.h>
 
#ifndef SSCDACESS_EXPORT
# if defined(MAKE_SSCDACESS_LIB)
   // We are building this library
#  define SSCDACESS_EXPORT KDE_EXPORT
# else
   // We are using this library
	//#ifdef Q_OS_WIN
	//#  define SSCDACESS_EXPORT
	//#else
	#  define SSCDACESS_EXPORT KDE_IMPORT
	//#endif
# endif
#endif
 
# ifndef SSCDACESS_EXPORT_DEPRECATED
#  define SSCDACESS_EXPORT_DEPRECATED KDE_DEPRECATED SSCDACESS_EXPORT
# endif
 
#endif
