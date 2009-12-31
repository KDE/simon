#ifndef SSCCONFIGURATION_EXPORT_H
#define SSCCONFIGURATION_EXPORT_H
 
// needed for KDE_EXPORT and KDE_IMPORT macros
#include <kdemacros.h>
 
#ifndef SSCCONFIGURATION_EXPORT
# if defined(MAKE_SSCCONFIGURATION_LIB)
   // We are building this library
#  define SSCCONFIGURATION_EXPORT KDE_EXPORT
# else
   // We are using this library
	#ifdef Q_OS_WIN
	#  define SSCCONFIGURATION_EXPORT
	#else
	#  define SSCCONFIGURATION_EXPORT KDE_IMPORT
	#endif
# endif
#endif
 
# ifndef SSCCONFIGURATION_EXPORT_DEPRECATED
#  define SSCCONFIGURATION_EXPORT_DEPRECATED KDE_DEPRECATED SSCCONFIGURATION_EXPORT
# endif
 
#endif
