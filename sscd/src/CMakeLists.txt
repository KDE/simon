find_package(Qt4 COMPONENTS QtCore QtSql QtNetwork)
find_package(MySql)

if (MYSQL_FOUND)
add_definitions(-DMYSQL_PING_WORKAROUND)
endif (MYSQL_FOUND)

IF(WIN32)
	add_definitions(-D__INTERLOCKED_DECLARED -DMAKE_SSCD -DKDEWIN_STRINGS_H)
ENDIF(WIN32)

set(QT_CONFIG "terminal")

include_directories( ${KDE4_INCLUDES} ${QT_INCLUDES} 
	../../simonlib)

set(sscd_SRCS
   main.cpp
   sscdcontrol.cpp
   clientsocket.cpp
   databaseaccess.cpp
   sscqueries.cpp
   mysqlqueries.cpp
 )

kde4_add_app_icon(sscd_SRCS
"${CMAKE_CURRENT_SOURCE_DIR}/../icons/hi*-app-sscd.png")

kde4_add_executable(sscd ${sscd_SRCS})

if (MYSQL_FOUND)
	message(INFO "mysql was found!")
target_link_libraries(sscd ${QT_QTCORE_LIBRARY} ${QT_QTSQL_LIBRARY}
	${QT_QTNETWORK_LIBRARY} ${MYSQL_LIBRARIES} sscobjects)
else (MYSQL_FOUND)
	message(INFO "mysql was NOT found!")
target_link_libraries(sscd ${QT_QTCORE_LIBRARY} ${QT_QTSQL_LIBRARY}
	${QT_QTNETWORK_LIBRARY} sscobjects)
endif (MYSQL_FOUND)

install(TARGETS sscd DESTINATION ${BIN_INSTALL_DIR} COMPONENT sscd )


########### install files ###############

install( FILES sscd.desktop  DESTINATION ${XDG_APPS_INSTALL_DIR} COMPONENT sscd )
