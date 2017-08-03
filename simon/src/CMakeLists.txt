include_directories(.)

add_subdirectory(simonactionsui)
add_subdirectory(simoncontextui)
add_subdirectory(simonappcore)
add_subdirectory(simonmodelmanagementui)
add_subdirectory(simonrecognitioncontrol)
add_subdirectory(simonsampleshare)

set(simonGeneralSources
  simonview.cpp
  firstrunwizard.cpp
  firstrunsimondconfig.cpp
  firstrunscenariosconfig.cpp
  firstrunsoundconfig.cpp
  firstrunbasemodelconfig.cpp
  welcomepage.cpp
  trainingtextaggregatormodel.cpp
  main.cpp
)

ki18n_wrap_ui(simonGeneralSources
  simonview.ui
  firstrunbasemodelconfig.ui
  firstrunsimondconfig.ui
  firstrunsoundconfig.ui
  firstrunscenariosconfig.ui
  welcomepage.ui
)

file(GLOB ICONS_SRCS "../icons/hi*-app-simon.png")
kde4_add_app_icon(simonGeneralSources ICONS ${ICONS_SRCS})
add_executable( simon ${simonGeneralSources} )

if(UNIX)
target_link_libraries(simon  KF5::KIOCore KF5::KDELibs4Support
  ${QT_LIBRARIES}  KF5::NewStuff KF5::KHtml KF5::KCMUtils
  ${X11_X11_LIB} simonappcore simonactions simoninfo
  simonlogging simonxml simonuicomponents simonsound simonscenarios 
  simonmodelmanagementui simonscenarioui
  simonrecognitioncontrol simonactionsui simonprogresstracking
  simoncontextui simonsampleshare)
else()
target_link_libraries(simon  KF5::KIOCore KF5::KDELibs4Support
  ${QT_LIBRARIES} Qt5::WebKitWidgets KF5::KHtml 
   KF5::NewStuff KF5::KCMUtils
  simonappcore simonactions simoninfo
  simonlogging simonxml simonuicomponents simonsound simonscenarios 
  simonmodelmanagementui simonscenarioui
  simonrecognitioncontrol simonactionsui simonprogresstracking
  simoncontextui simonsampleshare)
endif()

install (TARGETS simon ${INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES simonui.rc DESTINATION ${DATA_INSTALL_DIR} COMPONENT simon)
