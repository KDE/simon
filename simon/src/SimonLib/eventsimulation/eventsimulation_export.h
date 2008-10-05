#ifndef EVENTSIMULATION_EXPORT_H
#define EVENTSIMULATION_EXPORT_H
 
// needed for KDE_EXPORT and KDE_IMPORT macros
#include <kdemacros.h>
 
#ifndef EVENTSIMULATION_EXPORT
# if defined(MAKE_EVENTSIMULATION_LIB)
   // We are building this library
#  define EVENTSIMULATION_EXPORT KDE_EXPORT
# else
   // We are using this library
#  define EVENTSIMULATION_EXPORT KDE_IMPORT
# endif
#endif
 
# ifndef EVENTSIMULATION_EXPORT_DEPRECATED
#  define EVENTSIMULATION_EXPORT_DEPRECATED KDE_DEPRECATED EVENTSIMULATION_EXPORT
# endif
 
#endif