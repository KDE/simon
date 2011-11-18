find_package(Qt4 COMPONENTS QtGui REQUIRED)
include (${QT_USE_FILE})

find_package(KDE4 REQUIRED)
include(KDE4Defaults)
include(KDE4Macros)
include(SimonDirs)

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})

include_directories(${KDE4_INCLUDE_DIRS} ${KDE4_INCLUDE_DIR}
${KDE4_INCLUDE_DIR}/KDE ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/..
${CMAKE_CURRENT_SOURCE_DIR}/../..)


set(USE_PLASMA true CACHE BOOL "Use the plasma data engines in the dialog plugin")

if (USE_PLASMA)
add_definitions(-DUSE_PLASMA)
endif (USE_PLASMA)


set (simondialogengineconfui_SRCS 
  createtemplateoptiondialog.cpp
  createboundvaluedialog.cpp 
  createavatardialog.cpp
  avatarconfiguration.cpp
  boundvaluesconfiguration.cpp
  templateoptionsconfiguration.cpp
  outputconfiguration.cpp
    )

set( simondialogengineconfui_LIB_HDRS
  createtemplateoptiondialog.h
  createboundvaluedialog.h
  createavatardialog.h
  avatarconfiguration.h
  boundvaluesconfiguration.h
  templateoptionsconfiguration.h
  outputconfiguration.h
  simondialogengineconfui_export.h
)

kde4_add_ui_files(simondialogengineconfui_SRCS 
  createtemplateoptiondialog.ui
  createboundvalue.ui
  createavatardlg.ui
  avatarconfiguration.ui
  boundvaluesconfiguration.ui
  outputconfiguration.ui
  templateoptionsconfiguration.ui
)

kde4_add_library(simondialogengineconfui SHARED ${simondialogengineconfui_SRCS})
set_target_properties( simondialogengineconfui
  PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION}
)

if (USE_PLASMA)
target_link_libraries(simondialogengineconfui 
  ${QT_LIBRARIES} ${KDE4_KIO_LIBS} ${KDE4_PLASMA_LIBS} simondialogengine)
else (USE_PLASMA)
target_link_libraries(simondialogengineconfui 
  ${QT_LIBRARIES} ${KDE4_KIO_LIBS} simondialogengine)
endif(USE_PLASMA)

install(TARGETS simondialogengineconfui DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT simoncore )

install( FILES ${simondialogengineconfui_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/simondialogengine/confui
  COMPONENT simondevel
)
