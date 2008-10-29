#ifndef SIMONINFO_EXPORT_H
#define SIMONINFO_EXPORT_H
 
// needed for KDE_EXPORT and KDE_IMPORT macros
#include <kdemacros.h>
 
#ifndef SIMONINFO_EXPORT
# if defined(MAKE_SIMONINFO_LIB)
   // We are building this library
#  define SIMONINFO_EXPORT KDE_EXPORT
# else
   // We are using this library
#  define SIMONINFO_EXPORT 
# endif
#endif
 
# ifndef SIMONINFO_EXPORT_DEPRECATED
#  define SIMONINFO_EXPORT_DEPRECATED KDE_DEPRECATED SIMONINFO_EXPORT
# endif
 
#endif
