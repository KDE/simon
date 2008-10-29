#ifndef SIMONLOGGING_EXPORT_H
#define SIMONLOGGING_EXPORT_H
 
// needed for KDE_EXPORT and KDE_IMPORT macros
#include <kdemacros.h>
 
#ifndef SIMONLOGGING_EXPORT
# if defined(MAKE_SIMONLOGGING_LIB)
   // We are building this library
#  define SIMONLOGGING_EXPORT KDE_EXPORT
# else
   // We are using this library
#  define SIMONLOGGING_EXPORT 
# endif
#endif
 
# ifndef SIMONLOGGING_EXPORT_DEPRECATED
#  define SIMONLOGGING_EXPORT_DEPRECATED KDE_DEPRECATED SIMONLOGGING_EXPORT
# endif
 
#endif
