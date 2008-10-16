#ifndef SIMONACTIONS_EXPORT_H
#define SIMONACTIONS_EXPORT_H
 
// needed for KDE_EXPORT and KDE_IMPORT macros
#include <kdemacros.h>
 
#ifndef SIMONACTIONS_EXPORT
# if defined(MAKE_SIMONACTIONS_LIB)
   // We are building this library
#  define SIMONACTIONS_EXPORT KDE_EXPORT
# else
   // We are using this library
#  define SIMONACTIONS_EXPORT KDE_IMPORT
# endif
#endif
 
# ifndef SIMONACTIONS_EXPORT_DEPRECATED
#  define SIMONACTIONS_EXPORT_DEPRECATED KDE_DEPRECATED SIMONACTIONS_EXPORT
# endif
 
#endif