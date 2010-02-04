include(FindQt4)
include(SimonDirs)

find_package(KDE4 REQUIRED)
include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} ../ )

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include(KDE4Defaults)


set( simonvoiceui_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/voiceinterceptor.cpp
)

set( simonvoiceui_LIB_HDRS
	${CMAKE_CURRENT_SOURCE_DIR}/voiceinterceptor.h
	${CMAKE_CURRENT_SOURCE_DIR}/simonvoiceui_export.h
)

kde4_add_library( simonvoiceui  SHARED ${simonvoiceui_LIB_SRCS} )

target_link_libraries( simonvoiceui ${QT_LIBRARIES} ${QT_QTXML_LIBRARY} ${KDE4_KDEUI_LIBS} )


set_target_properties( simonvoiceui
  PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION}
)

install( FILES ${simonvoiceui_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/simonvoiceui
  COMPONENT simoncoredevel
)
 
install( TARGETS simonvoiceui DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT simoncore )
