set(ssc_SRCS
  main.cpp
  sscview.cpp
  modifyuser.cpp
  modifyuserininstitution.cpp
  manageinstitutions.cpp
  uploadsamples.cpp
  manageusers.cpp
  institutionmodel.cpp
  usermodel.cpp
  trainingswizard.cpp
  trainsamplepage.cpp
  sendsamplespage.cpp
  trainsampleintropage.cpp
  deviceinformationwidget.cpp
  deviceinformationpage.cpp
  sscsampledataprovider.cpp
)

kde4_add_ui_files(ssc_SRCS
  details.ui
  finduser.ui
  uploadsamples.ui
  deviceinformation.ui
  institution.ui
  main.ui
  modifyuserininstitution.ui
)

kde4_add_kcfg_files(ssc_SRCS ../config/sscconfig.kcfgc)

kde4_add_app_icon(ssc_SRCS "../icons/hi*-app-ssc.png")
kde4_add_executable(ssc ${ssc_SRCS})

target_link_libraries(ssc ${QT_LIBRARIES} ${KDE4_KDEUI_LIBS}
  ${KDE4_KUTILS_LIBS} simonuicomponents simonprogresstracking sscobjects simonsound sscdaccess)

install(TARGETS ssc DESTINATION ${BIN_INSTALL_DIR} COMPONENT ssc)

########### install files ###############

install(FILES ssc.desktop DESTINATION ${XDG_APPS_INSTALL_DIR} COMPONENT ssc)
install(FILES sscui.rc DESTINATION ${DATA_INSTALL_DIR} COMPONENT ssc)
