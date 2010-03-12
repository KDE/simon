#ifndef SIMON_MODELMANAGEMENT_EXPORT_H_0791F558C23544B3B7F419C871F8222A
#define SIMON_MODELMANAGEMENT_EXPORT_H_0791F558C23544B3B7F419C871F8222A
 
// needed for KDE_EXPORT and KDE_IMPORT macros
#include <kdemacros.h>
#include <KGenericFactory>
 
#ifndef MODELMANAGEMENT_EXPORT
# if defined(MAKE_MODELMANAGEMENT_LIB)
   // We are building this library
#  define MODELMANAGEMENT_EXPORT KDE_EXPORT
# else
   // We are using this library
	//#ifdef Q_OS_WIN
	//#  define MODELMANAGEMENT_EXPORT
	//#else
	#  define MODELMANAGEMENT_EXPORT KDE_IMPORT
	//#endif
# endif
#endif
 
# ifndef MODELMANAGEMENT_EXPORT_DEPRECATED
#  define MODELMANAGEMENT_EXPORT_DEPRECATED KDE_DEPRECATED MODELMANAGEMENT_EXPORT
# endif
 
#endif
