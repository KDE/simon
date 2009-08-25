#ifndef SPEECHMODELBASE_EXPORT_H
#define SPEECHMODELBASE_EXPORT_H
 
// needed for KDE_EXPORT and KDE_IMPORT macros
#include <kdemacros.h>
 
#ifndef SPEECHMODELBASE_EXPORT
# if defined(MAKE_SPEECHMODELBASE_LIB)
   // We are building this library
#  define SPEECHMODELBASE_EXPORT KDE_EXPORT
# else
   // We are using this library
#  define SPEECHMODELBASE_EXPORT
# endif
#endif
 
# ifndef SPEECHMODELBASE_EXPORT_DEPRECATED
#  define SPEECHMODELBASE_EXPORT_DEPRECATED KDE_DEPRECATED SPEECHMODELBASE_EXPORT
# endif
 
#endif 
