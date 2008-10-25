#ifndef SIMONACTIONSUI_EXPORT_H
#define SIMONACTIONSUI_EXPORT_H
 
// needed for KDE_EXPORT and KDE_IMPORT macros
#include <kdemacros.h>
 
#ifndef SIMONACTIONSUI_EXPORT
# if defined(MAKE_SIMONACTIONSUI_LIB)
   // We are building this library
#  define SIMONACTIONSUI_EXPORT KDE_EXPORT
# else
   // We are using this library
#  define SIMONACTIONSUI_EXPORT KDE_IMPORT
# endif
#endif
 
# ifndef SIMONACTIONSUI_EXPORT_DEPRECATED
#  define SIMONACTIONSUI_EXPORT_DEPRECATED KDE_DEPRECATED SIMONACTIONSUI_EXPORT
# endif
 
#endif