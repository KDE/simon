#ifndef SIMONAPPCORE_EXPORT_H
#define SIMONAPPCORE_EXPORT_H
 
// needed for KDE_EXPORT and KDE_IMPORT macros
#include <kdemacros.h>
 
#ifndef SIMONAPPCORE_EXPORT
# if defined(MAKE_SIMONAPPCORE_LIB)
   // We are building this library
#  define SIMONAPPCORE_EXPORT KDE_EXPORT
# else
   // We are using this library
#  define SIMONAPPCORE_EXPORT KDE_IMPORT
# endif
#endif
 
# ifndef SIMONAPPCORE_EXPORT_DEPRECATED
#  define SIMONAPPCORE_EXPORT_DEPRECATED KDE_DEPRECATED SIMONAPPCORE_EXPORT
# endif
 
#endif