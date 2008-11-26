include(FindQt4)
find_package(KDE4 REQUIRED)
include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES}
			${CMAKE_CURRENT_SOURCE_DIR}/..
			${CMAKE_CURRENT_SOURCE_DIR}/../speechmodelbase
			${CMAKE_CURRENT_SOURCE_DIR}/../speechgui
)

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include(KDE4Defaults)

set( simonmodelmanagement_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/wordlistmanager.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/grammarmanager.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/trainingmanager.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/xmltrainingtext.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/modelmanager.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/speechmodelsettings.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/wordlistmodel.cpp
)


set( simonmodelmanagement_LIB_HDRS
  ${CMAKE_CURRENT_SOURCE_DIR}/simonacousticmodelmanagement_export.h
  ${CMAKE_CURRENT_SOURCE_DIR}/simonlanguagemodelmanagement_export.h
  ${CMAKE_CURRENT_SOURCE_DIR}/trainingmanager.h
  ${CMAKE_CURRENT_SOURCE_DIR}/wordlistmodel.h
  ${CMAKE_CURRENT_SOURCE_DIR}/wordlistmanager.h
)



kde4_add_ui_files( simonmodelmanagement_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/trainingsettings.ui
)

kde4_add_kcfg_files(simonmodelmanagement_LIB_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/speechmodelmanagementconfiguration.kcfgc
)

kde4_add_library( simonmodelmanagement SHARED ${simonmodelmanagement_LIB_SRCS} )
target_link_libraries( simonmodelmanagement ${QT_LIBRARIES} ${KDE4_KDECORE_LIBS} ${KDE4_KIO_LIBS} simonxml simonlogging speechgui speechmodelbase simonprogresstracking )


set_target_properties( simonmodelmanagement
  PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION}
)

install( FILES ${simonmodelmanagement_LIB_HDRS}
  DESTINATION ${INCLUDE_INSTALL_DIR}/simon/speechmodelmanagement
  COMPONENT Devel
)
 
install( TARGETS simonmodelmanagement  ${INSTALL_TARGETS_DEFAULT_ARGS} )
install( FILES simonspeechmodelmanagementconfig.desktop  DESTINATION  ${SERVICES_INSTALL_DIR} )
install( FILES ${CMAKE_CURRENT_SOURCE_DIR}/speechmodelmanagementconfiguration.kcfg DESTINATION  ${KCFG_INSTALL_DIR} )

	
