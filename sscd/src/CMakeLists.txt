set(sscd_SRCS
  main.cpp
  sscdcontrol.cpp
  clientsocket.cpp
  databaseaccess.cpp
  sscqueries.cpp
  mysqlqueries.cpp
)

kde4_add_app_icon(sscd_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/../icons/hi*-app-sscd.png")

kde4_add_executable(sscd ${sscd_SRCS})

target_link_libraries(sscd ${QT_QTCORE_LIBRARY} ${QT_QTSQL_LIBRARY}
  ${QT_QTNETWORK_LIBRARY} sscobjects)

install(TARGETS sscd DESTINATION ${BIN_INSTALL_DIR} COMPONENT sscd)
install(FILES sscd.desktop DESTINATION ${XDG_APPS_INSTALL_DIR} COMPONENT sscd)