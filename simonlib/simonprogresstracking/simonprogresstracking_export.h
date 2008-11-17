#ifndef SIMONPROGRESSTRACKING_EXPORT_H
#define SIMONPROGRESSTRACKING_EXPORT_H
 
// needed for KDE_EXPORT and KDE_IMPORT macros
#include <kdemacros.h>
 
#ifndef SIMONPROGRESSTRACKING_EXPORT
# if defined(MAKE_SIMONPROGRESSTRACKING_LIB)
   // We are building this library
#  define SIMONPROGRESSTRACKING_EXPORT KDE_EXPORT
# else
   // We are using this library
#  define SIMONPROGRESSTRACKING_EXPORT 
# endif
#endif
 
# ifndef SIMONPROGRESSTRACKING_EXPORT_DEPRECATED
#  define SIMONPROGRESSTRACKING_EXPORT_DEPRECATED KDE_DEPRECATED SIMONPROGRESSTRACKING_EXPORT
# endif
 
#endif