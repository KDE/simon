ENABLE_TESTING()

find_package(Qt4 COMPONENTS QtGui QtCore QtXml QtScript REQUIRED)
include (${QT_USE_FILE})

find_package(KDE4 REQUIRED)
include(KDE4Defaults)
include(KDE4Macros)
include(SimonDirs)

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})

set(USE_PLASMA true CACHE BOOL "Use the plasma data engines in the dialog plugin")

if (USE_PLASMA)
add_definitions(-DUSE_PLASMA)
endif (USE_PLASMA)

include_directories(${KDE4_INCLUDE_DIRS} ${KDE4_INCLUDE_DIR}
${KDE4_INCLUDE_DIR}/KDE ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/..)

set (simondialogengine_SRCS 
  dialogcommand.cpp
  dialogstate.cpp
  dialogtext.cpp
  dialogtextparser.cpp
  dialogtemplateoptions.cpp
  boundvalue.cpp
  dialogboundvalues.cpp
  staticboundvalue.cpp
  scriptboundvalue.cpp
  avatar.cpp
  avatarmodel.cpp
  dialogmanager.cpp
  dialogview.cpp
    )

set( simondialogengine_LIB_HDRS
  dialogcommand.h
  dialogstate.h
  dialogtext.h
  dialogtextparser.h
  dialogtemplateoptions.h
  boundvalue.h
  dialogboundvalues.h
  staticboundvalue.h
  scriptboundvalue.h
  avatar.h
  avatarmodel.h
  dialogmanager.h
  simondialogengine_export.h
)

if (USE_PLASMA)
set (simondialogengine_SRCS 
  ${simondialogengine_SRCS}
  plasmaboundvalue.cpp
  )
endif(USE_PLASMA)

kde4_add_library(simondialogengine SHARED ${simondialogengine_SRCS})

if (USE_PLASMA)
target_link_libraries(simondialogengine 
  ${QT_LIBRARIES} ${KDE4_KIO_LIBS} ${KDE4_PLASMA_LIBS} simonscenarios
  simonscenariobase simonactions)
else (USE_PLASMA)
target_link_libraries(simondialogengine 
  ${QT_LIBRARIES} ${KDE4_KIO_LIBS} simonscenarios simonscenariobase
  simonactions)
endif(USE_PLASMA)

install(TARGETS simondialogengine DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT simoncore )


install( FILES ${simondialogengine_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/simondialogengine
  COMPONENT simondevel
)

add_subdirectory(graphicalview)
add_subdirectory(ttsview)
add_subdirectory(confui)

########
# Tests
########

add_subdirectory(tests)
