set(simonutils_LIB_SRCS fileutils.cpp)
set(simonutils_LIB_HDRS simonutils_export.h fileutils.h)

kde4_add_library(simonutils SHARED ${simonutils_LIB_SRCS})
target_link_libraries(simonutils ${QT_LIBRARIES} ${KDE4_KDECORE_LIBS}
                                 ${KDE4_KIO_LIBS})

set_target_properties(simonutils
  PROPERTIES VERSION ${CMAKE_SIMON_VERSION_STRING} SOVERSION ${CMAKE_SIMON_VERSION_MAJOR}
)

install(FILES ${simonutils_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/simonutils
  COMPONENT simondevel
)
 
install(TARGETS simonutils DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT simoncore)
