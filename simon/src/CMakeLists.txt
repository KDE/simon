include(FindQt4)

set (QT_USE_QTMAIN true)
set (QT_USE_QTSVG true)

include(${QT_USE_FILE})


include_directories( ${KDE4_INCLUDE_DIRS} ${KDE4_INCLUDE_DIR} ${KDE4_INCLUDE_DIR}/KDE 
		     ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/../../simonlib
		     ${CMAKE_CURRENT_SOURCE_DIR}/../../simonlib/speechgui
		     ${CMAKE_CURRENT_SOURCE_DIR}/simonappcore
		     ${CMAKE_CURRENT_SOURCE_DIR}/simonactionsui
		     ${CMAKE_CURRENT_SOURCE_DIR}/simonmodelmanagementui)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/simonactionsui)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/simonappcore)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/simonmodelmanagementui)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/simonrecognitioncontrol)

set (	simonGeneralSources
	${CMAKE_CURRENT_SOURCE_DIR}/simonview.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

kde4_add_ui_files( simonGeneralSources
	${CMAKE_CURRENT_SOURCE_DIR}/simonview.ui
)


install( FILES ${simon_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon
  COMPONENT Devel
)


kde4_add_app_icon(simonGeneralSources "${CMAKE_CURRENT_SOURCE_DIR}/../icons/hi*-app-simon.png")

kde4_add_executable( simon ${simonGeneralSources} )

target_link_libraries( simon ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS}
${QT_LIBRARIES} ${KDE4_KUTILS_LIBS} simonappcore simoninfo
simonlogging simonxml speechgui simonsound simonmodelmanagementui 
simonrecognitioncontrol simonactionsui simonprogresstracking)

install (TARGETS simon DESTINATION ${INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES simonui.rc simon.log DESTINATION ${DATA_INSTALL_DIR})
