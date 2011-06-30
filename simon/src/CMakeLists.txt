find_package(KDE4 REQUIRED)
include(FindQt4)

set (QT_USE_QTMAIN true)
set (QT_USE_QTSVG true)

include(${QT_USE_FILE})


include_directories( ${KDE4_INCLUDE_DIRS} ${KDE4_INCLUDE_DIR} ${KDE4_INCLUDE_DIR}/KDE 
		     ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR}/../../simonlib
		     ${CMAKE_CURRENT_SOURCE_DIR}/simonappcore
		     ${CMAKE_CURRENT_SOURCE_DIR}/simonactionsui
		     ${CMAKE_CURRENT_SOURCE_DIR}/simonmodelmanagementui)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/simonactionsui)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/simonappcore)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/simonmodelmanagementui)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/simonrecognitioncontrol)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/about)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/simonsampleshare)

set (	simonGeneralSources
	${CMAKE_CURRENT_SOURCE_DIR}/simonview.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/firstrunwizard.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/firstrunsimondconfig.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/firstrunscenariosconfig.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/firstrunsoundconfig.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/firstrunbasemodelconfig.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

kde4_add_ui_files( simonGeneralSources
	${CMAKE_CURRENT_SOURCE_DIR}/simonview.ui
	${CMAKE_CURRENT_SOURCE_DIR}/firstrunbasemodelconfig.ui
	${CMAKE_CURRENT_SOURCE_DIR}/firstrunsimondconfig.ui
	${CMAKE_CURRENT_SOURCE_DIR}/firstrunsoundconfig.ui
	${CMAKE_CURRENT_SOURCE_DIR}/firstrunscenariosconfig.ui
)

kde4_add_app_icon(simonGeneralSources "${CMAKE_CURRENT_SOURCE_DIR}/../icons/hi*-app-simon.png")

kde4_add_executable( simon ${simonGeneralSources} )

if(UNIX)
	FIND_LIBRARY(X11_X11_LIB X11 ${X11_LIB_SEARCH_PATH})
target_link_libraries( simon ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS}
${QT_LIBRARIES} ${KDE4_KHTML_LIBS} ${KDE4_KPARTS_LIBS} 
${KDE4_KUTILS_LIBS} ${KDE4_KNEWSTUFF3_LIBS} ${X11_X11_LIB}
simonappcore simonactions simoninfo
simonlogging simonxml simonuicomponents simonsound simonscenarios 
simonmodelmanagementui simonscenariobase simonscenarioui
simonrecognitioncontrol simonactionsui simonprogresstracking simonsampleshare)
else(UNIX)
target_link_libraries( simon ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS}
${QT_LIBRARIES} ${KDE4_KHTML_LIBS} ${KDE4_KPARTS_LIBS} 
${KDE4_KUTILS_LIBS} ${KDE4_KNEWSTUFF3_LIBS}
simonappcore simonactions simoninfo
simonlogging simonxml simonuicomponents simonsound simonscenarios 
simonmodelmanagementui simonscenariobase simonscenarioui
simonrecognitioncontrol simonactionsui simonprogresstracking)
endif(UNIX)

install (TARGETS simon DESTINATION bin COMPONENT simon) #${BIN_INSTALL_DIR} COMPONENT simon)
install(FILES simonui.rc simon.log DESTINATION ${DATA_INSTALL_DIR} COMPONENT simon)
