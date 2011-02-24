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

set (simondialogenginegraphicalview_SRCS 
  visualdialogview.cpp
  dialogcommandbutton.cpp
    )

set( simondialogenginegraphicalview_LIB_HDRS
  visualdialogview.h
  simondialogenginevisualview_export.h
)

kde4_add_ui_files(simondialogenginegraphicalview_SRCS 
	dialogwidget.ui
)

kde4_add_library(simondialogenginegraphicalview SHARED ${simondialogenginegraphicalview_SRCS})

target_link_libraries(simondialogenginegraphicalview 
  ${QT_LIBRARIES} ${KDE4_KIO_LIBS} simonactions simondialogengine)

install(TARGETS simondialogenginegraphicalview DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT simoncore )

install( FILES ${simondialogenginegraphicalview_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/simondialogengine/graphicalview
  COMPONENT simondevel
)
