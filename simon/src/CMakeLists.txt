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

kde4_add_ui_files(simonGeneralSources
  simonview.ui
  firstrunbasemodelconfig.ui
  firstrunsimondconfig.ui
  firstrunsoundconfig.ui
  firstrunscenariosconfig.ui
  welcomepage.ui
)

kde4_add_app_icon(simonGeneralSources "../icons/hi*-app-simon.png")
kde4_add_executable( simon ${simonGeneralSources} )

if(UNIX)
target_link_libraries(simon ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS}
  ${QT_LIBRARIES} ${KDE4_KUTILS_LIBS} ${KDE4_KNEWSTUFF3_LIBS}
  ${X11_X11_LIB} simonappcore simonactions simoninfo
  simonlogging simonxml simonuicomponents simonsound simonscenarios 
  simonmodelmanagementui simonscenariobase simonscenarioui
  simonrecognitioncontrol simonactionsui simonprogresstracking
  simoncontextui simonsampleshare)
else()
target_link_libraries(simon ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS}
  ${QT_LIBRARIES} ${QT_QTWEBKIT_LIBRARY}
  ${KDE4_KUTILS_LIBS} ${KDE4_KNEWSTUFF3_LIBS}
  simonappcore simonactions simoninfo
  simonlogging simonxml simonuicomponents simonsound simonscenarios 
  simonmodelmanagementui simonscenariobase simonscenarioui
  simonrecognitioncontrol simonactionsui simonprogresstracking
  simoncontextui simonsampleshare)
endif()

install (TARGETS simon DESTINATION bin COMPONENT simon)
install(FILES simonui.rc simon.log DESTINATION ${DATA_INSTALL_DIR} COMPONENT simon)
