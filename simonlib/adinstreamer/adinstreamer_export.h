#ifndef ADINSTREAMER_EXPORT_H
#define ADINSTREAMER_EXPORT_H
 
// needed for KDE_EXPORT and KDE_IMPORT macros
#include <kdemacros.h>
 
#ifndef ADINSTREAMER_EXPORT
# if defined(MAKE_ADINSTREAMER_LIB)
   // We are building this library
#  define ADINSTREAMER_EXPORT KDE_EXPORT
# else
   // We are using this library
#  define ADINSTREAMER_EXPORT 
# endif
#endif
 
# ifndef ADINSTREAMER_EXPORT_DEPRECATED
#  define ADINSTREAMER_EXPORT_DEPRECATED KDE_DEPRECATED ADINSTREAMER_EXPORT
# endif
 
#endif
