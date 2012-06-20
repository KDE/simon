

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../../cmake ${CMAKE_MODULE_PATH})
include(SimonDirs)


include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/.. )

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include(KDE4Defaults)

set( simoncontextui_LIB_SRCS
        ${CMAKE_CURRENT_SOURCE_DIR}/contextview.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/contextviewprivate.cpp
)

set( simoncontextui_LIB_HDRS
        ${CMAKE_CURRENT_SOURCE_DIR}/contextview.h
        ${CMAKE_CURRENT_SOURCE_DIR}/simoncontextui_export.h
)

kde4_add_ui_files( simoncontextui_LIB_SRCS
        ${CMAKE_CURRENT_SOURCE_DIR}/contextdialog.ui
)

kde4_add_library( simoncontextui  SHARED ${simoncontextui_LIB_SRCS} )

target_link_libraries( simoncontextui ${QT_LIBRARIES}
        ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS} ${KDE4_KUTILS_LIBS} simonactions
        simonscenarios simonscenarios simonuicomponents simoncontextdetection simoncontextcoreui)

set_target_properties( simoncontextui
  PROPERTIES VERSION ${CMAKE_SIMON_VERSION_STRING} SOVERSION ${CMAKE_SIMON_VERSION_MAJOR}
)

install( FILES ${simoncontextui_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/simoncontextui
  COMPONENT simondevel
)

install( TARGETS simoncontextui DESTINATION ${SIMON_LIB_INSTALL_DIR} COMPONENT simon )
