#ifndef SIMON_SIMONWAV_EXPORT_H_DAA46318B9EE4B9888F8F8C2CDDFD6DC
#define SIMON_SIMONWAV_EXPORT_H_DAA46318B9EE4B9888F8F8C2CDDFD6DC
 
// needed for KDE_EXPORT and KDE_IMPORT macros
#include <kdemacros.h>
 
#ifndef SIMONWAV_EXPORT
# if defined(MAKE_SIMONWAV_LIB)
   // We are building this library
#  define SIMONWAV_EXPORT KDE_EXPORT
# else
   // We are using this library
#  define SIMONWAV_EXPORT
# endif
#endif
 
# ifndef SIMONWAV_EXPORT_DEPRECATED
#  define SIMONWAV_EXPORT_DEPRECATED KDE_DEPRECATED SIMONWAV_EXPORT
# endif
 
#endif
