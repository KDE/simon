#ifndef SSCOBJECTS_EXPORT_H
#define SSCOBJECTS_EXPORT_H
 
// needed for KDE_EXPORT and KDE_IMPORT macros
#include <kdemacros.h>
 
#ifndef SSCOBJECTS_EXPORT
# if defined(MAKE_SSCOBJECTS_LIB)
   // We are building this library
#  define SSCOBJECTS_EXPORT KDE_EXPORT
# else
   // We are using this library
#  define SSCOBJECTS_EXPORT
# endif
#endif
 
# ifndef SSCOBJECTS_EXPORT_DEPRECATED
#  define SSCOBJECTS_EXPORT_DEPRECATED KDE_DEPRECATED SSCOBJECTS_EXPORT
# endif
 
#endif 
