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

ki18n_wrap_ui(ssc_SRCS
  details.ui
  finduser.ui
  uploadsamples.ui
  deviceinformation.ui
  institution.ui
  main.ui
  modifyuserininstitution.ui
)

kconfig_add_kcfg_files(ssc_SRCS ../config/sscconfig.kcfgc)

file(GLOB ICONS_SRCS "../icons/hi*-app-ssc.png")
kde4_add_app_icon(ssc_SRCS ICONS ${ICONS_SRCS})
add_executable(ssc ${ssc_SRCS})

target_link_libraries(ssc ${QT_LIBRARIES} KF5::KCMUtils
   simonuicomponents simonprogresstracking sscobjects simonsound sscdaccess)

install(TARGETS ssc ${INSTALL_TARGETS_DEFAULT_ARGS})

########### install files ###############

install(FILES org.kde.ssc.desktop DESTINATION ${XDG_APPS_INSTALL_DIR} COMPONENT ssc)
install(FILES sscui.rc DESTINATION ${DATA_INSTALL_DIR} COMPONENT ssc)
