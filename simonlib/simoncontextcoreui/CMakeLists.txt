include(SimonDirs)
include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/.. )

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include(KDE4Defaults)

set( simoncontextcoreui_LIB_SRCS
        ${CMAKE_CURRENT_SOURCE_DIR}/newcondition.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/compoundconditionsettings.cpp
)

set( simoncontextcoreui_LIB_HDRS
        ${CMAKE_CURRENT_SOURCE_DIR}/newcondition.h
        ${CMAKE_CURRENT_SOURCE_DIR}/compoundconditionsettings.h
        ${CMAKE_CURRENT_SOURCE_DIR}/simoncontextcoreui_export.h
)

kde4_add_ui_files( simoncontextcoreui_LIB_SRCS
        ${CMAKE_CURRENT_SOURCE_DIR}/modifycondition.ui
        ${CMAKE_CURRENT_SOURCE_DIR}/compoundconditionsettings.ui
)

kde4_add_library( simoncontextcoreui  SHARED ${simoncontextcoreui_LIB_SRCS} )

target_link_libraries( simoncontextcoreui ${QT_LIBRARIES}
        ${KDE4_KDEUI_LIBS} simoncontextdetection)

set_target_properties( simoncontextcoreui
  PROPERTIES VERSION ${CMAKE_SIMON_VERSION_STRING} SOVERSION ${CMAKE_SIMON_VERSION_MAJOR}
)

install( FILES ${simoncontextcoreui_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/simoncontextcoreui
  COMPONENT simondevel
)

install( TARGETS simoncontextcoreui DESTINATION ${SIMON_LIB_INSTALL_DIR}
          COMPONENT simoncore )
