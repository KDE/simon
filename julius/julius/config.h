/* config.h.  Generated automatically by configure.  */
/* Define if you use character conversion function */
#define CHARACTER_CONVERSION 1

#ifdef __WIN32
/* Define if you use Win32 character conversion API */
#undef USE_WIN32_MULTIBYTE
#else
/* Define if you have iconv function */
#define HAVE_ICONV 1
/* Define if you have const char prototype in iconv function */
#define ICONV_CONST 
#endif

/* Define if you use libjcode */
/* #undef USE_LIBJCODE */
