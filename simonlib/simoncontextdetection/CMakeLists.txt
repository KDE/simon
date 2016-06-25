set( simoncontextdetection_LIB_SRCS
  contextmanager.cpp
  condition.cpp
  conditionassociation.cpp
  compoundcondition.cpp
  createconditionwidget.cpp
  processinfo.cpp
  processinfogatherer.cpp
  samplegroupcondition.cpp
)

set( simoncontextdetection_LIB_HDRS
  ${CMAKE_CURRENT_BINARY_DIR}/simoncontextdetection_export.h
  contextmanager.h
  condition.h
  conditionassociation.h
  compoundcondition.h
  createconditionwidget.h
  processinfo.h
  processinfogatherer.h
  samplegroupcondition.h
)

IF(WIN32)
  set(simoncontextdetection_LIB_SRCS ${simoncontextdetection_LIB_SRCS} windowsprocessinfogatherer.cpp)
ENDIF()
IF(UNIX AND NOT WIN32 AND NOT APPLE)
  set(simoncontextdetection_LIB_SRCS ${simoncontextdetection_LIB_SRCS} linuxprocessinfogatherer.cpp linuxprocessinfogathererhelper.cpp)
ENDIF()
IF(APPLE)
  set(CMAKE_SHARED_LINKER_FLAGS "-framework Foundation -framework AppKit -framework ScriptingBridge")
  set(simoncontextdetection_LIB_SRCS ${simoncontextdetection_LIB_SRCS} macprocessinfogatherer.cpp macprocessinfogathererhelper.mm)
ENDIF()

kde4_add_library(simoncontextdetection SHARED ${simoncontextdetection_LIB_SRCS})

target_link_libraries(simoncontextdetection ${QT_LIBRARIES} KF5::KDELibs4Support Qt5::Xml Qt5::X11Extras )

generate_export_header(simoncontextdetection)

if(WIN32)
  target_link_libraries(simoncontextdetection psapi)
endif(WIN32)

set_target_properties(simoncontextdetection
  PROPERTIES VERSION ${CMAKE_SIMON_VERSION_STRING} SOVERSION ${CMAKE_SIMON_VERSION_MAJOR}
)

install(FILES ${simoncontextdetection_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/contextdetection
  COMPONENT simoncoredevel
)
 
install(TARGETS simoncontextdetection DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT simoncore)
install(FILES simonconditionplugin.desktop DESTINATION ${SERVICETYPES_INSTALL_DIR} COMPONENT simoncore)

# add_subdirectory(test)
