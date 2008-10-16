#ifndef SIMONSOUND_EXPORT_H
#define SIMONSOUND_EXPORT_H
 
// needed for KDE_EXPORT and KDE_IMPORT macros
#include <kdemacros.h>
 
#ifndef SIMONSOUND_EXPORT
# if defined(MAKE_SIMONSOUND_LIB)
   // We are building this library
#  define SIMONSOUND_EXPORT KDE_EXPORT
# else
   // We are using this library
#  define SIMONSOUND_EXPORT KDE_IMPORT
# endif
#endif
 
# ifndef SIMONSOUND_EXPORT_DEPRECATED
#  define SIMONSOUND_EXPORT_DEPRECATED KDE_DEPRECATED SIMONSOUND_EXPORT
# endif
 
#endif