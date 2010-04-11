find_package(KDE4 REQUIRED)

add_definitions(-DUSE_WITH_SIMON)
set (USE_WITH_SIMON "1")

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/po)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/adinstreamer)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/eventsimulation)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/simonddatabaseaccess)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/simondstreamer)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/simonactions)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/simonlogging)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/simoninfo)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/simonxml)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/simonuicomponents)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/simonmodeltest)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/simonsound)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/simonprotocol)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/sscobjects)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/sscprotocol)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/simonrecognitionresult)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/speechmodelcompilationadapter)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/speechmodelcompilation)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/simonprogresstracking)

#install( FILES settings-simon.desktop  DESTINATION  ${SERVICES_INSTALL_DIR}  COMPONENT simon)
install( FILES settings-simond.desktop  DESTINATION  ${SERVICES_INSTALL_DIR} COMPONENT simond )



message(STATUS "Enabling simon scenario support.")
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/simonscenariobase)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/simonscenarios)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/simonscenarioui)

