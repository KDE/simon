find_package(Qt4 COMPONENTS QtConcurrent  QtNetwork)
include_directories ( ${QT_INCLUDES} ${KDE_INCLUDES} ../../simonlib
	 ../)
	
set (QT_USE_QTNETWORK true)

include(${QT_USE_FILE})

set(ssc_SRCS
   main.cpp
   sscview.cpp
   modifyuser.cpp
   modifyuserininstitution.cpp
   manageinstitutions.cpp
   manageusers.cpp
   institutionmodel.cpp
   usermodel.cpp
   trainingswizard.cpp
   trainsamplepage.cpp
   sendsamplespage.cpp
   trainsampleintropage.cpp
   sscdaccess.cpp
   deviceinformationwidget.cpp
   deviceinformationpage.cpp
   sampledataprovider.cpp
 )

kde4_add_ui_files( ssc_SRCS
	${CMAKE_CURRENT_SOURCE_DIR}/details.ui
	${CMAKE_CURRENT_SOURCE_DIR}/finduser.ui
	${CMAKE_CURRENT_SOURCE_DIR}/deviceinformation.ui
	${CMAKE_CURRENT_SOURCE_DIR}/institution.ui
	${CMAKE_CURRENT_SOURCE_DIR}/main.ui
	${CMAKE_CURRENT_SOURCE_DIR}/modifyuserininstitution.ui
)

kde4_add_kcfg_files(ssc_SRCS ../config/sscconfig.kcfgc)

kde4_add_app_icon(ssc_SRCS
"${CMAKE_CURRENT_SOURCE_DIR}/../icons/hi*-app-ssc.png")
kde4_add_executable(ssc ${ssc_SRCS})

target_link_libraries(ssc ${QT_LIBRARIES} ${KDE4_KDEUI_LIBS}
	${KDE4_KUTILS_LIBS} simonuicomponents simonprogresstracking sscobjects simonsound)

install(TARGETS ssc DESTINATION ${BIN_INSTALL_DIR}  COMPONENT ssc)


########### install files ###############

install( FILES ssc.desktop  DESTINATION ${XDG_APPS_INSTALL_DIR}  COMPONENT ssc)
install(FILES sscui.rc DESTINATION ${DATA_INSTALL_DIR} COMPONENT ssc)

