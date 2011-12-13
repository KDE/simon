
include(SimonDirs)


include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} ../../
                        ${CMAKE_CURRENT_SOURCE_DIR}/..
                        ${CMAKE_CURRENT_SOURCE_DIR}/../)

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include(KDE4Defaults)

set (QT_USE_QTMAIN true)
 
set( simoncontextdetection_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/contextmanager.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/condition.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/conditionassociation.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/compoundcondition.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/compoundconditionmodel.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/createconditionwidget.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/processinfo.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/processinfogatherer.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/linuxprocessinfogatherer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/linuxprocessinfogathererhelper.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/windowsprocessinfogatherer.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/samplegroupconditionmodel.cpp
)

set( simoncontextdetection_LIB_HDRS
  simoncontextdetection_export.h
  contextmanager.h
  condition.h
  conditionassociation.h
  compoundcondition.h
  compoundconditionmodel.h
  createconditionwidget.h
  processinfo.h
  processinfogatherer.h
  linuxprocessinfogatherer.h
  linuxprocessinfogathererhelper.h
  windowsprocessinfogatherer.h
  samplegroupconditionmodel.h
)

kde4_add_library( simoncontextdetection  SHARED ${simoncontextdetection_LIB_SRCS} )

target_link_libraries( simoncontextdetection ${QT_LIBRARIES} ${KDE4_KDECORE_LIBS}
        ${QT_QTXML_LIBRARY} ${KDE4_KDEUI_LIBS})


set_target_properties( simoncontextdetection
  PROPERTIES VERSION ${CMAKE_SIMON_VERSION_STRING} SOVERSION ${CMAKE_SIMON_VERSION_MAJOR}
)

install( FILES ${simoncontextdetection_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/contextdetection
  COMPONENT simoncoredevel
)
 
install( TARGETS simoncontextdetection DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT simoncore )

install(FILES
${CMAKE_CURRENT_SOURCE_DIR}/simonconditionplugin.desktop DESTINATION
${SERVICETYPES_INSTALL_DIR} COMPONENT simoncore)
