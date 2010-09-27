include(FindQt4)
include(SimonDirs)

find_package(Qt4 COMPONENTS QtCore QtXml REQUIRED)
include(${QT_USE_FILE})
find_package(KDE4 REQUIRED)
include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} )

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include(KDE4Defaults)

set (QT_USE_QTMAIN true)
 
set( simontts_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/simontts.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/recordingset.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/recordingsetcollection.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/simonttsprivate.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/joviettsprovider.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/recordedttsprovider.cpp
)

set( simontts_LIB_HDRS
  simontts_export.h
  simontts.h
)

kde4_add_kcfg_files(simontts_LIB_SRCS config/ttsconfiguration.kcfgc)

kde4_add_library( simontts  SHARED ${simontts_LIB_SRCS} )
target_link_libraries( simontts ${QT_LIBRARIES} ${KDE4_KDECORE_LIBS}  ${KDE4_KIO_LIBS})


set_target_properties( simontts
  PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION}
)

install( FILES ${simontts_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/tts
  COMPONENT simoncoredevel
)
 
install( TARGETS simontts DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT simoncore )

install( FILES ttssets.xml DESTINATION ${DATA_INSTALL_DIR}/simon/ttsrec
  COMPONENT simoncore )

add_subdirectory(config)
