#ifndef SPEECHGUI_EXPORT_H
#define SPEECHGUI_EXPORT_H
 
// needed for KDE_EXPORT and KDE_IMPORT macros
#include <kdemacros.h>
 
#ifndef SPEECHGUI_EXPORT
# if defined(MAKE_SPEECHGUI_LIB)
   // We are building this library
#  define SPEECHGUI_EXPORT KDE_EXPORT
# else
   // We are using this library
#  define SPEECHGUI_EXPORT KDE_IMPORT
# endif
#endif
 
# ifndef SPEECHGUI_EXPORT_DEPRECATED
#  define SPEECHGUI_EXPORT_DEPRECATED KDE_DEPRECATED SPEECHGUI_EXPORT
# endif
 
#endif
