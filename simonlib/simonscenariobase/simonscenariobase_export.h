#ifndef SCENARIOBASE_EXPORT_H
#define SCENARIOBASE_EXPORT_H
 
// needed for KDE_EXPORT and KDE_IMPORT macros
#include <kdemacros.h>
 
#ifndef SCENARIOBASE_EXPORT
# if defined(MAKE_SCENARIOBASE_LIB)
   // We are building this library
#  define SCENARIOBASE_EXPORT KDE_EXPORT
# else
   // We are using this library
#  define SCENARIOBASE_EXPORT
# endif
#endif
 
# ifndef SCENARIOBASE_EXPORT_DEPRECATED
#  define SCENARIOBASE_EXPORT_DEPRECATED KDE_DEPRECATED SCENARIOBASE_EXPORT
# endif
 
#endif 
